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
void PlayerActions(vector<Card> &playerCards, vector<Card> dealerCards, GameState &state, double &money, double playerBet, double insuranceBet);
void PlaceInsuranceBet(double &insuranceBet, double &money, double playerBet);
void ShowHands(vector<Card> playerCards, vector<Card> dealerCards);
void DetermineResult(int playerTotal, int dealerTotal, double &money, double playerBet, double insuranceBet);
void PlayAgain(GameState &state, double money);

int main()
{
  GameState state = GameState::Betting;
  double playerBet = 0, insuranceBet = 0;
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
      // Check for blackjack
      playerTotal = CalculateTotal(playerCards);
      if (playerTotal == 21)
        state = GameState::Dealer;
      else
        state = GameState::Player;
      break;

    case GameState::Player:
      // Player decision
      PlayerActions(playerCards, dealerCards, state, money, playerBet, insuranceBet);
      ShowHands(playerCards, dealerCards);

      // Check for blackjack or bust
      playerTotal = CalculateTotal(playerCards);
      if (playerTotal == 21)
        state = GameState::Dealer;

      if (playerTotal > 21)
        state = GameState::Result;
      break;

    case GameState::Dealer:
      // Draw second card
      GetCard(dealerCards);

      // Draw until over 16
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
      DetermineResult(playerTotal, dealerTotal, money, playerBet, insuranceBet);
      PlayAgain(state, money);
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

  // Option to place the same bet as the previous hand
  if (playerBet != 0 && money >= playerBet)
  {
    cout << "Place same bet? [Y]es or [N]o" << endl;
    char response;
    cin >> response;
    response = toupper(response);

    while (response != 'Y' && response != 'N')
    {
      cout << "Invalid response." << endl;
      cout << "Place same bet? [Y]es or [N]o" << endl;
      char response;
      cin >> response;
      response = toupper(response);
    }
    if (response == 'Y')
      return;
  }

  // Place a new bet
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

void PlayerActions(vector<Card> &playerCards, vector<Card> dealerCards, GameState &state, double &money, double playerBet, double insuranceBet)
{
  // Output action options
  string availableActions = "[H]it\n[S]tand\n";
  if (money - playerBet >= playerBet)
    availableActions += "[D]ouble down\n";
  if (dealerCards[0].getValue() == 11 && insuranceBet == 0)
    availableActions += "[I]nsurance\n";
  cout << availableActions;

  // Store action input
  char action;
  cin >> action;
  action = toupper(action);

  // Validate input
  while (action != 'H' && action != 'S' && action != 'D' && action != 'I')
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
  case 'I': // Insurance
    PlaceInsuranceBet(insuranceBet, money, playerBet);
  }
}

void PlaceInsuranceBet(double &insuranceBet, double &money, double playerBet)
{
  cout << "Money available: $" << money << endl;
  cout << "Place your insurance bet. $";
  cin >> insuranceBet;

  while (insuranceBet <= 0 || insuranceBet > money || insuranceBet > (playerBet / 2.0))
  {
    if (insuranceBet == 0)
      cout << "Can't bet nothing" << endl;

    if (insuranceBet < 0)
      cout << "Can't place a negative bet" << endl;

    if (insuranceBet > money)
      cout << "Can't bet more than you have" << endl;

    if (insuranceBet > (playerBet / 2.0))
      cout << "Can't bet more than half of your current bet" << endl;

    cout << "Place a new insurance bet. $";
    cin >> insuranceBet;
  }
}

void ShowHands(vector<Card> playerCards, vector<Card> dealerCards)
{
  cout << endl;
  cout << "Dealer card" << ((dealerCards.size() > 1) ? "s" : "") << " (" << CalculateTotal(dealerCards) << ") : ";
  PrintCards(dealerCards);

  cout << "Your cards (" << CalculateTotal(playerCards) << "): ";
  PrintCards(playerCards);
  cout << endl;
}

void DetermineResult(int playerTotal, int dealerTotal, double &money, double playerBet, double insuranceBet)
{
  if (playerTotal > 21)
  {
    cout << "Bust" << endl;
    money -= playerBet;
  }
  else if (dealerTotal == 21 && insuranceBet > 0) // Check for dealer blackjack if insurance bet was made
  {
    cout << "Dealer has blackjack. Insurance pays 2:1." << endl;
    money += insuranceBet * 2; // Insurance bet pays 2:1
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

void PlayAgain(GameState &state, double money)
{
  if (money == 0)
  {
    cout << "You've run out of money. Thanks for playing!" << endl;
    state = GameState::End;
    return;
  }

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
