#ifndef CHESS_INVALIDPIECEEXCEPTION_H
#define CHESS_INVALIDPIECEEXCEPTION_H

#include <iostream>
#include <exception>

class InvalidPieceException : public std::exception {
private:
    std::string message;
public:
    InvalidPieceException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif //CHESS_INVALIDPIECEEXCEPTION_H
