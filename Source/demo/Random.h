#pragma once

#include <random>

namespace Network
{
    class Random
    {
    public:
        static void Init();

        /* 随机实数 */
        static double randomRealNumber(double begin=-1, double end=1);

        /* 随机整数 */
        static int randomIntNumber(int end);
        static int randomIntNumber(int begin, int end);

        /* 依据给定概率触发 */
        static bool randomTrigger(double probability);


    private:
        static std::mt19937 gen;

        static Random s_random;
    };
}
