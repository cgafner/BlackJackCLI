#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <ctime>
#include <algorithm> // for "shuffle"

using namespace std;

enum Rank {
    ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING
};

enum Suit {
    HEARTS, DIAMONDS, CLUBS, SPADES
};

/* Card Class
 * This class is responsible for creating a card with a Rank and Suit.
 * Any given card only knows about its own rank and suit.
 * getRank(): returns the rank of the card.
 * getSuit(): returns the suit of the card.
 */
class Card {
public:
    Card(Rank rank, Suit suit) : rank_(rank), suit_(suit) {}

    Rank getRank() const { return rank_; }
    Suit getSuit() const { return suit_; }

private:
    Rank rank_;
    Suit suit_;
};


/* Deck Class
 * This class is responsible for creating a deck with all 52 cards.
 * Will make all ranks for each suit until all cards are made.
 * Will add each card to a vector as a unique_ptr, for memory safety.
 * shuffledeck(): This method will sort the cards randomly.
 * drawCard(): Will draw a card from the deck if possible and return it.
 * cards_: A private vector containing all cards in the Deck
 */
class Deck {
public:
    Deck() {
        for (int suit = HEARTS; suit <= SPADES; ++suit) {
            for (int rank = ACE; rank <= KING; ++rank) {
                cards_.push_back(make_unique<Card>(static_cast<Rank>(rank), static_cast<Suit>(suit)));
            }
        }
    }

    void shuffledeck() {
        random_device rd;
        mt19937 g(rd());
        shuffle(cards_.begin(), cards_.end(), g);
    }

    unique_ptr<Card> drawCard() {
        if (cards_.empty()) {
            return nullptr;
        }
        unique_ptr<Card> card = std::move(cards_.back());
        cards_.pop_back();
        return card;
    }

private:
    vector<unique_ptr<Card> > cards_;
};


/* Player Class
 * This class is responsible for keeping track of a player's cards and their total hand value.
 * addCard(): This method will add a card from the Deck.
 * getHandValue(): This method calculates the total hand value of the player.
 * hand_: A private vector that contains all cards in the player's hand.
 */
class Player {
public:
    Player(string name)
    {
        name_ = name;
    }
    void addCard(unique_ptr<Card> card) {
        hand_.push_back(std::move(card));
    }

    int getHandValue() const {
        int value = 0;
        int aces = 0;
        for (const auto& card : hand_) {
            int cardValue = card->getRank();
            if (cardValue >= 10) {
                cardValue = 10;
            }
            else if (cardValue == ACE) {
                aces++;
                cardValue = 11;
            }
            value += cardValue;
        }

        while (value > 21 && aces > 0) {
            value -= 10;
            aces--;
        }

        if (value > 21) {
            value = -1;
        }

        return value;
    }

    void printHandValue(int value) const
    {
        cout << name_ << " hand value: " << value << '\n';
    }

private:
    vector<unique_ptr<Card> > hand_;
    string name_;
};

int main() {
    Deck deck;
    deck.shuffledeck();

    Player player("player");
    Player dealer("dealer");

    while(player.getHandValue() != -1 || dealer.getHandValue() != -1)
    {
    	player.addCard(deck.drawCard());
    	player.addCard(deck.drawCard());
        player.printHandValue(player.getHandValue());

    	dealer.addCard(deck.drawCard());
    	dealer.addCard(deck.drawCard());
        dealer.printHandValue(dealer.getHandValue());

        cout << "\nNext deal." << "\n\n";
    }

    cout << "\n\nGame over.";

    return 0;
}