// Card.cpp
#include "Card.h"

Card::Card(Rank rank, Suit suit)
    : m_rank(rank), m_suit(suit) {}

Card::Rank Card::getRank() const
{
    return m_rank;
}

Card::Suit Card::getSuit() const
{
    return m_suit;
}

int Card::getValue() const
{
    if (m_rank >= TWO && m_rank <= TEN)
    {
        return m_rank;
    }
    else if (m_rank >= JACK && m_rank <= KING)
    {
        return 10;
    }
    else
    {              // ACE
        return 11; // By default, Ace is worth 11 in Blackjack
    }
}

std::string Card::toString() const
{
    std::string rankStr;
    std::string suitStr;

    // Convert rank to string
    switch (m_rank)
    {
    case ACE:
        rankStr = "Ace";
        break;
    case JACK:
        rankStr = "Jack";
        break;
    case QUEEN:
        rankStr = "Queen";
        break;
    case KING:
        rankStr = "King";
        break;
    default:
        rankStr = std::to_string(m_rank);
        break;
    }

    // Convert suit to string
    switch (m_suit)
    {
    case CLUBS:
        suitStr = "Clubs";
        break;
    case DIAMONDS:
        suitStr = "Diamonds";
        break;
    case HEARTS:
        suitStr = "Hearts";
        break;
    case SPADES:
        suitStr = "Spades";
        break;
    }

    return rankStr + " of " + suitStr;
}
