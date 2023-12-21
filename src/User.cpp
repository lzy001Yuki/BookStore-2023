//
// Created by Yuki on 2023/12/16.
//
#include "../include/User.h"

User::User(const char* index_) {
    strcpy(index, index_);
    strcpy(UserID, index_);
}

bool User::operator == (const User &obj) const {
    if (strcmp(index, obj.index) == 0) return true;
    else return false;
}

bool User::operator < (const User &obj) const {
    if (strcmp(index, obj.index) < 0) return true;
    else return false;
}

bool User::operator > (const User &obj) const {
    if (strcmp(index, obj.index) > 0) return true;
    else return false;
}

bool User::operator != (const User &obj) const {
    if (strcmp(index, obj.index) == 0) return false;
    else return true;
}

UserAll::UserAll() : users("Users.txt") {
    users.Clear();
    // 初始时创建店主账户
    const char *username = "Yuki";
    const char *password = "sjtu";
    const char *userid = "root";
    User owner(userid, password, username, 7);
    User tmp;
    bool exist = users.Find(owner, tmp);
    if (!exist) users.Insert(owner); // 没有创建 ，但就算创建了重复也不会再次插入？？？
}

UserAll::~UserAll() {
    users.Clear();
}

void UserAll::Register(const char *userid, const char *password, const char *username) {
    /// 此处的error可以在TokenScanner类里面进行
    /*error.customer1(userid);
    error.customer1(password);
    error.customer2(username);*/
    User new_customer(userid, password, username, 1);
    // 插入的同时就已经进行了查找
    bool flag1;
    flag1 = users.Insert(new_customer);
    if (!flag1) throw InvalidExp();
    else {
        new_customer.permission = 1;// 顾客状态
    }
}

void UserAll::su(const char *userid, const char *password, Diary &diary) {
    //error.customer1(userid); /// 可以省略
    User find_one(userid);
    User su_customer;
    bool exist = users.Find(find_one, su_customer);
    if (!exist) throw InvalidExp();
    if (password == nullptr && current_permission <= su_customer.permission) throw InvalidExp();
    if (password != nullptr && strcmp(password, su_customer.Password) != 0) throw InvalidExp();
    // 登录后更改状态
    current_permission = su_customer.permission;
    LogUsers.push_back(su_customer);
    su_customer.log_status = true;

    /// Update不更改索引块时
    users.Update(su_customer);
    //users.Update(su_customer, su_customer.index_num);
    //users.Delete(su_customer);
    //users.Insert(su_customer);
    /// 写入日志
    std::string command = "su";
    Record tmp(userid, command, su_customer.permission);
    diary.write_diary(tmp);
}

void UserAll::logout(Diary &diary) {
    if (LogUsers.empty()) throw InvalidExp();

    if (current_permission == 0)  throw InvalidExp();
    User now_customer = LogUsers.back();
    User out_customer;
    /// Find的意义是什么？
    users.Find(now_customer, out_customer);
    out_customer.log_status = false;
    //char empty[66] = {'\0'};
    //strcpy(out_customer.select_isbn, empty);// 无需保存选中图书
    out_customer.select_one = false;
    //users.Update(out_customer, out_customer.index_num);
    //users.Delete(out_customer);
    //users.Insert(out_customer);
    users.Update(out_customer);
    LogUsers.pop_back();
    if (LogUsers.empty()) current_permission = 0;
    else {
        User now_user = LogUsers.back();
        current_permission = now_user.permission;

        /// log_status 有存在的必要吗？
        /// 用Update?
        now_user.log_status = true;
        //users.Update(now_user, now_user.index_num);
        users.Delete(now_user);
        users.Insert(now_user);
    }
    Record tmp(out_customer.UserID, "logout", out_customer.permission);
    diary.write_diary(tmp);
}

void UserAll::Delete(const char *userid) {
    if (current_permission < 7) throw InvalidExp();
    User now_user(userid);
    User delete_user;
    bool exist = users.Find(now_user, delete_user);
    if (!exist) throw InvalidExp();
    if (delete_user.log_status) throw InvalidExp(); // 删除的账户在登录中
    /// 优化：在delete之前find的index
    users.Delete(delete_user);
}

void UserAll::passwd(const char *userid, const char *current, const char *new_one) {
    if (current_permission == 0) throw InvalidExp();
    if (current == nullptr && current_permission < 7) throw InvalidExp();
    User change_user(userid);
    User target;
    bool exist = users.Find(change_user, target);
    /// 存在问题
    if (!exist) throw InvalidExp();
    //error.customer1(new_one);
    if (strcmp(new_one, target.Password) == 0) throw InvalidExp();
    if (current != nullptr) if (strcmp(current, target.Password) != 0) throw InvalidExp();
    strcpy(target.Password, new_one);
    users.Update(target);
}

void UserAll::useradd(const char *userid, const char *Passwd, int privilege, const char *username) {
    //error.permission(privilege);
    if (current_permission < 3) throw InvalidExp();
    if (current_permission < privilege) throw InvalidExp();
    if (privilege == 7) throw InvalidExp();
    User add_customer(userid, Passwd, username, privilege);
    bool exist = users.Insert(add_customer);
    if (!exist) throw InvalidExp();
}

/*void UserAll::select(const char *isbn_, Book &book) {
    BookInfo obj1(isbn_), obj2;
    bool exist = book.book_isbn.Find(obj1, obj2);
    if (!exist) {
        //obj1.isSelected = true;
        book.book_isbn.Insert(obj1);// index是isbn
        LogUsers.back().select_info = obj1;
    } else {
        LogUsers.back().select_info = obj2;
    }
    LogUsers.back().select_one = true;
    /// select_isbn存在的意义是什么？
    strcpy(LogUsers.back().select_isbn, isbn_);
    //users.Update(LogUsers.back(), LogUsers.back().index_num);
    //users.Delete(LogUsers.back());
    //users.Insert(LogUsers.back());
    users.Update(LogUsers.back());
}*/

