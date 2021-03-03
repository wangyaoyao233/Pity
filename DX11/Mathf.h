#pragma once
#include <random>

class Mathf
{
public:
	//[from, to]
	template<typename T>
	static T Random(T from = 1, T to = 100)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_real_distribution<T> dist(from, to);
		return dist(rng);
	}

	static int Random(int from, int to)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<int> dist(from, to);
		return dist(rng);
	}

	static float Lerp(float from, float to, float t);

	template<typename T>
	static T Lerp(T from, T to, float t);
};

template<typename T>
inline T Mathf::Lerp(T from, T to, float t)
{
	if (t < 0)
	{
		t = 0;
	}
	if (t > 1)
	{
		t = 1;
	}
	return t * to + (1 - t) * from;
}