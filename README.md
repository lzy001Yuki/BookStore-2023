# Bookstore-总体开发文档
## **（一）程序功能概述**  
本项目主要实现功能如下：  
* 储存用户信息
* 储存书籍信息并输出相关内容（进货、查询等）
* 储存工作日志（盈利情况、交易信息）
## **（二）主体逻辑说明**  
* TokenScanner类实现指令的切片
* User类实现用户信息系统
* Book类实现书籍信息系统
* Block类实现块状链表，即用户和书籍信息的存储
* Dairy类实现日志系统
* error类实现异常抛出
## **（三）代码文件结构与各类的接口**  
###  User.h/User.hpp 
```
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
    friend class Diary;
private:
    char UserID[MAX_LEN] = {'\0'};
    char Username[MAX_LEN] = {'\0'};
    char Password[MAX_LEN] = {'\0'};
    int permission = 0;// 游客状态
    int index_num = 0;
    int size = 0;
    char index[MAX_LEN] = {'\0'};
    int next = 0;
    //bool log_status = false; // 未登录状态
    int log_cnt = 0; // 可能多次登录登出，所以要计数 cnt = 0 代表没有登录
    bool select_one = false; // false时不管select_isbn是什么，只有在true时考虑
    //BookInfo select_info;
public:
    char select_isbn[66] = {'\0'};/// 不同次登录时的选中图书的isbn可能不相同
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
    int current_permission = 0;
    Block<User> users;
    std::map<std::string, int> log_map;

public:

    std::vector<User> LogUsers;// 登录栈

    UserAll();
    ~UserAll();

    void su(const char *UserID, const char *Password, Diary &diary);

    void Register(const char *UserId, const char *Password, const char *Username, Diary &diary);

    void Delete(const char *UserID, const char* command, Diary &diary);

    void useradd(const char *UserID, const char *Password, int privilege, const char *Username, const char* command, Diary &diary);

    void passwd(const char *UserID, const char *CurrentPassword, const char *NewPassword, const char* command, Diary &diary);

    void logout(Diary &diary, Book &book);

    void exit();
};

```
### Block.h  
```
// 文件存储类
template <class T, int info_len = 2> class MemoryRiver {
private:
    std::fstream file;
    std::string file_name;
    int sizeofT = sizeof(T);
public:
    MemoryRiver() = default;
    explicit MemoryRiver(const std::string &file_name) : file_name(file_name) {}
    void initialise(std::string FN = "");// 初始化

    // 读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, int n)；

    // 将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n)；

    // 在文件合适位置写入类对象t，并返回写入的位置索引index
    // 位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    // 位置索引index可以取为对象写入的起始位置
    void write(T &t, long long place, int size = 1)；
    void read(T &t, const long long index, int size = 1)；
    // 清除文件错误流
    void clear()；
};

template <class T>
class Block {
    friend class Book;
private:
    MemoryRiver<T, 1> File;

public:
    const int MAX = 1000;
    const int MIN = 500;
    explicit Block(std::string file_name);

    int IndexFind(T &obj);
    void Divide(int num, int size);
    bool Insert(T &key);
    bool Delete(T &key);
    void Update(T &target);
    // 从T查找
    /// 关键词可能相同，所以一个索引可能会对应多个值
    bool Find(T key, T &target) ;
    bool FindAll(T &key, std::vector<T> &ans);

    void Clear() ;
};
```
### Book.h/Book.cpp 
```
class UserAll;
class Finance;
class Diary;

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
    char ISBN[30] = {'\0'};
    char BookName[70] = {'\0'};
    char Author[70] = {'\0'};
    int Quantity = 0;
    double price = 0.0;
    char keyword[66] = {'\0'};
    bool hasKey = false;
    double cost = 0.0;
public:
    BookInfo() = default;
    explicit BookInfo(const char *index_);
    ~BookInfo() = default;

    bool operator == (const BookInfo &obj) const;
    bool operator < (const BookInfo &obj) const;
    bool operator > (const BookInfo &obj) const;
    bool operator != (const BookInfo &obj) const;
};

/// 考虑途径：搜索name->isbn->all所以每次只要更改isbn中的数值就可以


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
     // 注意这个指的是现在登录的账户的选择图书

public:
    BookInfo select_info;
    Book() : book_isbn("isbn.txt"), book_name("name.txt"), book_author("author.txt"), book_keyword("keyword.txt") {
        book_isbn.Clear();
        book_name.Clear();
        book_author.Clear();
        book_keyword.Clear();
    }
    ~Book() ;

    void modify(const char *isbn, const char *name, const char *author, const char* all_key, double price, UserAll &user_all,
                bool I_flag, bool n_flag, bool a_flag, bool k_flag, bool p_flag, Diary &diary, const char* command);

    void Import(int quantity, double total, UserAll &user_all, Finance &fin, Diary &diary, const char* command) ;

    static std::vector<std::string> SplitWords(const char *keyword) ;

    void buy(const char *isbn, int quantity, UserAll &user_all, Finance &fin, Diary &diary, const char* command);

    void show(const char *isbn, const char *name, const char *author, const char *keyword, UserAll &user_all, Diary &diary, const char* command) ;

    void print_isbn(char *isbn) ;

    void show_all(UserAll &user_all, Diary &diary, const char* command) ;

    void select(const char* isbn_, UserAll &user_all, Diary &dairy, const char* command);

};

```
###  Diary.h/Diary.cpp 
```
class UserAll;
class Record{
    friend class Diary;
private:
    char userid[66] = {'\0'};
    char operation[1000] = {'\0'};
    int permission = 0;
    double cost = -1.0;
    bool status = true;
public:
    Record() = default;
    explicit Record(const char *userid_, const char *op, int per) :permission(per){
        strcpy(userid, userid_);
        strcpy(operation, op);
    }
    explicit Record(const char* userid_, const char *op, int per, double cost_, bool sta) :permission(per), cost(cost_), status(sta){
        strcpy(userid, userid_);
        strcpy(operation, op);
    }

};

class Diary{
private:
    MemoryRiver<Record, 1> diary;
public:
    Diary();

    void write_diary(Record &tmp);

    void read_employee(UserAll &user_all);

    void read_all(UserAll &user_all);
};

```
###  Finance.h/Finance.cpp 
```
class UserAll;

class fin_info {
    friend class Finance;
    friend class Book;
private:
    double number;
    bool status; // true 表示收入， false表示支出
    char userid[66] = {'\0'};;
public:
    fin_info() : number(0.0), status(true){}
    explicit fin_info(double num, bool sta, const char *userid_) : number(num), status(sta) {
        strcpy(userid, userid_);
    }
};

class Finance {
    friend class Book;

private:
    // 不需要块状链表, 但应该存储到文件之中
    MemoryRiver<fin_info, 1> fin_report; // 开头记录交易总数
public:
    Finance();
    ~Finance();
    int get_total();

    void read();

    void show_finance(int count);

    void report_finance(UserAll &user_all);
};

```
###  error.h/error.cpp 
```
class InvalidExp : public std::exception{
public:
    const char* what() const noexcept override;
};
```
### TokenScanner.h/TokenScanner.cpp
```
// 划分指令并且进行指令正确性检查(processLine)
class TokenScanner{
public:
    static void SplitString (std::string str, std::vector<std::string> &all_string);

    // 检查并且返回Quantity, Count
    static int StringToInteger(std::string str);

    // 检查userid, password, currentPassword, newPassword
    static void customer1(std::string str);

    // 检查username
    static void customer2(std::string str);

    // 检查privilege
    static void customer3(std::string str);

    // 检查ISBN
    static void book1(std::string str);

    // 检查bookName, Author, keyWord(all)
    static void book2(std::string str);

    // 检查一个keyWord
    static void book3(std::string str);

    // 检查price, totalCost
    static double book4(std::string str);

    // 剪切show指令
    static std::string cut(std::string str);

    // keyWord不能包含多个关键词
    static void more_key(std::string str);

    static std::string cut_price(std::string str) ;
};
```

## **（四）文件存储说明**  
* users.txt 存储用户信息（ID、Password、 Username）
* ISBN.txt 储存图书的ISBN号
* author.txt 储存图书的author
* keyword.txt 储存图书的关键字
* diary.txt 储存日志
* finance.txt 储存交易信息
