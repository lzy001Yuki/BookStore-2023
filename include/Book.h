//
// Created by Yuki on 2023/12/17.
//
#ifndef BOOKSTORE_BOOK_H
#define BOOKSTORE_BOOK_H
#include <vector>
#include <iomanip>
#include "TokenScanner.h"
#include "Finance.h"

class UserAll;
class Finance;

class BookInfo{
    friend class Book;
    friend class UserAll;
    friend class Block<BookInfo>;
    friend class fin_info;
private:
    int size = 0;
    int index_num = 0;
    int next = 0;
    char index[70] = {'\0'};
    //bool isSelected = false; // 及时改变状态,但也不需要？？？
    char ISBN[30] = {'\0'};
    char BookName[70] = {'\0'};
    char Author[70] = {'\0'};
    int Quantity = 0;
    double price = 0.0;
    //double TotalCost = 0.0; // 注意！保留到小数点后两位 检查与输出
    //std::vector<const char*> Keyword;// 存储keyword的时候“重复储存”
    //char keyword[66] = {'\0'};
    //std::string keyword;
    char keyword[66] = {'\0'};
    bool hasKey = false;
    double cost = 0.0; // 成本价格, 输出盈利信息
public:
    BookInfo() = default;
    explicit BookInfo(const char *index_);
    ~BookInfo() = default;

    bool operator == (const BookInfo &obj) const;
    bool operator < (const BookInfo &obj) const;
    bool operator > (const BookInfo &obj) const;
    bool operator != (const BookInfo &obj) const;
    //bool operator = (const BookInfo &obj) const;
};

/// 考虑途径：搜索name->isbn->all所以每次只要更改isbn中的数值就可以

/*class Node{ // 索引为name
    friend class Block<Node>;
    friend class Book;
private:
    int size = 0;
    int index_num = 0;
    int next = 0;
    char index[66] = {'\0'};
    char isbn[66] = {'\0'};
public:
    Node() = default;

    explicit Node(const char *index_);
    explicit Node(const char *index_, const char *isbn_);
    explicit Node(const char *index_, char *isbn_);

    bool operator == (const Node &obj) const;
    bool operator < (const Node &obj) const;
    bool operator > (const Node &obj) const;
    bool operator != (const Node &obj) const;
};*/

class Key{
    friend class Block<Key>;
    friend class Book;
private:
    int size = 0;
    int index_num = 0;
    int next = 0;
    char index[66] = {'\0'};
    char isbn[66] = {'\0'};
public:
    Key() = default;

    explicit Key(const char *index_);
    explicit Key(const char *index_, const char *isbn_);
    explicit Key(const char *index_, char *isbn_);

    bool operator == (const Key &obj) const;
    bool operator < (const Key &obj) const;
    bool operator > (const Key &obj) const;
    bool operator != (const Key &obj) const;
};


class Book {
    friend class UserAll;
    friend class Finance;
private:
    Block<BookInfo> book_isbn;
    /// bookname 也有可能一样！！！
    Block<Key> book_name; // name->isbn
    Block<Key> book_author; // author->isbn
    Block<Key> book_keyword; // keyword->isbn
    BookInfo select_info; // 注意这个指的是现在登录的账户的选择图书

public:
    Book() : book_isbn("isbn.txt"), book_name("name.txt"), book_author("author.txt"), book_keyword("keyword.txt") {
        book_isbn.Clear();
        book_name.Clear();
        book_author.Clear();
        book_keyword.Clear();
    }
    ~Book() ;

    void modify(const char *isbn, const char *name, const char *author, const char* all_key, double price, UserAll &user_all,
                bool I_flag, bool n_flag, bool a_flag, bool k_flag, bool p_flag);

    void Import(int quantity, double total, UserAll &user_all, Finance &fin) ;

    static std::vector<std::string> SplitWords(const char *keyword) ;

    void buy(const char *isbn, int quantity, UserAll &user_all, Finance &fin);

    void show(const char *isbn, const char *name, const char *author, const char *keyword, UserAll &user_all) ;

    void print_isbn(char *isbn) ;

    void show_all(UserAll &user_all) ;

    void select(const char* isbn_, UserAll &user_all);

};


#endif //BOOKSTORE_BOOK_H
