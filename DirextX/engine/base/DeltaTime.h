#pragma once
#include "chrono"
class DeltaTime
{
public:
	static DeltaTime* GetInstance();

	void Initialize();

	void Finalize();

	void Update();

	float GetDeltaTime() const { return deltaTime; }



private:


	static DeltaTime* instance;

	std::chrono::steady_clock::time_point reference_;

	float deltaTime = 0.0f;

	DeltaTime() = default;
	~DeltaTime() = default;
	DeltaTime(DeltaTime&) = default;
	DeltaTime& operator=(DeltaTime&) = default;

};

