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

const int SIZE = 1000000;

// The floats need to be on a 64-byte boundary for some of the instructions to work. We use these implementation-defined
// directives to do that.
#if defined(_WIN64)
__declspec(align(64)) float inputA[SIZE];
__declspec(align(64)) float inputB[SIZE];

__declspec(align(64)) float output[SIZE];
#elif defined(__unix__) || defined(__APPLE__)
float inputA[SIZE] __attribute__((aligned(64)));
float inputB[SIZE] __attribute__((aligned(64)));

float output[SIZE] __attribute__((aligned(64)));
#else
#endif

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_real_distribution<> dist(-10, 10); // distribution in range [1, 6]

void prepare_input_arrays() {
	auto f = [] () -> float { return dist(rng); };

	std::generate_n(inputA, SIZE, f);
	std::generate_n(inputB, SIZE, f);
}

void clear_output_array() {
	std::fill_n(output, SIZE, 0);
}

void compute_via_normal_loop() {
	for (int i = 0; i < SIZE; ++i) {
		output[i] = inputA[i] * inputB[i];
	}
}

void compute_via_intrinsics() {
	// the 512-bit AVX instructions require the floats to be aligned on a 64-byte boundary, which the aligned attributes
	// up there have guaranteed. Since SIZE is a multiple of 8 in this case, we can just do this without considering the
	// later stuff.
	//_mm512_load_epi64(output);
#if defined(__AVX__)
	float *end_array_a = inputA + SIZE;

	for (float *a_ptr = inputA, *b_ptr = inputB, *out_ptr = output; a_ptr < end_array_a; a_ptr += 16, b_ptr += 16, out_ptr += 16) {
		__m256 a_pack1 = _mm256_load_ps(a_ptr);
		__m256 a_pack2 = _mm256_load_ps(a_ptr + 8);
		__m256 b_pack1 = _mm256_load_ps(b_ptr);
		__m256 b_pack2 = _mm256_load_ps(b_ptr + 8);

		__m256 out_pack1 = _mm256_mul_ps(a_pack1, b_pack1);
		__m256 out_pack2 = _mm256_mul_ps(a_pack2, b_pack2);

		_mm256_store_ps(out_ptr, out_pack1);
		_mm256_store_ps(out_ptr + 8, out_pack2);
	}
#endif
}

float get_output_sum() {
	float sum = 0;
	for (float i : output) {
		sum += i;
	}
	return sum;
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

	std::printf("Computed sum was: %f.\n", get_output_sum());
}


int main() {
	compute_via_intrinsics();
	/*prepare_input_arrays();
	compute_via_normal_loop();

	std::printf("Correct sum should be: %f.\n", get_output_sum());

	time_method(compute_via_normal_loop, "");
	clear_output_array();

	time_method(compute_via_intrinsics, "");
	clear_output_array();*/

	return 0;
}