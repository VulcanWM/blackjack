#include <iostream>
#include <string>
#include <vector>
#include <cmath>

long long nCr(int n, int r)
{
    if (r == 0) return 1;
    if (r > n / 2) return nCr(n, n - r);

    long res = 1;

    for (int k = 1; k <= r; ++k)
    {
        res *= n - k + 1;
        res /= k;
    }

    return res;
}


std::vector<std::string> card_shuffle(int n) {
    int total = 52;
    double probs[total+1];
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

    return deck;
}

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    std::vector<std::string> deck = card_shuffle(50);

    for (int i = 0; i < 52; i++) {
        std::cout << deck[i] + "\n";
    }
    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}