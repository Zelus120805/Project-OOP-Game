#include "Random.h"

int Random::getRandomInt(const int& min, const int& max) {
    return min + rand() % ((max - min) + 1);
}

float Random::getRandomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}