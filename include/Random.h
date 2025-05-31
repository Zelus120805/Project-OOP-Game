#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <iostream>

class Random {
public:
    static int getRandomInt(const int& min, const int& max);
    static float getRandomFloat(float min, float max);
};

#endif // _RANDOM_H_