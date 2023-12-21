//
// Created by 25825 on 2023/12/18.
//
#include "../include/TokenScanner.h"
void TokenScanner::SplitString (std::string str, std::vector<std::string> &all_string) {
    std::string tmp;
    for (int i = 0; i <= str.length(); i++) {
        if (str[i] == ' ' || i == str.length()) {
            if (!tmp.empty()) all_string.push_back(tmp);
            tmp = "";
            continue;
        }
        tmp += str[i];
    }
}

///注意手动释放内存
// 检查成功后再转变
// 感觉不需要了？
/*static char* StringToChar(std::string str) {
    char* theChar = new char[str.length() + 1];
    strcpy(theChar, str.c_str());
    return theChar;
}*/

// 检查并且返回Quantity, Count
int TokenScanner::StringToInteger(std::string str) {
    if (str.length() > 10) throw InvalidExp();
    for (int i = 0; i < str.length(); i++) {
        if (str[i] < '0' || str[i] > '9') throw InvalidExp();
    }
    long long num = std::stoll(str);
    if (num > 2147483647) throw InvalidExp();
    if (num < 0) throw InvalidExp();
    int ans = std::stoi(str);
    return ans;
}

// 检查userid, password, currentPassword, newPassword
void TokenScanner::customer1(std::string str) {
    if (str.size() > 30) {
        throw InvalidExp();
    } else {
        for (int i = 0; i < str.size(); i++) {
            if (str[i] < '0' || str[i] > '9') {
                if (str[i] < 'a' || str[i] > 'z') {
                    if (str[i] < 'A' ||str[i] > 'Z') {
                        if (str[i] != '_') {
                            throw InvalidExp();
                        }
                    }
                }
            }
        }
    }
}

// 检查username
void TokenScanner::customer2(std::string str) {
    if (str.size() > 30)  throw InvalidExp();
}

// 检查privilege
void TokenScanner::customer3(std::string str) {
    if (str.length() > 1) throw InvalidExp();
    int x = std::stoi(str);
    if (x != 1 && x != 3 && x != 7) throw InvalidExp();
}

// 检查ISBN
void TokenScanner::book1(std::string str) {
    if (str.size() > 20) throw InvalidExp();
}

// 检查bookName, Author, keyWord(all)
void TokenScanner::book2(std::string str) {
    if (str.size() > 60) throw InvalidExp();
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '"') throw InvalidExp();
    }
}

// 检查一个keyWord
void TokenScanner::book3(std::string str) {
    if (str.empty() || str.length() > 60) throw InvalidExp();
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '"') throw InvalidExp();
    }
}

// 检查price, totalCost
double TokenScanner::book4(std::string str) {
    if (str.length() > 13) throw InvalidExp();
    bool flag = false;
    for (int i = 0; i < str.size() - 1; i++) {
        if (str[i] == '.') {
            if (!flag) {
                flag = true;
                if (i != str.size() - 3) throw InvalidExp();
            } else throw InvalidExp();
        }
    }
    double x = std::stod(str);
    return x;
}

// 剪切show指令
std::string TokenScanner::cut(std::string str) {
    std::string ans;
    if (str[1] == 'I') {
        for (int i = 6; i < str.length(); i++) {
            ans += str[i];
        }
    } else if (str[1] == 'n') {
        for (int i = 7; i < str.length() - 1; i++) {
            ans += str[i];
        }
    } else if (str[1] == 'a') {
        for (int i = 9; i < str.length() - 1; i++) {
            ans += str[i];
        }
    } else if (str[1] == 'k') {
        for (int i = 10; i < str.length() - 1; i++) {
            ans += str[i];
        }
    } else throw InvalidExp();

    if (ans.empty()) throw InvalidExp();
    return ans;
}

// keyWord不能包含多个关键词
void TokenScanner::more_key(std::string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '|') throw InvalidExp();
    }
}

std::string TokenScanner::cut_price(std::string str) {
    std::string ans;
    for (int i = 7; i < str.length(); i++) {
        ans += str[i];
    }
    if (ans.empty()) throw InvalidExp();
    return ans;
}

