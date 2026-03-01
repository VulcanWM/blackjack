#ifndef BLACKJACK_GAME_H
#define BLACKJACK_GAME_H

#include <string>
#include "deck.h"

class Hand {
private:
    std::vector<Card> cards;
public:
    Hand();
    int total() const;
    void add(Card c);
    void show_cards(std::string name) const;
    bool is_bust() const;
    bool is_blackjack() const;

    bool game_ended() const;

    void empty_hand();

    void show_first_card() const;
};

class Player {
private:
    int money_;
    Hand hand;
public:
    Player(int m);

    Player();

    void show_money() const;

    int money() const;

    void lose_money(int bet);

    void gain_money(int bet);

    bool has_no_money() const;

    void add_card(Card c);

    void show_first_card() const;

    void show_cards(const std::string& name) const;

    bool is_blackjack() const;

    bool is_bust() const;

    bool game_ended() const;

    void empty_hand();

    int total() const;
};

class Game {
private:
    Deck d;
    Player p;
    Player dealer;
    bool quit = false;

    int rounds_played = 0;

    int reckless_bets = 0;

    int wins = 0;
    int blackjack_wins = 0;

    int losses_by_bust = 0;
    int losses_by_lower_total = 0;

    int draws = 0;

    int high_score;

public:
    Game();
    void play();

    void personality_type();

    int get_highscore();

    void save_highscore(int score);
};

#endif //BLACKJACK_GAME_H