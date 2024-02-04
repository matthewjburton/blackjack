#include <iostream>
#include <ctime>
#include <cstdlib> // Include <cstdlib> for rand()
#include <vector>
#include "Card.h"
using namespace std;

enum class GameState
{
  Betting,
  Player,
  Dealer,
  Result,
  End
};

// Function prototypes
void GetCard(vector<Card> &cards);
void PrintCards(const vector<Card> &cards);
int CalculateTotal(const vector<Card> &cards);
void PlaceBet(double &playerBet, double &money);
void DealNewHand(vector<Card> &playerCards, vector<Card> &dealerCards);
void PlayerActions(vector<Card> &playerCards, GameState &state, double &money, double playerBet);
void ShowHands(vector<Card> playerCards, vector<Card> dealerCards);
void DetermineResult(int playerTotal, int dealerTotal, double &money, double playerBet);
void PlayAgain(GameState &state);

int main()
{
  GameState state = GameState::Betting;
  double playerBet;
  double money = 100.0;
  int playerTotal, dealerTotal;
  vector<Card> playerCards, dealerCards;

  srand(time(0)); // Seed the randomizer

  while (state != GameState::End)
  {
    switch (state)
    {
    case GameState::Betting:
      PlaceBet(playerBet, money);
      DealNewHand(playerCards, dealerCards);
      state = GameState::Player;
      break;

    case GameState::Player:
      PlayerActions(playerCards, state, money, playerBet);
      ShowHands(playerCards, dealerCards);

      playerTotal = CalculateTotal(playerCards);

      if (playerTotal == 21)
        state = GameState::Dealer;

      if (playerTotal > 21)
        state = GameState::Result;
      break;

    case GameState::Dealer:
      GetCard(dealerCards);

      dealerTotal = CalculateTotal(dealerCards);

      while (dealerTotal < 16)
      {
        GetCard(dealerCards);
        dealerTotal = CalculateTotal(dealerCards);
      }

      ShowHands(playerCards, dealerCards);
      state = GameState::Result;
      break;

    case GameState::Result:
      DetermineResult(playerTotal, dealerTotal, money, playerBet);
      PlayAgain(state);
      break;

    case GameState::End:
      break;
    }
  }
  return 0;
}

void GetCard(vector<Card> &cards)
{
  int cardValue = rand() % 13 + 1;
  Card::Rank rank = static_cast<Card::Rank>(cardValue);
  Card::Suit suit = static_cast<Card::Suit>(rand() % 4);
  Card newCard(rank, suit);
  cards.push_back(newCard);
}

void PrintCards(const vector<Card> &cards)
{
  for (const auto &card : cards)
    cout << card.toString() << " ";
  cout << endl;
}

int CalculateTotal(const vector<Card> &cards)
{
  int total = 0;
  int num_aces = 0; // Count the number of aces

  for (const auto &card : cards)
  {
    total += card.getValue();
    if (card.getValue() == 11) // Check if the card is an Ace
      num_aces++;
  }

  // Adjust the total value for Aces if needed
  while (total > 21 && num_aces > 0)
  {
    total -= 10; // Convert an Ace from 11 to 1
    num_aces--;
  }
  return total;
}

void PlaceBet(double &playerBet, double &money)
{
  cout << "Money available: $" << money << endl;
  cout << "Place your bet. $";
  cin >> playerBet;

  while (playerBet <= 0 || playerBet > money)
  {
    if (playerBet == 0)
      cout << "Can't bet nothing" << endl;

    if (playerBet < 0)
      cout << "Can't place a negative bet" << endl;

    if (playerBet > money)
      cout << "Can't bet more than you have" << endl;

    cout << "Place a new bet. $";
    cin >> playerBet;
  }
}

void DealNewHand(vector<Card> &playerCards, vector<Card> &dealerCards)
{
  // Clear hands
  playerCards.clear();
  dealerCards.clear();

  // Deal Cards
  GetCard(playerCards);
  GetCard(playerCards);
  GetCard(dealerCards);

  ShowHands(playerCards, dealerCards);
}

void PlayerActions(vector<Card> &playerCards, GameState &state, double &money, double playerBet)
{
  // Output action options
  string availableActions = "[H]it\n[S]tand\n";
  if (money >= playerBet)
    availableActions += "[D]ouble down\n";
  cout << availableActions;

  // Store action input
  char action;
  cin >> action;
  action = toupper(action);

  // Validate input
  while (action != 'H' && action != 'S' && action != 'D')
  {
    cout << availableActions;
    cin >> action;
    action = toupper(action);
  }

  // Perform action
  switch (action)
  {
  case 'H': // Hit
    GetCard(playerCards);
    break;
  case 'S': // Stand
    state = GameState::Dealer;
    break;
  case 'D': // Double Down
    GetCard(playerCards);
    money -= playerBet;
    playerBet += playerBet;
    state = GameState::Dealer;
    break;
  }
}

void ShowHands(vector<Card> playerCards, vector<Card> dealerCards)
{
  cout << endl;
  dealerCards.size() > 1 ? cout << "Dealer cards: " : cout << "Dealer card: ";
  PrintCards(dealerCards);

  cout << "Your cards: ";
  PrintCards(playerCards);
}

void DetermineResult(int playerTotal, int dealerTotal, double &money, double playerBet)
{
  if (playerTotal > 21)
  {
    cout << "Bust" << endl;
    money -= playerBet;
  }
  else if (playerTotal == dealerTotal)
    cout << "Push" << endl;
  else if (playerTotal == 21)
  {
    cout << "BLACKJACK" << endl;
    money += playerBet;
  }
  else if (playerTotal > dealerTotal)
  {
    cout << "You win!" << endl;
    money += playerBet;
  }
  else
  {
    cout << "You lose" << endl;
    money -= playerBet;
  }
}

void PlayAgain(GameState &state)
{
  char again;
  cout << "Play again? [Y]es or [N]o" << endl;
  cin >> again;
  again = toupper(again);

  while (again != 'Y' && again != 'N')
  {
    cout << "Invalid entry." << endl;
    cout << "Play again? [Y]es or [N]o" << endl;
    cin >> again;
  }

  if (again == 'Y')
    state = GameState::Betting;
  else if (again == 'N')
    state = GameState::End;
}
