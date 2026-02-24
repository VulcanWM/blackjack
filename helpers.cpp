#include "helpers.h"

long long nCr(int n, int r)
{
    if (r == 0) return 1;
    if (r > n / 2) return nCr(n, n - r);

    long long res = 1;

    for (int k = 1; k <= r; ++k)
    {
        res *= n - k + 1;
        res /= k;
    }

    return res;
}

int weighted_choice(const std::vector<int>& keys,
                    const std::vector<double>& weights) {

    static std::random_device rd;
    static std::mt19937 gen(rd());  // only seeded once

    std::discrete_distribution<> dist(weights.begin(), weights.end());
    int index = dist(gen);

    return keys[index];
}

int randint(const int min, const int max) {
    static std::random_device rd;    // seed only once
    static std::mt19937 gen(rd());   // mersenne twister engine
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

std::vector<int> range(int start, int end) {
    std::vector<int> result;
    for (int i = start; i <= end; i++) {
        result.push_back(i);
    }
    return result;
}