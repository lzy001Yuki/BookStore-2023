//
// Created by 25825 on 2023/12/18.
//
#include "../include/Book.h"

BookInfo::BookInfo(const char *index_) {
    strcpy(index, index_);
    strcpy(ISBN, index_);
}

bool BookInfo::operator == (const BookInfo &obj) const {
    if (strcmp(index, obj.index) == 0) return true;
    else return false;
}

bool BookInfo::operator < (const BookInfo &obj) const {
    if (strcmp(index, obj.index) < 0) return true;
    else return false;
}

bool BookInfo::operator > (const BookInfo &obj) const {
    if (strcmp(index, obj.index) > 0) return true;
    else return false;
}

bool BookInfo::operator != (const BookInfo &obj) const {
    if (strcmp(index, obj.index) == 0) return false;
    else return true;
}


Node::Node(const char *index_) {
    strcpy(index, index_);
}

Node::Node(const char *index_, const char *isbn_) {
    strcpy(index, index_);
    strcpy(isbn, isbn_);
}
Node::Node(const char *index_, char *isbn_) {
    strcpy(index, index_);
    strcpy(isbn, isbn_);
}

bool Node::operator == (const Node &obj) const {
    if (strcmp(index, obj.index) == 0) return true;
    else return false;
}

bool Node::operator < (const Node &obj) const {
    if (strcmp(index, obj.index) < 0) return true;
    else return false;
}

bool Node::operator > (const Node &obj) const {
    if (strcmp(index, obj.index) > 0) return true;
    else return false;
}

bool Node::operator != (const Node &obj) const {
    if (strcmp(index, obj.index) == 0) return false;
    else return true;
}

Key::Key(const char *index_) {
    strcpy(index, index_);
}

Key::Key(const char *index_, const char *isbn_) {
    strcpy(index, index_);
    strcpy(isbn, isbn_);
}

Key::Key(const char *index_, char *isbn_) {
    strcpy(index, index_);
    strcpy(isbn, isbn_);
}

bool Key::operator == (const Key &obj) const {
    if (strcmp(index, obj.index) == 0) {
        if (strcmp(isbn, obj.isbn) == 0) return true;
        else return false;
    }
    else return false;
}

bool Key::operator < (const Key &obj) const {
    if (strcmp(index, obj.index) < 0) return true;
    else if (strcmp(index, obj.index) == 0) {
        if (strcmp(isbn, obj.isbn) < 0) return true;
        else return false;
    } else return false;
}

bool Key::operator > (const Key &obj) const {
    if (strcmp(index, obj.index) > 0) return true;
    else if (strcmp(index, obj.index) == 0) {
        if (strcmp(isbn, obj.isbn) > 0) return true;
        else return false;
    } else return false;
}

bool Key::operator != (const Key &obj) const {
    if (*this == obj) return false;
    else return true;
}

Book::~Book(){
    book_isbn.Clear();
    book_name.Clear();
    book_author.Clear();
    book_keyword.Clear();
}


