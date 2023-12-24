//
// Created by 25825 on 2023/12/21.
//

#ifndef BOOKSTOREFINAL_DIARY_H
#define BOOKSTOREFINAL_DIARY_H
#include "Block.h"
#include <iomanip>
#include "User.h"

class UserAll;
class Record{
    friend class Diary;
private:
    char userid[66] = {'\0'};
    char operation[1000] = {'\0'};
    int permission = 0;
    double cost = -1.0;
    bool status = true;
public:
    Record() = default;
    explicit Record(const char *userid_, const char *op, int per) :permission(per){
        strcpy(userid, userid_);
        strcpy(operation, op);
    }
    explicit Record(const char* userid_, const char *op, int per, double cost_, bool sta) :permission(per), cost(cost_), status(sta){
        strcpy(userid, userid_);
        strcpy(operation, op);
    }

};

// 写在末尾
class Diary{
private:
    MemoryRiver<Record, 1> diary;
public:
    Diary();

    void write_diary(Record &tmp);

    void read_employee(UserAll &user_all);

    void read_all(UserAll &user_all);
};


#endif //BOOKSTOREFINAL_DIARY_H
