#include "Random.h"

std::mt19937 Random::randomNumberGenerator;

void Random::Init() 
{
    randomNumberGenerator.seed(12345);
}

int Random::GetRandomInt(int min, int max) 
{ 
    std::uniform_int_distribution<int> dist(min, max);
    return dist(randomNumberGenerator);
}

float Random::GetRandomFloat(float min, float max) 
{ 
    std::uniform_real_distribution<float> dist(min, max);
    return dist(randomNumberGenerator);
}
