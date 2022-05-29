#include "Random.h"

namespace Network
{
    static std::random_device rd;
    std::mt19937 Random::gen(rd());

    void Random::Init()
    {
        s_random = Random();
    }

    int Random::randomIntNumber(int end)
    {
        auto num = std::uniform_int_distribution<int>(0, end);
        return num(Random::gen);
    }
     
    int Random::randomIntNumber(int begin, int end)
    {
        auto num = std::uniform_int_distribution<int>(begin, end);
        return num(Random::gen);
    }

    double Random::randomRealNumber(double begin, double end)
    {
        auto num = std::uniform_real_distribution<double>(begin, end);
        return num(gen);
    }

    /* 依据给定概率触发 */
    bool Random::randomTrigger(double probability)
    {

        double trigger = randomRealNumber(0, 1);
        if (!probability)
            return false;
        if (trigger <= probability)
            return true;
        return false;
    }
}
