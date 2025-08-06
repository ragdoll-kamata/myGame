#include "Audio.h"
#include <cassert>
#include <fstream>
#include <sstream>

#define MINIMP3_IMPLEMENTATION
#include "minimp3_ex.h"

uint32_t Audio::kMaxSoundDates = 100;
Audio* Audio::instance = nullptr;
void Audio::Initialize()
{
	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));

	soundDates.reserve(kMaxSoundDates);
	directoryPath_ = "Resource/Audio";
}

uint32_t Audio::LoadSound(const std::string& filename) {
	if (filename.ends_with(".wav")) {
		return SoundLoadWave(filename);
	} else if (filename.ends_with(".mp3")) {
		return SoundLoadMP3(filename);
	}
	assert(false && "Unsupported format");
	return -1;
}

uint32_t Audio::SoundLoadWave(const std::string& filename)
{
	for (SoundData soundData : soundDates) {
		if (soundData.filename == filename) {
			return soundData.index;
		}
	}

	std::ifstream file;

	file.open(directoryPath_ + "/" + filename, std::ios_base::binary);

	assert(file.is_open());

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(false);
	}

	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(false);
	}

	FormatChunk format;
	file.read((char*)&format, sizeof(ChunkHeader));

	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(false);
	}

	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);


	ChunkHeader data;
	file.read((char*)&data, sizeof(data));


	if (strncmp(data.id, "JUNK", 4) == 0) {
		file.seekg(data.size, std::ios_base::cur);

		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "LIST", 4) == 0) {
		file.seekg(data.size, std::ios_base::cur);

		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(false);
	}
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	file.close();

	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;
	soundData.filename = filename;

	soundDates.push_back(soundData);
	soundDates[soundDates.size() - 1].index = static_cast<uint32_t>(soundDates.size()) - 1;
	return static_cast<uint32_t>(soundDates.size()) - 1;
}

uint32_t Audio::SoundLoadMP3(const std::string& filename) {
	// 既に読み込み済みか確認
	for (const SoundData& soundData : soundDates) {
		if (soundData.filename == filename) {
			return soundData.index;
		}
	}
	// ファイルオープン
	mp3dec_ex_t dec;
	std::string fullPath = directoryPath_ + "/" + filename;
	if (mp3dec_ex_open(&dec, fullPath.c_str(), MP3D_SEEK_TO_SAMPLE)) {
		assert(false && "Failed to open MP3");
	}

	// デコード後の PCM データ
	size_t samples = dec.samples;
	size_t pcmSize = samples * sizeof(mp3d_sample_t);
	mp3d_sample_t* pcmData = new mp3d_sample_t[samples];
	mp3dec_ex_read(&dec, pcmData, samples);

	// WAVEFORMATEX を構築
	WAVEFORMATEX wfex = {};
	wfex.wFormatTag = WAVE_FORMAT_PCM;
	wfex.nChannels = dec.info.channels;
	wfex.nSamplesPerSec = dec.info.hz;
	wfex.wBitsPerSample = sizeof(mp3d_sample_t) * 8;
	wfex.nBlockAlign = wfex.nChannels * wfex.wBitsPerSample / 8;
	wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

	// SoundData 登録
	SoundData soundData = {};
	soundData.wfex = wfex;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pcmData);
	soundData.bufferSize = static_cast<uint32_t>(pcmSize);
	soundData.filename = filename;

	soundDates.push_back(soundData);
	soundDates[soundDates.size() - 1].index = static_cast<uint32_t>(soundDates.size()) - 1;

	// ファイルクローズ
	mp3dec_ex_close(&dec);

	// SH返し
	return soundDates.back().index;
}

int Audio::SoundPlayWave(uint32_t soundHandle, bool isLoop)
{
	HRESULT result;

	IXAudio2SourceVoice* pSoundVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSoundVoice, &soundDates[soundHandle].wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundDates[soundHandle].pBuffer;
	buf.AudioBytes = soundDates[soundHandle].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	if (isLoop) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
		buf.LoopBegin = 0;
		buf.LoopLength = 0;
	}

	result = pSoundVoice->SubmitSourceBuffer(&buf);
	
	result = pSoundVoice->Start();
	assert(SUCCEEDED(result));

	if (isLoop) {
		int index = -1;
		if (!freeVoiceSlots.empty()) {
			// 空きスロットがあれば再利用（先頭を使う）
			index = freeVoiceSlots.front();
			freeVoiceSlots.pop_front();
			playingVoices[index] = pSoundVoice;
		} else {
			// なければ新しく追加
			playingVoices.push_back(pSoundVoice);
			index = static_cast<int>(playingVoices.size()) - 1;
		}
		return index;
	} else {
		// 使い捨て音声（ループでないSEなど）は破棄してもよい（後で自動管理するともっと良い）
		// 今は使い捨てとし、明示的な Stop は行わない
		return -1; // 無効な voiceHandle
	}
}

void Audio::StopLoopSound(int voiceHandle) {
	if (voiceHandle < 0 || voiceHandle >= static_cast<int>(playingVoices.size())) return;
	if (playingVoices[voiceHandle]) {
		playingVoices[voiceHandle]->Stop();
		playingVoices[voiceHandle]->DestroyVoice();
		playingVoices[voiceHandle] = nullptr;
		freeVoiceSlots.push_front(voiceHandle);  // 空きインデックスをリストの先頭に戻す
	}
}


void Audio::AllSoundUnload()
{
	for (SoundData soundData : soundDates) {
		delete[] soundData.pBuffer;
		soundData.pBuffer = 0;
		soundData.bufferSize = 0;
		soundData.wfex = {};
	}
	soundDates.clear();
}

Audio* Audio::GetInstance()
{
	if (instance == nullptr) {
		instance = new Audio;
	}
	return instance;
}

void Audio::Finalize()
{
	for (auto voice : playingVoices) {
		if (voice) {
			voice->Stop();
			voice->DestroyVoice();
		}
	}
	playingVoices.clear();
	freeVoiceSlots.clear();

	AllSoundUnload();
	delete instance;
	instance = nullptr;
}
