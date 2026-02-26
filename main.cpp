#include <string>
#include <iostream>
#include <algorithm>
#include <random>

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
    void shuffle() {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        std::ranges::shuffle(cards, rng);
        top_index = 0;
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
    void add(Card c) {
        cards.push_back(c);
    }
    void show_cards(std::string name) const {
        std::cout << name << " cards are:\n";
        for (auto & card : cards) {
            std::cout << card.name() << "\n";
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

    void empty_hand() {
        cards = {};
    }

    void show_first_card() const {
        std::cout << "Dealer's first card is " << cards[0].name() << "\n";
    }
};

class Player {
private:
    int money_;
    Hand hand;
public:
    Player(int m) : money_(m) {}

    Player() : Player(1000) {}

    void show_money() const {
        std::cout << "You have " << money_ << " money\n";
    }

    int money() const {
        return money_;
    }

    void lose_money(int bet) {
        money_ -= bet;
    }

    void gain_money(int bet) {
        money_ += bet;
    }

    bool has_no_money() const {
        return money_ == 0;
    }

    void add_card(Card c) {
        hand.add(c);
    }

    void show_first_card() const {
        hand.show_first_card();
    }

    void show_cards(const std::string &name) const {
        hand.show_cards(name);
    }

    bool is_blackjack() const {
        return hand.is_blackjack();
    }

    bool is_bust() const {
        return hand.is_bust();
    }

    bool game_ended() const {
        return hand.game_ended();
    }

    void empty_hand() {
        hand.empty_hand();
    }

    int total() const {
        return hand.total();
    }
};

class Game {
private:
    Deck d;
    Player p;
    Player dealer;
    bool quit = false;
public:
    Game() = default;
    void play() {
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
                p.gain_money(bet * 5/2);
            } else {
                while (gameEnd == false and !p.game_ended()) {
                    std::cout << "Hit (h) or Stand (s):\n";
                    std::string option;
                    std::cin >> option;
                    while (option != "h" && option != "s") {
                        std::cout << "Hit (h) or Stand (s):\n";
                        std::cin >> option;
                    }
                    if (option == "h") {
                        Card c = d.deal();
                        p.add_card(c);
                        p.show_cards("Your");
                    }
                    if (option == "s") {
                        gameEnd = true;
                    }
                }
                if (p.is_bust()) {
                    std::cout << "Bust! You lost!\n";
                }
                else {
                    while (dealer.total() < 17) {
                        Card c = d.deal();
                        dealer.add_card(c);
                    }
                    dealer.show_cards("Dealer's");
                    if (dealer.is_bust()) {
                        std::cout << "Dealer got bust. You won!\n";
                        p.gain_money(bet * 2);
                    } else {
                        if (dealer.total() > p.total()) {
                            std::cout << "Dealer got more than you. You lost!\n";
                        } else if (dealer.total() == p.total()) {
                            std::cout << "You got the same as dealer. Draw!\n";
                            p.gain_money(bet);
                        } else {
                            std::cout << "You got more than the dealer. You won!\n";
                            p.gain_money(bet * 2);
                        }
                    }
                }
            }
            p.empty_hand();
            dealer.empty_hand();

            d.shuffle();
        }
        if (p.has_no_money()) {
            std::cout << "The game ended because you have no money!";
        } else {
            std::cout << "You ended the game with " << p.money() << " money.";
        }
    }
};

int main() {
    Game g;
    g.play();
    return 0;
}