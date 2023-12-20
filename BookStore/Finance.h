//
// Created by 25825 on 2023/12/18.
//

#ifndef BOOKSTOREACM_FINANCE_H
#define BOOKSTOREACM_FINANCE_H
#include <vector>
#include "Block.h"
#include "error.h"
#include "User.h"

class UserAll;

class fin_info {
    friend class Finance;
    friend class Book;
private:
    double number;
    bool status; // true 表示收入， false表示支出
    char userid[66] = {'\0'};;
public:
    fin_info() : number(0.0), status(true){}
    explicit fin_info(double num, bool sta, const char *userid_) : number(num), status(sta) {
        strcpy(userid, userid_);
    }
};

class Finance {
    friend class Book;

private:
    // 不需要块状链表, 但应该存储到文件之中
    MemoryRiver<fin_info, 1> fin_report; // 开头记录交易总数
    //int total = 0;
public:
    Finance();
    ~Finance();
    int get_total();

    void read();

    void show_finance(int count);

    void report_finance(UserAll &user_all);

};


#endif //BOOKSTOREACM_FINANCE_H
