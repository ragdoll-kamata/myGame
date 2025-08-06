#pragma once
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include <wrl.h>
#include <cstdint>
#include <vector>
#include <list>
#include <string>

class Audio
{
public:
	void Initialize();
	/// <summary>
	/// サウンドの読み込み(wav、MP3対応)
	/// </summary>
	/// <param name="filename">ファイルネーム</param>
	/// <returns>サウンドハンドル</returns>
	uint32_t LoadSound(const std::string& filename);

	/// <summary>
	/// サウンドの読み込み(まだwavしかできない)
	/// </summary>
	/// <param name="filename">ファイルネーム</param>
	/// <returns>サウンドハンドル</returns>
	uint32_t SoundLoadWave(const std::string& filename);

	uint32_t SoundLoadMP3(const std::string& filename);

	/// <summary>
	/// サウンドの再生
	/// </summary>
	/// <param name="soundHandle">サウンドハンドル</param>
	int SoundPlayWave(uint32_t soundHandle, bool isLoop);

	void StopLoopSound(int voiceHandle);

	/// <summary>
	/// 全てのサウンドのアンロード
	/// </summary>
	void AllSoundUnload();

	void SetDirectoryPath(std::string directoryPath) {
		directoryPath_ = directoryPath;
	}

	/// <summary>
	/// インスタンスゲット
	/// </summary>
	/// <returns>Audioの生ポインタ</returns>
	static Audio* GetInstance();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();


private:
	struct ChunkHeader {
		char id[4];
		int32_t size;
	};
	struct RiffHeader {
		ChunkHeader chunk;
		char type[4];
	};
	struct FormatChunk {
		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};
	struct SoundData {
		WAVEFORMATEX wfex;
		BYTE* pBuffer;
		unsigned int bufferSize;
		std::string filename;
		uint32_t index;
	};
private:
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice = nullptr;

	std::vector<SoundData> soundDates;

	static uint32_t kMaxSoundDates;


	std::vector<IXAudio2SourceVoice*> playingVoices; // ループ再生中の音声
	std::list<int> freeVoiceSlots; // 空いている音声スロット

	std::string directoryPath_;

private:
	static Audio* instance;

	Audio() = default;
	~Audio() = default;
	Audio(Audio&) = default;
	Audio& operator=(Audio&) = default;

};

