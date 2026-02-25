#include <string>
#include <iostream>
#include "helpers.h"

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

    std::string rank_to_string() const {
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

    std::string suit_to_string() const {
        switch (suit_value) {
            case suit::diamond: return "♦";
            case suit::heart: return "♥";
            case suit::spade: return "♠";
            case suit::club: return "♣";
        }
        return "?";
    }

public:
    Card(rank r, suit s) : card_value(r), suit_value(s) {}

    Card() : Card(rank::jack, suit::diamond) {}

    int value() const {
        if (card_value == rank::ace) {
            return 11;
        }
        if (card_value == rank::jack || card_value == rank::queen || card_value == rank::king) {
            return 10;
        }
        return static_cast<int>(card_value);
    }

    std::string name() const {
        return rank_to_string() + suit_to_string();
    }
};

class Deck {
private:
    size_t top_index = 0;
    std::vector<Card> cards;
public:
    Deck() {
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
    void shuffle(int n) {
        int total = cards.size();
        std::vector<double> probs(total);
        for (int i = 0; i < total; i++) {
            probs[i] = std::sqrt(nCr(total - 1, i));
        }

        for (int iter = 0; iter < n; iter++) {
            std::vector<int> keys(total);
            for (int i = 0; i < total; i++) {
                keys[i] = i;
            }
            int left = weighted_choice(keys, probs);
            if (left < 0) left = 0;
            if (left > total) left = total;
            std::vector left_arr(cards.begin(), cards.begin() + left);
            std::vector right_arr(cards.begin() + left, cards.end());

            std::vector<Card> new_deck;
            new_deck.reserve(total);

            while (!left_arr.empty() || !right_arr.empty()) {
                if (left_arr.empty()) {
                    new_deck.push_back(right_arr.front());
                    right_arr.erase(right_arr.begin());
                    continue;
                }
                if (right_arr.empty()) {
                    new_deck.push_back(left_arr.front());
                    left_arr.erase(left_arr.begin());
                    continue;
                }
                int total_remaining = left_arr.size() + right_arr.size();
                int num = randint(1, total_remaining);
                if (num > left_arr.size()) {
                    new_deck.push_back(right_arr.front());
                    right_arr.erase(right_arr.begin());
                } else {
                    new_deck.push_back(left_arr.front());
                    left_arr.erase(left_arr.begin());
                }
            }
            cards = new_deck;
        }
    }
    Card deal() {
        if (top_index >= cards.size()) {
            throw std::out_of_range("No more cards in deck");
        }
        Card c = cards[top_index];
        top_index++;
        return c;
    }
};

class Hand {
private:
    std::vector<Card> cards;
public:
    Hand() = default;
    int total() const {
        int total = 0;
        int numberOfA = 0;

        for (Card card : cards) {
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
    void add(Card c) {
        cards.push_back(c);
    }
    void show_cards(std::string name) const {
        std::cout << name + " cards are:\n";
        for (auto & card : cards) {
            std::cout << card.name() + "\n";
        }
    }
    bool is_bust() const {
        return total() > 21;
    }
    bool is_blackjack() const {
        return total() == 21;
    }

    bool game_ended() const {
        return total() >= 21;
    }
};

class Player {
private:
    int money_;
public:
    Player(int m) : money_(m) {}

    Player() : Player(1000) {}

    void show_money() const {
        std::cout << "You have " + std::to_string(money_) + " money\n";
    }

    int money() const {
        return money_;
    }

    int lose_money(int bet) {
        money_ -= bet;
    }

    int gain_money(int bet) {
        money_ += bet;
    }

    bool has_no_money() const {
        return money_ == 0;
    }
};

int main() {
    Deck d;
    d.shuffle(10);
    Player p;
    bool quit = false;
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
        if (bet == 0) {
            quit = true;
            continue;
        }
        p.lose_money(bet);
        Hand user;
        Hand dealer;
        for (int i = 0; i < 2; i++) {
            Card user_card = d.deal();
            user.add(user_card);
            Card dealer_card = d.deal();
            dealer.add(dealer_card);
        }

        bool gameEnd = false;
        int rounds = 0;
        user.show_cards("Your");

        if (user.is_blackjack()) {
            std::cout << "Blackjack! You won!\n";
            p.gain_money(bet * 3/2);
        } else {
            while (gameEnd == false and !user.game_ended()) {
                std::cout << "Hit (h) or Stand (s):\n";
                std::string option;
                std::cin >> option;
                while (option != "h" && option != "s") {
                    std::cout << "Hit (h) or Stand (s):\n";
                    std::cin >> option;
                }
                if (option == "h") {
                    Card c = d.deal();
                    user.add(c);
                    user.show_cards("Your");
                }
                if (option == "s") {
                    gameEnd = true;
                }
                rounds++;
            }
            if (user.is_bust()) {
                std::cout << "Bust! You lost!\n";
            }
            else {
                while (dealer.total() < 17) {
                    Card c = d.deal();
                    dealer.add(c);
                }
                dealer.show_cards("Dealer's");
                if (dealer.is_bust()) {
                    std::cout << "Dealer got bust. You won!\n";
                } else {
                    if (dealer.total() > user.total()) {
                        std::cout << "Dealer got more than you. You lost!\n";
                    } else if (dealer.total() == user.total()) {
                        std::cout << "You got the same as dealer. Draw!\n";
                        p.gain_money(bet);
                    } else {
                        std::cout << "You got more than the dealer. You won!\n";
                        p.gain_money(bet * 2);
                    }
                }
            }
        }
    }
    if (p.has_no_money()) {
        std::cout << "The game ended because you have no money!";
    } else {
        std::cout << "You ended the game with " + std::to_string(p.money()) + " money.";
    }
    return 0;
}