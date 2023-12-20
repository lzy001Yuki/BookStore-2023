//
// Created by 25825 on 2023/12/18.
//
#include "../include/error.h"

const char* InvalidExp::what() const noexcept {
    return "Invalid\n";
}
