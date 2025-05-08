#ifndef CHESS_EMPTYQUEUEEXCEPTION_H
#define CHESS_EMPTYQUEUEEXCEPTION_H
#include <iostream>
#include <exception>

class EmptyQueueException : public std::exception {
private:
    std::string message;
public:
    EmptyQueueException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif //CHESS_EMPTYQUEUEEXCEPTION_H
