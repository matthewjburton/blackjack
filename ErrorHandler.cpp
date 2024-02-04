#include "ErrorHandler.h"
#include <iostream>
#include <string>

// Define color codes
#define RESET "\033[0m"
#define RED "\033[31m"

void ErrorHandler::HandleError(ErrorType errorType, const std::string &message)
{
    switch (errorType)
    {
    case ErrorType::InvalidBet:
        std::cerr << RED << "Error: Invalid bet amount";
        break;
    case ErrorType::NegativeBet:
        std::cerr << RED << "Error: Bet amount cannot be negative";
        break;
    case ErrorType::InsufficientFunds:
        std::cerr << RED << "Error: Insufficient funds";
        break;
    case ErrorType::InvalidResponse:
        std::cerr << RED << "Error: Invalid response";
        break;
    case ErrorType::UnknownError:
    default:
        std::cerr << RED << "Error: Unknown error";
        break;
    }
    if (!message.empty())
    {
        std::cerr << " - " << message;
    }
    std::cerr << RESET << std::endl;
}
