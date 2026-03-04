#include <string>
#include <algorithm>
#include <fstream>
#include <random>
#include "deck.h"

std::string Card::rank_to_string() const {
    switch (card_value) {
        case rank::two: return "2";
        case rank::three: return "3";
        case rank::four: return "4";
        case rank::five: return "5";
        case rank::six: return "6";
        case rank::seven: return "7";
        case rank::eight: return "8";
        case rank::nine: return "9";
        case rank::ten: return "10";
        case rank::jack: return "J";
        case rank::queen: return "Q";
        case rank::king: return "K";
        case rank::ace: return "A";
    }
    return "?";
}

std::string Card::suit_to_string() const {
    switch (suit_value) {
        case suit::diamond: return "[D]";
        case suit::heart: return "[H]";
        case suit::spade: return "[S]";
        case suit::club: return "[C]";
    }
    return "?";
}

Card::Card(rank r, suit s) : card_value(r), suit_value(s) {}

Card::Card() : Card(rank::jack, suit::diamond) {}

int Card::value() const {
    if (card_value == rank::ace) {
        return 11;
    }
    if (card_value == rank::jack || card_value == rank::queen || card_value == rank::king) {
        return 10;
    }
    return static_cast<int>(card_value);
}

std::string Card::name() const {
    return rank_to_string() + suit_to_string();
}


Deck::Deck() {
    for (int s = 0; s < 4; s++) {
        for (int r = 2; r < 15; r++) {
            rank current_rank;
            switch (r) {
                case 2: current_rank = rank::two; break;
                case 3: current_rank = rank::three; break;
                case 4: current_rank = rank::four; break;
                case 5: current_rank = rank::five; break;
                case 6: current_rank = rank::six; break;
                case 7: current_rank = rank::seven; break;
                case 8: current_rank = rank::eight; break;
                case 9: current_rank = rank::nine; break;
                case 10: current_rank = rank::ten; break;
                case 11: current_rank = rank::jack; break;
                case 12: current_rank = rank::queen; break;
                case 13: current_rank = rank::king; break;
                case 14: current_rank = rank::ace; break;
            }
            cards.emplace_back(current_rank, static_cast<suit>(s));
        }
    }
}

void Deck::shuffle() {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::ranges::shuffle(cards, rng);
    top_index = 0;
}

Card Deck::deal() {
    if (top_index >= cards.size()) {
        throw std::out_of_range("No more cards in deck");
    }
    Card c = cards[top_index];
    top_index++;
    return c;
}