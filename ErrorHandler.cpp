#include "ErrorHandler.h"
#include <iostream>
#include <string>

void ErrorHandler::HandleError(ErrorType errorType, const std::string &message)
{
    switch (errorType)
    {
    case ErrorType::InvalidBet:
        std::cerr << "Error: Invalid bet amount";
        break;
    case ErrorType::NegativeBet:
        std::cerr << "Error: Bet amount cannot be negative";
        break;
    case ErrorType::InsufficientFunds:
        std::cerr << "Error: Insufficient funds";
        break;
    case ErrorType::InvalidResponse:
        std::cerr << "Error: Invalid response";
        break;
    case ErrorType::UnknownError:
    default:
        std::cerr << "Error: Unknown error";
        break;
    }
    if (!message.empty())
    {
        std::cerr << " - " << message;
    }
    std::cerr << std::endl;
}