void Book::modify(const char *isbn, const char *name, const char *author, const char* all_key, double price, UserAll &user_all,
                  bool I_flag, bool n_flag, bool a_flag, bool k_flag, bool p_flag) {
    if (user_all.LogUsers.empty()) throw InvalidExp();
    User now_user = user_all.LogUsers.back();
    if (now_user.permission < 3) throw InvalidExp();
    if (!now_user.select_one) throw InvalidExp();
    //BookInfo tmp(now_user.select_isbn);
    //BookInfo target;
    //book_isbn.Find(tmp, target); /// 是不是没有用？
    //if (!exist) throw InvalidExp();
    if (n_flag) {
        // book_name更改
        // 删除的时候已经进行了查找
        // 如果书名存在 isbn->name
        Node pre_name_isbn(select_info.BookName, now_user.select_isbn); // BookName可能为空
        book_name.Delete(pre_name_isbn);// 没有就不删除
        strcpy(select_info.BookName, name);// book_isbn更改
        Node new_name_isbn(select_info.BookName, now_user.select_isbn);
        book_name.Insert(new_name_isbn);
    }
    if (a_flag) {
        Node pre_author_isbn(select_info.Author, now_user.select_isbn);
        book_author.Delete(pre_author_isbn);
        strcpy(select_info.Author, author);// book_isbn更改
        Node new_author_isbn(select_info.Author, now_user.select_isbn);
        book_author.Insert(new_author_isbn);
    }
    // 处理关键字
    /// update
    if (k_flag) {
        std::vector<std::string> ans1 = SplitWords(all_key);
        if (select_info.hasKey) {
            // 删除原来的关键词索引
            /// special delete!!!
            /// 为keyword新开一个类
            std::vector<std::string> ans2 = SplitWords(select_info.keyword);
            for (auto &it : ans2) {
                char ch[66] = {'\0'};
                strcpy(ch, it.c_str());
                Key pre_keyword_isbn(ch, now_user.select_isbn);
                book_keyword.Delete(pre_keyword_isbn);
            }
        }
        //target.keyword = all_key;
        strcpy(select_info.keyword, all_key);
        for (auto &it : ans1) {
            char key[66] = {'\0'};
            strcpy(key, it.c_str());
            Key new_keyword_isbn(key, now_user.select_isbn);
            book_keyword.Insert(new_keyword_isbn);
        }
        select_info.hasKey = true;
    }

    if (price != -1) {
        select_info.price = price;
    }

    if (I_flag){
        if (strcmp(select_info.ISBN, isbn) == 0) throw InvalidExp();
        book_isbn.Delete(select_info);
        strcpy(select_info.ISBN, isbn);
        strcpy(select_info.index, isbn);
        /// insert
        bool flag = book_isbn.Insert(select_info);
        if (!flag) throw InvalidExp();
        strcpy(now_user.select_isbn, isbn);
        /// 退出的时候选择书籍没有必要进行保存，所以更新users里面的isbn没有用
        //user_all.users.Update(now_user);
        user_all.LogUsers.pop_back();
        user_all.LogUsers.push_back(now_user);
        return;
    }

    //book_isbn.Update(target, target.index_num);
    //book_isbn.Delete(target);
    //book_isbn.Insert(target);
    /// 如果不更改isbn的话，book_isbn的索引不会改变所以没有必要删除再插入，因为位置并没有改变
    book_isbn.Update(select_info);
}

void Book::Import(int quantity, double total, UserAll &user_all, Finance &fin) {
    if (user_all.LogUsers.empty()) throw InvalidExp();
    User now_user = user_all.LogUsers.back();
    if (now_user.permission < 3) throw InvalidExp();
    if (!now_user.select_one) throw InvalidExp();
    /// 可以替换成select_info???
    BookInfo target;
    BookInfo tmp(now_user.select_isbn);
    bool exist = book_isbn.Find(tmp, target);
    if (!exist) throw InvalidExp();// 是不是有些冗余
    target.Quantity += quantity;
    double cost = total * 1.0 / quantity;
    /// 和finance 关联
    bool sta = false;
    fin_info node(total, sta, now_user.UserID, quantity, target.ISBN, target.price, cost);
    int total_count;
    //fin.fin_report.get_info(total_count, 1);
    total_count = fin.get_total();
    total_count++;
    fin.fin_report.write_info(total_count, 1);
    fin.fin_report.write(node, sizeof(fin_info) * (total_count - 1) + 4);
    //fin.TotalCost += total;

    //imp_info imp(select_info.ISBN, total, quantity);

    target.cost = cost;
    //book_isbn.Update(target, target.index_num);
    //book_isbn.Delete(target);
    //book_isbn.Insert(target);
    book_isbn.Update(target);
}

std::vector<std::string> Book::SplitWords(const char *keyword) {
    std::vector<std::string> ans1;
    //std::vector<const char*> ans2;
    if (strlen(keyword) > 60) throw InvalidExp();
    std::string str;
    for (int i = 0; i <= strlen(keyword); i++) {
        if (keyword[i] == '|' || i == strlen(keyword)) {
            for (auto &it : ans1) {
                // 不能出现重复的关键词
                if (it == str) {
                    throw InvalidExp();
                }
            }
            TokenScanner::book3(str);
            ans1.push_back(str);
            str = "";
            continue;
        }
        str += keyword[i];
    }
    return ans1;
    /*for (auto &it : ans1) {
        TokenScanner::book3(it);
        //const char *arr = it.c_str();
        char arr[60] = {'\0'};
        strcpy(arr, it.c_str());
        //ans2.push_back(arr);
    }*/
    //flag = true;
    //return ans2;
}

void Book::buy(const char *isbn, int quantity, UserAll &user_all, Finance &fin) {
    if (quantity <= 0) throw InvalidExp();
    User now_user = user_all.LogUsers.back();
    if (now_user.permission == 0) throw InvalidExp();
    BookInfo target(isbn);
    BookInfo buy_book;
    bool exist = book_isbn.Find(target, buy_book);
    if (!exist) throw InvalidExp();
    if (quantity > buy_book.Quantity) throw InvalidExp();
    buy_book.Quantity -= quantity;
    double price = buy_book.price;
    double cost = price * quantity * 1.0;
    /// 和交易系统相关联
    int total_count;
    total_count = fin.get_total();
    //fin.fin_report.get_info(total_count, 1);
    total_count++;
    fin.fin_report.write_info(total_count, 1);
    bool sta = true;
    fin_info node(cost, sta, now_user.UserID, quantity, buy_book.ISBN, buy_book.price, buy_book.cost);
    fin.fin_report.write(node, 4 + (total_count - 1) * sizeof(fin_info));

    /// 输出格式？
    std::cout<<std::fixed<<std::setprecision(2)<<cost<<'\n';
    book_isbn.Update(buy_book);
}

