#include <iostream>
#include <string>
#include <vector>
#include "../card_shuffling.h"
#include "helpers.h"

std::vector<std::string> card_shuffle(int n) {
    int total = 52;
    std::vector<double> probs(total+1);
    for (int i = 0; i < total + 1; i++) {
        double prob = pow(nCr(total, i), 0.5);
        probs[i] = prob;
    }

    char suits[4] = {'H', 'D', 'C', 'S'};

    std::string ranks[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    std::vector<std::string> deck(52);
    for (int suit = 0; suit < 4; suit++) {
        for (int rank = 0; rank < 13; rank++) {
            deck[suit * 13 + rank] = ranks[rank] + " " + suits[suit];
        }
    }

    for (int i = 0; i < n; i++) {
        std::vector<int> keys = range(0, total);

        int left = weighted_choice(keys, probs);
        int right = total - left;
        std::vector leftArr(deck.begin(), deck.begin() + left);
        std::vector rightArr(deck.begin() + left, deck.end());

        std::vector<std::string> new_deck;
        for (int j = 0; j < total; j++) {
            int num = randint(1, left + right);
            if (num > left) {
                new_deck.push_back(rightArr[0]);
                right--;
                rightArr.erase(rightArr.begin());
            } else {
                new_deck.push_back(leftArr[0]);
                left--;
                leftArr.erase(leftArr.begin());
            }
        }

        for (int j = 0; j < total; j++) {
            deck[j] = new_deck[j];
        }
    }

    return deck;
}