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

void Diary::read_employee(UserAll &user_all) {
    if (user_all.LogUsers.empty()) throw InvalidExp();
    if (user_all.LogUsers.back().permission != 7) throw InvalidExp();
    int total;
    diary.get_info(total, 1);
    int num = 0;
    num = total / 2000 + 1;
    std::cout<<"--Here is your employee's report--"<<'\n';
    for (int i = 0; i < num; i++) {
        Record all[2000];
        diary.read(all[0], 4 + i * 2000 * sizeof(Record), 2000);
        int end;
        if (i != num - 1) end = 2000;
        else end = total % 2000;
        for (int j = 0; j < end; j++) {
            if (all[j].permission == 3) {
                if ((all[j].cost == -1.0 && !all[j].status) || (all[j].cost != -1.0)) {
                    // 是buy 或者 import
                    if (!all[j].status) {
                        std::cout << all[j].userid << '\t' << all[j].operation << "  ---------cost: " << '\t' << std::fixed
                                  << std::setprecision(2) << all[j].cost << '\n';
                    } else {
                        std::cout << all[j].userid << '\t' << all[j].operation << "  ---------profit:" << '\t'
                                  << std::fixed << std::setprecision(2) << all[j].cost << '\n';
                    }
                } else {
                    std::cout << all[j].userid << '\t' << all[j].operation << '\n';
                }
            }
        }
    }
}

//
void Diary::read_all(UserAll &user_all) {
    if (user_all.LogUsers.empty()) throw InvalidExp();
    if (user_all.LogUsers.back().permission != 7) throw InvalidExp();
    int total;
    diary.get_info(total, 1);
    int num = 0;
    num = total / 2000 + 1;
    std::cout<<"--Here is your log--"<<'\n';
    std::cout<<"** Total Operation **"<<'\t'<<total<<'\n';
    for (int i = 0; i < num; i++) {
        Record all[2000];
        diary.read(all[0], 4 + i * 2000 * sizeof(Record), 2000);
        int end;
        if (i != num - 1) end = 2000;
        else end = total % 2000;
        for (int j = 0; j < end; j++) {
           if ((all[j].cost == -1.0 && !all[j].status) ||(all[j].cost != -1.0)) {
                // 是buy 或者 import
                if (!all[j].status) {
                    std::cout<<all[j].userid<<'\t'<<all[j].operation;
                    std::cout<<"  ---------cost: "<<'\t'<<std::fixed<<std::setprecision(2)<<all[j].cost<<'\n';
                } else {
                    std::cout<<all[j].userid<<'\t'<<all[j].operation;
                    std::cout<<"  ---------profit:"<<'\t'<<std::fixed<<std::setprecision(2)<<all[j].cost<<'\n';
                }
            } else {
                std::cout<<all[j].userid<<'\t'<<all[j].operation<<'\n';
            }
        }
    }
}
