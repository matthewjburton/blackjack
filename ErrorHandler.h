#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <iostream>
#include <string>

class ErrorHandler
{
public:
    enum class ErrorType
    {
        InvalidBet,
        NegativeBet,
        InsufficientFunds,
        InvalidResponse,
        UnknownError
    };

    static void HandleError(ErrorType errorType, const std::string &message = "");
};

#endif // ERRORHANDLER_H
