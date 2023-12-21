//
// Created by Yuki on 2023/12/16.
//

#ifndef BOOKSTORE_USER_H
#define BOOKSTORE_USER_H
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include "Block.h"
#include "error.h"
#include "Book.h"
#include "Diary.h"
const int MAX_LEN = 40; // 最大合法字符集为30

//Invalid error;
// 路径依赖！
class Book;
class BookInfo;
// 储存用户的基本信息
class User {
    friend class UserAll;
    friend class Block<User>;
    friend class Book;
    friend class BookInfo;
    friend class Finance;
private:
    char UserID[MAX_LEN] = {'\0'};
    char Username[MAX_LEN] = {'\0'};
    char Password[MAX_LEN] = {'\0'};
    int permission = 0;// 游客状态
    int index_num = 0;
    int size = 0;
    char index[MAX_LEN] = {'\0'};
    int next = 0;
    bool log_status = false; // 未登录状态
    char select_isbn[66] = {'\0'};
    bool select_one = false; // false时不管select_isbn是什么，只有在true时考虑
    //BookInfo select_info;
public:
    User() = default;
    User(const char* UserID_, const char *Password_, const char* Username_, int permission_) : permission(permission_) , size(0), index_num(0), next(0) {
        strcpy(UserID, UserID_);
        strcpy(Username, Username_);
        strcpy(Password, Password_);
        strcpy(index, UserID_);
    }
    explicit User(const char* index_);

    bool operator == (const User &obj) const;

    bool operator < (const User &obj) const;

    bool operator > (const User &obj) const;

    bool operator != (const User &obj) const;
};

class UserAll {
    friend class Book;
    friend class Finance;
private:
    std::vector<User> LogUsers;// 登录栈,现在登录的用户是LogUsers.back()
    int current_permission = 0;// 现在登录的用户权限
    Block<User> users;

public:
    UserAll();
    ~UserAll();

    void su(const char *UserID, const char *Password, Diary &diary);

    void Register(const char *UserId, const char *Password, const char *Username);

    void Delete(const char *UserID);

    void useradd(const char *UserID, const char *Password, int privilege, const char *Username);

    void passwd(const char *UserID, const char *CurrentPassword, const char *NewPassword);

    void logout(Diary &diary);

    //void select(const char *isbn, Book &book);
};


#endif //BOOKSTORE_USER_H