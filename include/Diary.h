//
// Created by 25825 on 2023/12/21.
//

#ifndef BOOKSTOREFINAL_DIARY_H
#define BOOKSTOREFINAL_DIARY_H
#include "Block.h"
class Record{
    friend class Diary;
private:
    char userid[66] = {'\0'};
    std::string operation;
    int permission = 0;
public:
    Record() = default;
    explicit Record(const char *userid_, std::string str, int per) :permission(per){
        strcpy(userid, userid_);
        operation = std::move(str);
    }

};

// 写在末尾
class Diary{
private:
    MemoryRiver<Record, 1> diary;
    //int total = 0;
public:
    Diary();

    void write_diary(Record &tmp);

    void read_employee();

    void read_all();
};


#endif //BOOKSTOREFINAL_DIARY_H
