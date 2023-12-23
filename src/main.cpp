#include <iostream>
#include "../include/Book.h"
#include "../include/User.h"
#include "../include/TokenScanner.h"
#include "../include/Finance.h"
#include "../include/Diary.h"

bool processLine(std::string str, UserAll &user_all, Book &book, Finance &finance, Diary &log);

int main() {
    //freopen("input", "r", stdin);
    //freopen("output", "w", stdout);

    UserAll user_all;
    Book book;
    Finance fin;
    Diary Log;
    while (true) {
        std::string line;
        //getline(std::cin, line);
        try {
            if (!getline(std::cin, line)) {
                line = "quit";
            }
            if (!processLine(line, user_all, book, fin, Log)) break;//return 0;
        } catch (InvalidExp &err) {
            std::cout<<err.what();
        }
    }
    return 0;
}

bool processLine(std::string str, UserAll &user_all, Book &book, Finance &finance, Diary &log) {
    std::vector<std::string> parse;
    TokenScanner::SplitString(str, parse);
    // 如果没有语句，继续进行下一行
    if (parse.empty()) return true;
    std::string command = parse[0];
    if (command == "quit" || command == "exit") {
        user_all.LogUsers.clear();
        user_all.log_map.clear();
        return false;
    }
    if (command == "su") {
        if (parse.size() == 1 || parse.size() > 3) throw InvalidExp();
        TokenScanner::customer1(parse[1]);
        //const char *userid = parse[1].c_str();
        char userid[40] = {'\0'};
        strcpy(userid, parse[1].c_str());
        if (parse.size() == 3) {
            TokenScanner::customer1(parse[2]);
            //const char *password = parse[2].c_str();
            char password[40] = {'\0'};
            strcpy(password, parse[2].c_str());
            user_all.su(userid, password, log);
        } else if (parse.size() == 2) {
            user_all.su(userid, nullptr, log);
        }
    } else if (command == "logout") {
        if (parse.size() != 1) throw InvalidExp();
        user_all.logout(log, book);
    } else if (command == "register") {
        if (parse.size() != 4) throw InvalidExp();
        TokenScanner::customer1(parse[1]);
        TokenScanner::customer1(parse[2]);
        TokenScanner::customer2(parse[3]);
        //const char *userid = parse[1].c_str();
        //const char *passwd = parse[2].c_str();
        //const char *username = parse[3].c_str();
        char userid[40] = {'\0'};
        char passwd[40] = {'\0'};
        char username[40] = {'\0'};
        strcpy(userid, parse[1].c_str());
        strcpy(passwd, parse[2].c_str());
        strcpy(username, parse[3].c_str());
        user_all.Register(userid, passwd, username, log);
    } else if (command == "passwd") {
        if (parse.size() < 3 || parse.size() > 4) throw InvalidExp();
        TokenScanner::customer1(parse[1]);
        //const char *userid = parse[1].c_str();
        char userid[40] = {'\0'};
        strcpy(userid, parse[1].c_str());
        if (parse.size() == 4) {
            TokenScanner::customer1(parse[3]);
            //const char *newPasswd = parse[3].c_str();
            char newPasswd[40] = {'\0'};
            strcpy(newPasswd, parse[3].c_str());
            TokenScanner::customer1(parse[2]);
            //const char *currentPasswd = parse[2].c_str();
            char currentPasswd[40] = {'\0'};
            strcpy(currentPasswd, parse[2].c_str());
            user_all.passwd(userid, currentPasswd, newPasswd, command, log);
        } else {
            TokenScanner::customer1(parse[2]);
            //const char *newPasswd = parse[2].c_str();
            char newPasswd[40] = {'\0'};
            strcpy(newPasswd, parse[2].c_str());
            user_all.passwd(userid, nullptr, newPasswd, command, log);
        }
    } else if (command == "useradd") {
        if (parse.size() != 5) throw InvalidExp();
        TokenScanner::customer1(parse[1]);
        TokenScanner::customer1(parse[2]);
        TokenScanner::customer2(parse[4]);
        TokenScanner::customer3(parse[3]);
        //const char *userid = parse[1].c_str();
        //const char *passwd = parse[2].c_str();
        //const char *username = parse[4].c_str();
        char userid[40] = {'\0'};
        strcpy(userid, parse[1].c_str());
        char passwd[40] = {'\0'};
        strcpy(passwd, parse[2].c_str());
        char username[40] = {'\0'};
        strcpy(username, parse[4].c_str());
        int privilege = std::stoi(parse[3]);
        user_all.useradd(userid, passwd, privilege, username,command, log);
    } else if (command == "delete") {
        if (parse.size() != 2) throw InvalidExp();
        TokenScanner::customer1(parse[1]);
        //const char *userid = parse[1].c_str();
        char userid[40] = {'\0'};
        strcpy(userid, parse[1].c_str());
        user_all.Delete(userid, command, log);
    } else if (command == "select") {
        if (parse.size() != 2) throw InvalidExp();
        TokenScanner::book1(parse[1]);
        //const char *isbn_ = parse[1].c_str();
        char isbn_[30] = {'\0'};
        strcpy(isbn_, parse[1].c_str());
        book.select(isbn_, user_all);
    } else if (command == "buy") {
        if (parse.size() != 3) throw InvalidExp();
        TokenScanner::book1(parse[1]);
        TokenScanner::StringToInteger(parse[2]);
        //const char *isbn_ = parse[1].c_str();
        char isbn_[30] = {'\0'};
        strcpy(isbn_, parse[1].c_str());
        int quantity = TokenScanner::StringToInteger(parse[2]);
        book.buy(isbn_, quantity, user_all, finance);
    } else if (command == "import") {
        if (parse.size() != 3) throw InvalidExp();
        int quantity = TokenScanner::StringToInteger(parse[1]);
        double totalCost = TokenScanner::book4(parse[2]);
        book.Import(quantity, totalCost, user_all, finance);
    } else if (command == "show" ) {
        if (parse.size() > 2 && parse[1] != "finance") throw InvalidExp();
        if (parse.size() == 1) {
            book.show_all(user_all);
        } else {
            if (parse[1] != "finance") {
                std::string tmp = TokenScanner::cut(parse[1]);
                // if (tmp.empty()) throw InvalidExp();
                if (parse[1][1] == 'I') {
                    // ISBN
                    TokenScanner::book1(tmp);
                    //const char *isbn_ = tmp.c_str();
                    char isbn_[30] = {'\0'};
                    strcpy(isbn_, tmp.c_str());
                    book.show(isbn_, nullptr, nullptr, nullptr, user_all);
                }
                if (parse[1][1] == 'n') {
                    // name
                    TokenScanner::book2(tmp);
                    //const char *name = tmp.c_str();
                    char name[70] = {'\0'};
                    strcpy(name, tmp.c_str());
                    book.show(nullptr, name, nullptr, nullptr, user_all);
                }
                if (parse[1][1] == 'a') {
                    // author
                    TokenScanner::book2(tmp);
                    //const char *author = tmp.c_str();
                    char author[70] = {'\0'};
                    strcpy(author, tmp.c_str());
                    book.show(nullptr, nullptr, author, nullptr, user_all);
                }
                if (parse[1][1] == 'k') {
                    // keyWords
                    TokenScanner::more_key(tmp);
                    TokenScanner::book3(tmp);
                    //const char *key = tmp.c_str();
                    char key[70] = {'\0'};
                    strcpy(key, tmp.c_str());
                    book.show(nullptr, nullptr, nullptr, key, user_all);
                }
            } else {
                if (parse.size() > 3) throw InvalidExp();
                if (parse.size() == 3) {
                    int count = TokenScanner::StringToInteger(parse[2]);
                    finance.show_finance(count, user_all);
                } else {
                    int total = finance.get_total();
                    finance.show_finance(total, user_all);
                }
            }
        }
    } else if (command == "modify") {
        if (parse.size() == 1 || parse.size() > 5) throw InvalidExp();
        bool I_flag = false;
        bool n_flag = false;
        bool a_flag = false;
        bool k_flag = false;
        bool p_flag = false;
        //const char *copy = nullptr;
        //const char *isbn_ = nullptr;
        char isbn_[25] = {'\0'};
        //const char *name = nullptr;
        char name[66] = {'\0'};
        //const char *author = nullptr;
        char author[66] = {'\0'};
        std::string keyWord;
        char keyword[66] = {'\0'};
        double price = -1;
        for (int i = 1; i < parse.size(); i++) {
            if (parse[i][1] == 'I' && !I_flag) {
                I_flag = true;
                std::string tmp = TokenScanner::cut(parse[i]);
                TokenScanner::book1(tmp);
                strcpy(isbn_, tmp.c_str());
            } else if (parse[i][1] == 'n' && !n_flag) {
                n_flag = true;
                std::string tmp = TokenScanner::cut(parse[i]);
                TokenScanner::book2(tmp);
                //copy = tmp.c_str();
                strcpy(name, tmp.c_str());
            }else if (parse[i][1] == 'a' && !a_flag) {
                a_flag = true;
                std::string tmp = TokenScanner::cut(parse[i]);
                TokenScanner::book2(tmp);
                //copy = tmp.c_str();
                strcpy(author, tmp.c_str());
            } else if (parse[i][1] == 'k' && !k_flag) {
                k_flag = true;
                keyWord = TokenScanner::cut(parse[i]);
                TokenScanner::book2(keyWord);
                strcpy(keyword, keyWord.c_str());
            } else if (parse[i][1] == 'p' && !p_flag) {
                p_flag = true;
                std::string tmp = TokenScanner::cut_price(parse[i]);
                price = TokenScanner::book4(tmp);
            } else throw InvalidExp();
        }
        book.modify(isbn_, name, author, keyword, price, user_all, I_flag, n_flag, a_flag, k_flag, p_flag);
    } else if (command == "report") {
        if (parse.size() != 2) throw InvalidExp();
        if (parse[1] == "finance") {
            finance.report_finance(user_all);
        } else if (parse[1] == "employee") {

        } else throw InvalidExp();
    } else if (command == "log") {
        if (parse.size() != 1) throw InvalidExp();
        log.read_all();
    } else throw InvalidExp();
    return true;
}
