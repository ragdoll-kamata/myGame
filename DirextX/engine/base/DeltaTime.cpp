#include "DeltaTime.h"

DeltaTime* DeltaTime::instance = nullptr;
DeltaTime* DeltaTime::GetInstance()
{
    if (instance == nullptr) {
        instance = new DeltaTime;
		instance->Initialize();
    }
    return instance;
}

void DeltaTime::Finalize()
{
	delete instance;
	instance = nullptr;
}

void DeltaTime::Update()
{
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f));


	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	deltaTime = static_cast<float>(elapsed.count()) / static_cast<float>(kMinTime.count());

	reference_ = std::chrono::steady_clock::now();
}

void DeltaTime::Initialize()
{
	reference_ = std::chrono::steady_clock::now();
}
