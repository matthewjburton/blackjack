// Card.h
#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>

class Card
{
public:
    enum Rank
    {
        ACE = 1,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING
    };

    enum Suit
    {
        CLUBS,
        DIAMONDS,
        HEARTS,
        SPADES
    };

    Card(Rank rank, Suit suit);
    Rank getRank() const;
    Suit getSuit() const;
    int getValue() const; // Returns the value of the card in Blackjack
    std::string toString() const;

private:
    Rank m_rank;
    Suit m_suit;
};

#endif // CARD_H
