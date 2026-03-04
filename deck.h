#ifndef BLACKJACK_DECK_H
#define BLACKJACK_DECK_H

#include <string>
#include <fstream>
#include <vector>

enum class rank {
    two = 2,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    ace
};

enum class suit {
    diamond,
    heart,
    spade,
    club
};

class Card {
private:
    rank card_value;
    suit suit_value;

    std::string rank_to_string() const;

    std::string suit_to_string() const;

public:
    Card(rank r, suit s);

    Card();

    int value() const;

    std::string name() const;
};

class Deck {
private:
    size_t top_index = 0;
    std::vector<Card> cards;
public:
    Deck();
    void shuffle();
    Card deal();
};

#endif //BLACKJACK_DECK_H