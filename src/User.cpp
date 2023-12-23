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

void UserAll::Register(const char *userid, const char *password, const char *username, Diary &diary) {
    User new_customer(userid, password, username, 1);
    // 插入的同时就已经进行了查找
    bool flag1;
    flag1 = users.Insert(new_customer);
    if (!flag1) {
        std::cout<<"register: already exist"<<'\n';
        throw InvalidExp();
    }
    else {
        new_customer.permission = 1;// 顾客状态
    }
    Record tmp(userid, "Register", 0);
    diary.write_diary(tmp);
}

void UserAll::su(const char *userid, const char *password, Diary &diary) {
    User find_one(userid);
    User su_customer;
    bool exist = users.Find(find_one, su_customer);
    if (!exist) {
        //std::cout<<"su : no exist"<<'\n';
        throw InvalidExp();
    }
    if (password == nullptr && current_permission <= su_customer.permission) {
        throw InvalidExp();
    }
    if (password != nullptr && strcmp(password, su_customer.Password) != 0) {
        throw InvalidExp();
    }
    // 登录后更改状态
    current_permission = su_customer.permission;
    su_customer.log_cnt++;
    log_map.insert({userid, su_customer.log_cnt});
    //log_map[userid] = su_customer.log_cnt;
    //su_customer.log_status = true;
    su_customer.select_one = false;
    char empty_ch[66] = {'\0'};
    strcpy(su_customer.select_isbn, empty_ch);

    LogUsers.push_back(su_customer);

    ////
    char ch[] = "T3yikdIWca7z";
    if (strcmp(userid, ch) == 0) std::cout<<"-----------------------T3yikdIWca7z su!!!!"<<'\n';

    /// Update不更改索引块时
    users.Update(su_customer);

    /// 写入日志
    Record tmp(userid, "su", su_customer.permission);
    diary.write_diary(tmp);
}

void UserAll::logout(Diary &diary, Book &book) {
    if (LogUsers.empty()) {
        //std::cout<<"logout: no login"<<"\n";
        throw InvalidExp();
    }
    if (current_permission == 0)  {
        //std::cout<<"logout: per = 0"<<'\n';
        throw InvalidExp();
    }
    User out_customer;
    User now_customer = LogUsers.back();
    users.Find(now_customer, out_customer);
    out_customer.log_cnt--;
    log_map.insert({out_customer.UserID, out_customer.log_cnt});

    /// Find的意义是什么？ LogUsers不可靠！！！
    //out_customer.log_status = false;
    //out_customer.log_cnt = 0;
    //char empty[66] = {'\0'};
    //strcpy(out_customer.select_isbn, empty);// 无需保存选中图书

    ///out_customer.select_one = false;存疑？？？

    /// 这点可以优化，但还是先不考虑吧。。。。
    users.Update(out_customer);

    ///
    char ch[] = "T3yikdIWca7z";
    if (strcmp(out_customer.UserID, ch) == 0) std::cout<<"----------------------------T3yikdIWca7z: logout!!!"<<'\n';

    LogUsers.pop_back();
    if (LogUsers.empty()) {
        current_permission = 0;
        BookInfo empty;
        book.select_info = empty;
    }
    else {
        User new_user = LogUsers.back();
        current_permission = new_user.permission;
        //User true_user;
        //bool the_flag = false;
        //users.Find()
        /// log_status 有存在的必要吗？
        /// 用Update?
        /// now_user会重复登录，更改密码不能只更改最新的那一个， 所有更改均是如此，LogUsers只提供一个记录登录的用户的功能

        //now_user.log_status = true;

        //current_permission = true_user.permission;
        //true_user.log_status = true;
        BookInfo target(new_user.select_isbn);
        book.book_isbn.Find(target, book.select_info);
        //users.Update(true_user);
    }
    Record tmp(out_customer.UserID, "logout", out_customer.permission);
    diary.write_diary(tmp);
}

void UserAll::Delete(const char *userid, std::string command, Diary &diary) {
    if (current_permission < 7) {
        //std::cout<<"delete: no permission"<<'\n';
        throw InvalidExp();
    }
    User now_user(userid);
    User delete_user;
    bool exist = users.Find(now_user, delete_user);
    if (!exist) {
        //std::cout<<"delete : no exist"<<"\n";
        throw InvalidExp();
    }
    if (log_map[userid] > 0) {
        //std::cout<<"delete: login_ing"<<'\n';
        throw InvalidExp(); // 删除的账户在登录中
    }
    /// 优化：在delete之前find的index
    users.Delete(delete_user);
    Record tmp(userid, std::move(command), 7);
    diary.write_diary(tmp);
}

void UserAll::passwd(const char *userid, const char *current, const char *new_one, std::string command, Diary &diary) {
    /*char ch[] = "qnU4";
    if (strcmp(userid, ch) == 0) {
        std::cout<<"-------------------cur"<<current<<'\n';
        std::cout<<"-------------------new"<<new_one<<"\n";
    }*/
    if (current_permission == 0) {
        //std::cout<<"*************premission_invalid"<<'\n';
        //std::cout<<"passwd: cp=0"<<'\n';
        throw InvalidExp();
    }
    if (current == nullptr && current_permission < 7) {
        //std::cout<<"*************non_invalid"<<'\n';
        //std::cout<<"passwd: cp != 7"<<'\n';
        throw InvalidExp();
    }
    User change_user(userid);
    User target;
    bool exist = users.Find(change_user, target);
    /// 存在问题
    if (!exist) {
        //std::cout<<"passwd: non_exist"<<'\n';
        throw InvalidExp();
    }
    //error.customer1(new_one);
    //if (strcmp(LogUsers.back().UserID, userid) == 0) throw InvalidExp();

    //if (strcmp(new_one, target.Password) == 0) throw InvalidExp();

    if (current != nullptr) if (strcmp(current, target.Password) != 0) {
        //std::cout<<"passwd: wrong_pd"<<'\n';
        throw InvalidExp();
    }
    if (strcmp(userid, LogUsers.back().UserID) == 0) strcpy(LogUsers.back().Password, new_one);// 修改现在用户的密码,但感觉没有什么必要
    strcpy(target.Password, new_one);
    users.Update(target);
    Record tmp(userid, std::move(command), current_permission);
    diary.write_diary(tmp);
}

void UserAll::useradd(const char *userid, const char *Passwd, int privilege, const char *username, std::string command, Diary &diary) {
    if (current_permission < 3) {
        //std::cout<<"useradd: no cp"<<"\n";
        throw InvalidExp();
    }
    if (current_permission < privilege) {
        //std::cout<<"useradd: less cp"<<"\n";
        throw InvalidExp();
    }
    if (privilege == 7) throw InvalidExp();
    User add_customer(userid, Passwd, username, privilege);
    bool exist = users.Insert(add_customer);
    if (!exist) {
        //std::cout<<"useradd: already exist"<<'\n';
        throw InvalidExp();
    }
   // Record tmp(userid, std::move(command), current_permission);
   // diary.write_diary(tmp);
}


