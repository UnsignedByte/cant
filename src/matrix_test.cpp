//
// Created by Timothy Herchen on 4/11/21.
//

#include <immintrin.h>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <functional>
#include <algorithm>
#include <random>
#include "matrix.hpp"

const int SIZE = 1000000;

// The floats need to be on a 64-byte boundary for some of the instructions to work. We use these implementation-defined
// directives to do that.
/*#if defined(_WIN64)
__declspec(align(64)) float inputA[SIZE];
__declspec(align(64)) float inputB[SIZE];

__declspec(align(64)) float output[SIZE];
#elif defined(__unix__) || defined(__APPLE__)
float inputA[SIZE] __attribute__((aligned(64)));
float inputB[SIZE] __attribute__((aligned(64)));

float output[SIZE] __attribute__((aligned(64)));
#else
#endif*/

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_real_distribution<> dist(-10, 10); // distribution in range [1, 6]

void prepare_input_arrays() {
	auto f = [] () -> float { return dist(rng); };
}

void time_method(std::function< void(void) > functor, const char* name) {
	using namespace std::chrono;
	using clock = high_resolution_clock;

	const int evaluations = 1000;
	clock::time_point t1 = clock::now();

	for (int i = 0; i < evaluations; ++i) {
		functor();
	}

	clock::time_point t2 = clock::now();

	double milliseconds = 1000. * (duration_cast<duration<double> > (t2 - t1)).count() / evaluations;
	std::printf("Method %s took %.3f milliseconds on average.\n", name, milliseconds);
}


int main() {
	

	return 0;
}