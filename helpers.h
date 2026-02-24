#ifndef BLACKJACK_HELPERS_H
#define BLACKJACK_HELPERS_H

#include <vector>
#include <random>

long long nCr(int n, int r);

int weighted_choice(const std::vector<int>& keys,
                    const std::vector<double>& weights);

int randint(int min, int max);

std::vector<int> range(int start, int end);

#endif //BLACKJACK_HELPERS_H