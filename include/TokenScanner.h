//
// Created by Yuki on 2023/12/17.
//

#ifndef BOOKSTORE_TOKENSCANNER_H
#define BOOKSTORE_TOKENSCANNER_H
#include "error.h"
#include <string>
#include <vector>
#include <cmath>

// 划分指令并且进行指令正确性检查(processLine)
class TokenScanner{
public:
    static void SplitString (std::string str, std::vector<std::string> &all_string);

    ///注意手动释放内存
    // 检查成功后再转变
    // 感觉不需要了？
    /*static char* StringToChar(std::string str) {
        char* theChar = new char[str.length() + 1];
        strcpy(theChar, str.c_str());
        return theChar;
    }*/

    // 检查并且返回Quantity, Count
    static int StringToInteger(std::string str);

    // 检查userid, password, currentPassword, newPassword
    static void customer1(std::string str);

    // 检查username
    static void customer2(std::string str);

    // 检查privilege
    static void customer3(std::string str);

    // 检查ISBN
    static void book1(std::string str);

    // 检查bookName, Author, keyWord(all)
    static void book2(std::string str);

    // 检查一个keyWord
    static void book3(std::string str);

    // 检查price, totalCost
    static double book4(std::string str);

    // 剪切show指令
    static std::string cut(std::string str);

    // keyWord不能包含多个关键词
    static void more_key(std::string str);

    static std::string cut_price(std::string str) ;

};

#endif //BOOKSTORE_TOKENSCANNER_H
