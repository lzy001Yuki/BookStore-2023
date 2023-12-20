//
// Created by 25825 on 2023/12/18.
//
#include "../include/Finance.h"
Finance::Finance() {
    fin_report.initialise("finance.txt");
    fin_report.clear();
}

Finance::~Finance() {
    fin_report.clear();
}

int Finance::get_total() {
    int total_count;
    fin_report.get_info(total_count, 1);
    return total_count;
}

void Finance::show_finance(int count) {
    int total_count = get_total();
    if (count > total_count) throw InvalidExp();
    if (count == 0) {
        std::cout<<'\n';
        return;
    }
    double in = 0;
    double out = 0;
    auto *find = new fin_info[count];
    fin_report.read(find[0], 4 + sizeof(fin_info) * (total_count - count), count);
    for (int i = 0; i < count; i++) {
        if (find[i].status) in += find[i].number;
        else out += find[i].number;
    }
    std::cout<<"+ ";
    std::cout<<std::fixed<<std::setprecision(2)<<in<<' ';
    std::cout<<"- ";
    std::cout<<std::fixed<<std::setprecision(2)<<out<<'\n';
    delete [] find;
}

void Finance::report_finance(UserAll &user_all) {
    if (user_all.LogUsers.back().permission < 7) throw InvalidExp();
    std::cout<<"-FINANCE REPORT-"<<'\n';
    int total = get_total();
    auto *all = new fin_info[total];
    fin_report.read(all[0], 4, total);
    for (int i = 0; i < total; i++) {
        if (all[i].status) {
            std::cout<<"INCOME:"<<'\t'<<std::fixed<<std::setprecision(2)<<all[i].number<<'\t';
            std::cout<<"by user(ID):"<<'\t'<<all[i].userid;
        } else {
            std::cout<<"EXPENSE:"<<'\t'<<std::fixed<<std::setprecision(2)<<all[i].number<<'\t';
            std::cout<<"by user(ID):"<<'\t'<<all[i].userid;
        }
    }
    std::cout<<"-THE END-"<<'\n';
}

void Finance::read() {
    int total;
    fin_report.get_info(total, 1);
    total++;
    fin_report.write_info(total, 1);
    std::cout<<total;
}
