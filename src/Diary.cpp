//
// Created by 25825 on 2023/12/21.
//

#include "Diary.h"

Diary::Diary() {
    diary.initialise("diary.txt");
    diary.clear();
}

void Diary::write_diary(Record &tmp) {
    //diary.write_end(tmp);
    int total;
    diary.get_info(total, 1);
    total++;
    diary.write_info(total, 1);
    diary.write(tmp, 4 + (total - 1) * sizeof(Record), 1);
}

void Diary::read_employee() {

}

void Diary::read_all() {
    int total;
    diary.get_info(total, 1);
    //auto *all = new Record[total];
    //Record all[10000];
    //diary.read(all[0], 4, total);
    std::cout<<"-- Here is your log --"<<'\n';
    /*for (int i = 0; i < total; i++) {
        std::cout<<"User(ID):"<<'\t'<<all[i].userid<<'\t'<<all[i].operation<<'\n';
    }*/
    for (int i = 0; i < total; i++) {
        Record tmp;
        diary.read(tmp, 4 + i * sizeof(Record) , 1);
        std::cout<<"User(ID):"<<'\t'<<tmp.userid<<'\t'<<tmp.operation<<'\n';
    }
    //delete [] all;
}
