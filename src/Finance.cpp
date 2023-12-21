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
    if (user_all.LogUsers.empty()) throw InvalidExp();
    if (user_all.LogUsers.back().permission < 7) throw InvalidExp();
    std::cout<<"-FINANCE REPORT-"<<'\n';
    int total = get_total();
    std::cout<<"** TOTAL NUMBER OF TRANSACTIONS :"<<total<<"**"<<'\n';
    std::cout<<"-- Income and Expense --"<<'\n';
    auto *all = new fin_info[total];
    fin_report.read(all[0], 4, total);
    double income = 0.0;
    double expense = 0.0;
    int in = 0, out = 0;
    for (int i = 0; i < total; i++) {
        if (all[i].status) {
            //in++;
            //std::cout<<in<<'\t';
            std::cout<<"INCOME: "<<'\t'<<std::fixed<<std::setprecision(2)<<all[i].number<<'\t';
            std::cout<<"by user(ID):"<<'\t'<<all[i].userid<<'\n';
            income += all[i].number;
        } else {
            //out++;
            //std::cout<<out<<'\t';
            std::cout<<"EXPENSE:"<<'\t'<<std::fixed<<std::setprecision(2)<<all[i].number<<'\t';
            std::cout<<"by user(ID):"<<'\t'<<all[i].userid<<'\n';
            expense += all[i].number;
        }
    }
    std::cout<<"** ALL_INCOME **  "<<std::fixed<<std::setprecision(2)<<income<<'\n';
    std::cout<<"** ALL_EXPENSE **  "<<std::fixed<<std::setprecision(2)<<expense<<'\n';
    double profit = income - expense;
    if (profit >= 0) std::cout<<"PROFIT: "<<profit<<'\n';
    else std::cout<<"LOSS: "<<profit<<'\n';

    /// 复杂度有点高，可以考虑进行优化
    std::cout<<"-- Import Information --"<<'\n';
    for (int i = 0; i < total; i++) {
        if (!all[i].status) {
            std::cout<<"Import: "<<all[i].selected_isbn<<'\t'<<"Quantity: "<<std::setw(10)<<all[i].sum<<'\t';
            std::cout<<"Cost:  "<<std::fixed<<std::setprecision(2)<<std::setw(13)<<all[i].cost<<'\t'<<"Total Expense: "<<all[i].number<<'\n';
        }
    }
    std::cout<<"-- Income Information --"<<'\n';
    for (int i = 0; i < total; i++) {
        if (all[i].status) {
            std::cout<<"Income: "<<all[i].selected_isbn<<'\t'<<"Quantity: "<<std::setw(10)<<all[i].sum<<'\t';
            std::cout<<"Price: "<<std::fixed<<std::setprecision(2)<<std::setw(13)<<all[i].price<<'\t'<<"Total Income:  "<<all[i].number<<'\n';
        }
    }
    std::cout<<"-THE END-"<<'\n';
    delete [] all;
}

void Finance::read() {
    int total;
    fin_report.get_info(total, 1);
    total++;
    fin_report.write_info(total, 1);
    std::cout<<total;
}
