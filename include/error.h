//
// Created by 25825 on 2023/12/16.
//

#ifndef BOOKSTORE_ERROR_H
#define BOOKSTORE_ERROR_H
#include <exception>
#include <string>

class InvalidExp : public std::exception{
public:
    const char* what() const noexcept override;
};

#endif //BOOKSTORE_ERROR_H
