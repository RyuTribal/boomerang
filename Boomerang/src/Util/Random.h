#pragma once
#include <random>

#include "Util/Log.h"

namespace Boomerang {
    class Random {
    private:
        std::mt19937 m_gen; // Mersenne Twister random number generator

    public:
        Random() : m_gen(std::random_device()()) {}

        int GetInt(int min, int max) {
            std::uniform_int_distribution<> dis(min, max);
            return dis(m_gen);
        }

        int GetInt(const std::vector<int>& vec) {
            if (vec.empty()) {
                HANDLE_ERROR(ErrorType::BOOMERANG, "Vector is empty");
            }

            int index = GetInt(0, vec.size() - 1);
            return vec[index];
        }
    };
}
