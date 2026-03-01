#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <random>
#include "deck.h"
#include "game.h"

Hand::Hand() = default;

int Hand::total() const {
    int total = 0;
    int numberOfA = 0;

    for (const Card& card : cards){
        if (card.value() == 11) {
            numberOfA++;
        }
        total += card.value();
    }

    while (numberOfA > 0 && total > 21) {
        numberOfA--;
        total -= 10;
    }

    return total;
}

void Hand::add(Card c) {
    cards.push_back(c);
}

void Hand::show_cards(std::string name) const {
    std::cout << name << " cards are:\n";
    for (auto & card : cards) {
        std::cout << card.name() << "\n";
    }
}

bool Hand::is_bust() const {
    return total() > 21;
}

bool Hand::is_blackjack() const {
    return total() == 21;
}

bool Hand::game_ended() const {
    return total() >= 21;
}

void Hand::empty_hand() {
    cards.clear();
}

void Hand::show_first_card() const {
    std::cout << "Dealer's first card is " << cards[0].name() << "\n";
}

Player::Player(int m) : money_(m) {}

Player::Player() : Player(1000) {}

void Player::show_money() const {
    std::cout << "You have " << money_ << " money\n";
}

int Player::money() const {
    return money_;
}

void Player::lose_money(int bet) {
    money_ -= bet;
}

void Player::gain_money(int bet) {
    money_ += bet;
}

bool Player::has_no_money() const {
    return money_ == 0;
}

void Player::add_card(Card c) {
    hand.add(c);
}

void Player::show_first_card() const {
    hand.show_first_card();
}

void Player::show_cards(const std::string& name) const {
    hand.show_cards(name);
}

bool Player::is_blackjack() const {
    return hand.is_blackjack();
}

bool Player::is_bust() const {
    return hand.is_bust();
}

bool Player::game_ended() const {
    return hand.game_ended();
}

void Player::empty_hand() {
    hand.empty_hand();
}

int Player::total() const {
    return hand.total();
}

Game::Game() : high_score(get_highscore()) {}

void Game::play() {
    std::cout << "Highscore: " << high_score << "\n";

    d.shuffle();
    while (quit == false and !p.has_no_money()) {
        p.show_money();
        int bet;
        std::cout << "How much do you want to bet? (Enter 0 if you want to quit)\n";
        std::cin >> bet;
        while (bet > p.money()) {
            std::cout << "You don't have this much money!\n";
            std::cout << "How much do you want to bet? (Enter 0 if you want to quit)\n";
            std::cin >> bet;
        }
        if (bet <= 0) {
            quit = true;
            continue;
        }
        if (bet >= (3.0/4.0) * p.money()) {
            reckless_bets++;
        }
        p.lose_money(bet);
        for (int i = 0; i < 2; i++) {
            Card user_card = d.deal();
            p.add_card(user_card);
            Card dealer_card = d.deal();
            dealer.add_card(dealer_card);
        }

        bool gameEnd = false;
        dealer.show_first_card();
        p.show_cards("Your");

        if (p.is_blackjack()) {
            std::cout << "Blackjack! You won!\n";
            p.gain_money(bet * (5.0/2.0));
            blackjack_wins++;
        } else {
            while (gameEnd == false and !p.game_ended()) {
                std::cout << "Hit (H) or Stand (S):\n";
                std::string option;
                std::cin >> option;
                std::ranges::transform(option, option.begin(), ::toupper);
                while (option != "H" && option != "S") {
                    std::cout << "Hit (H) or Stand (S):\n";
                    std::cin >> option;
                    std::ranges::transform(option, option.begin(), ::toupper);
                }
                if (option == "H") {
                    Card c = d.deal();
                    p.add_card(c);
                    p.show_cards("Your");
                }
                if (option == "S") {
                    gameEnd = true;
                }
            }
            if (p.is_bust()) {
                std::cout << "Bust! You lost!\n";
                losses_by_bust++;
            }
            else {
                int dealer_total = dealer.total();
                while (dealer_total < 17) {
                    Card c = d.deal();
                    dealer.add_card(c);
                    dealer_total = dealer.total();
                }

                dealer.show_cards("Dealer's");
                if (dealer.is_bust()) {
                    std::cout << "Dealer got bust. You won!\n";
                    wins++;
                    p.gain_money(bet * 2);
                } else {
                    if (dealer_total > p.total()) {
                        std::cout << "Dealer got more than you. You lost!\n";
                        losses_by_lower_total++;
                    } else if (dealer_total == p.total()) {
                        std::cout << "You got the same as dealer. Draw!\n";
                        draws++;
                        p.gain_money(bet);
                    } else {
                        std::cout << "You got more than the dealer. You won!\n";
                        wins++;
                        p.gain_money(bet * 2);
                    }
                }
            }
        }
        rounds_played++;
        p.empty_hand();
        dealer.empty_hand();

        d.shuffle();
    }
    if (p.has_no_money()) {
        std::cout << "The game ended because you have no money!\n";
    } else {
        std::cout << "You ended the game with " << p.money() << " money.\n";
        if (p.money() > high_score) {
            std::cout << "New highscore!\n";
            save_highscore(p.money());
        }
    }
    personality_type();
}

void Game::personality_type() {
    std::cout << "Your blackjack personality type is: ";
    if (p.money() < 500) {
        if (reckless_bets > ((2.0/3.0) * rounds_played)) {
            std::cout << "Reckless";
        } else if (losses_by_lower_total >= losses_by_bust) {
            std::cout << "Scared";
        } else {
            std::cout << "Overconfident";
        }
    } else if (p.money() < 1500) {
        std::cout << "Stable";
    } else {
        if (blackjack_wins > 2 && (blackjack_wins / rounds_played) > (2.0/10.0)) {
            std::cout << "Lucky";
        } else if (rounds_played < 10) {
            std::cout << "Fluke";
        } else {
            if (p.money() > 5000) {
                std::cout << "WINNER";
            } else {
                std::cout << "Winner";
            }
        }
    }
}

int Game::get_highscore() {
    std::ifstream file("highscore.txt");
    if (!file) {
        return 0;
    }
    std::string firstLine;
    if (!std::getline(file, firstLine) || firstLine.empty()) {
        return 0;
    }
    try {
        return std::stoi(firstLine);
    } catch (...) {
        return 0;
    }
}

void Game::save_highscore(int score) {
    std::ofstream file("highscore.txt");

    if (!file) {
        std::cout << "Cannot save highscore. Lack of permissions to write to fail.\n";
        return;
    }

    file << score;
}