void Book::show(const char *isbn, const char *name, const char *author, const char *keyword, UserAll &user_all)  {
    if (user_all.LogUsers.empty()) throw InvalidExp();
    User now_user = user_all.LogUsers.back();
    if (now_user.permission == 0) throw InvalidExp();
    if (name != nullptr) {
        Node name_isbn(name);
        Node target;
        // 此时
        bool exist = book_name.Find(name_isbn, target);
        if (!exist) {
            std::cout<<'\n';
            return;
        }
        print_isbn(target.isbn);
        return;
    }
    if (author != nullptr) {
        Node author_isbn(author);
        Node target;
        bool exist = book_author.Find(author_isbn, target);
        if (!exist) {
            std::cout<<'\n';
            return;
        }
        print_isbn(target.isbn);
        return;
    }
    if (keyword != nullptr) {
        // 有很多图书满足
        std::vector<Key> ans;
        Key target(keyword);
        bool exist = book_keyword.FindAll(target, ans);
        if (!exist) {
            std::cout<<'\n';
            return;
        }
        if (ans.empty()) {
            std::cout<<'\n';
            return;
        }
        for (int i = 0; i < ans.size(); i++) {
            print_isbn(ans[i].isbn);
            //std::cout<<'\n';
        }
        return;
    }
    if (isbn != nullptr) {
        BookInfo tmp(isbn);
        BookInfo target;
        bool exist = book_isbn.Find(tmp, target);
        if (!exist) {
            std::cout<<'\n';
            return;
        }
        print_isbn(target.ISBN);
        return;
    }
}

void Book::print_isbn(char *isbn) {
    if (isbn != nullptr) {
        // 从isbn找
        BookInfo tmp(isbn);
        BookInfo target;
        book_isbn.Find(tmp, target);
        std::cout<<target.ISBN<<'\t';
        std::cout<<target.BookName<<'\t';
        std::cout<<target.Author<<'\t';
        std::cout<<target.keyword<<'\t';
        std::cout<<std::fixed<<std::setprecision(2)<<target.price<<'\t';
        std::cout<<target.Quantity<<'\n';
    }
}

void Book::show_all(UserAll &user_all) {
    BookInfo block_index[1000], block_num[1000];
    User now_user = user_all.LogUsers.back();
    if (now_user.permission == 0) throw InvalidExp();
    int total;
    book_isbn.File.get_info(total, 1);
    book_isbn.File.read(block_index[1], 4, total);
    for (int i = 1; i <= total; i = block_index[i].next) {
        if (i == 0) return;
        book_isbn.File.read(block_num[1], 4 + i * book_isbn.MAX * sizeof(BookInfo), block_index[i].size);
        for (int j = 1; j <= block_index[i].size; j++) {
            BookInfo obj = block_num[j];
            std::cout<<obj.ISBN<<'\t'<<obj.BookName<<'\t'<<obj.Author<<'\t';
            /*for (int k = 0; k < obj.Keyword.size(); k++) {
                if (k != obj.Keyword.size() - 1) std::cout<<obj.Keyword[i]<<'|';
                else std::cout<<obj.Keyword[i]<<'\t';
            }*/
            std::cout<<obj.keyword<<'\t';
            std::cout<<std::fixed<<std::setprecision(2)<<obj.price<<'\t';
            std::cout<<obj.Quantity<<'\n';
        }
    }
}

void Book::select(const char* isbn_, UserAll &user_all) {
    BookInfo obj1(isbn_), obj2;
    bool exist = book_isbn.Find(obj1, obj2);
    if (!exist) {
        //obj1.isSelected = true;
        book_isbn.Insert(obj1);// index是isbn
        select_info = obj1;
    } else {
        select_info = obj2;
    }
    user_all.LogUsers.back().select_one = true;
    /// select_isbn存在的意义是什么？
    strcpy(user_all.LogUsers.back().select_isbn, isbn_);
    //users.Update(LogUsers.back(), LogUsers.back().index_num);
    //users.Delete(LogUsers.back());
    //users.Insert(LogUsers.back());
    user_all.users.Update(user_all.LogUsers.back());
}