#include <random>

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<double> dist(0.0, 1.0);

double rand_01()
{
	return dist(mt);
}

int urand(int min, int max){
	return static_cast<int>(rand_01()*(max-min)+min);
}