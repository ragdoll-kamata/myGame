#pragma once

enum class RandomRotateType {
	None = 0,
	X = 1 << 0,
	Y = 1 << 1,
	Z = 1 << 2
};

inline RandomRotateType operator|(RandomRotateType a, RandomRotateType b) {
	return static_cast<RandomRotateType>(static_cast<int>(a) | static_cast<int>(b));
}
inline bool operator&(RandomRotateType a, RandomRotateType b) {
	return (static_cast<int>(a) & static_cast<int>(b));
}
