
#ifndef BOOKSTORE_BLOCK_H
#define BOOKSTORE_BLOCK_H
#include <algorithm>
#include <cstring>
#include <iostream>
#include <fstream>
#include <unistd.h>

template <class T, int info_len = 2> class MemoryRiver {
private:
    std::fstream file;
    std::string file_name;
    int sizeofT = sizeof(T);

public:
    MemoryRiver() = default;

    MemoryRiver(const std::string &file_name) : file_name(file_name) {}

    void initialise(std::string FN = "") {
        if (FN != "")
            file_name = FN;
        file.open(file_name, std::fstream::in);
        if (access(file_name.c_str(), F_OK) == 0) {
            return;
        }
        file.open(file_name, std::fstream::out);
        int tmp = 0;
        file.seekp(0);
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    // 读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, int n) {
        if (n > info_len)
            return;
        file.open(file_name);
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
        return;
    }

    // 将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len)
            return;
        file.open(file_name, std::fstream::in | std::fstream::out);
        file.seekp(0);
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.seekg(0);
        int x;
        file.read(reinterpret_cast<char*> (&x), sizeof(int));
        file.close();
        return;
    }

    // 在文件合适位置写入类对象t，并返回写入的位置索引index
    // 位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    // 位置索引index可以取为对象写入的起始位置
    void write(T &t, long long place, int size = 1) {
        file.open(file_name ,std::fstream::in | std::fstream::out);
        file.seekp(place);
        file.write(reinterpret_cast<char *>(&t), sizeofT * size);
        file.close();
        return;
    }
    void read(T &t, const long long index, int size = 1) {
        file.open(file_name);
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&t), sizeofT * size);
        file.close();
        return;
    }

    /*void update(T &t, const long long index, int size = 1) {
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char*> (&t), sizeofT * size);
        file.close();
    }*/

    void clear() {
        file.open(file_name);
        file.clear();
        file.close();
    }

};

template <class T>
class Block {
    friend class Book;
private:
    MemoryRiver<T, 1> File;

public:
    const int MAX = 1000;
    const int MIN = 500;
    explicit Block(std::string file_name) {
        File.initialise(file_name);
    }

    int IndexFind(T &obj) {
        int total;
        File.get_info(total, 1);
        T block_index[1050], block_num[1050];
        File.read(block_index[1], 4, total);
        int last = 1;
        for (int i = 1; i; i = block_index[i].next) {
            if (obj < block_index[i]) {
                return last;
            }
            last = i;
        }
        return last;
    }

    void Divide(int num, int size) {
        T block_index[1050], block_num[1050];
        int mid = size / 2;
        T preNode, newNode;
        File.read(preNode, 4 + (num - 1) * sizeof(T));
        File.read(block_num[1], preNode.index_num * MAX * sizeof(T) + 4, size);
        int total;
        File.get_info(total, 1);
        total++;
        File.write_info(total, 1);
        File.write(block_num[mid + 1], total * MAX * sizeof(T) + 4, size - mid);
        newNode = block_num[mid + 1];
        newNode.size = size - mid;
        newNode.next = preNode.next;
        newNode.index_num = total;
        File.write(newNode, (total - 1) * sizeof(T) + 4);
        preNode.size = mid;
        preNode.next = total;
        File.write(preNode, (num - 1) * sizeof(T) + 4);
    }

    bool Insert(T &key) {
        T block_num[1050];
        int total;
        File.get_info(total, 1);
        // 如果是第一个元素
        if (total == 0) {
            total++;
            key.index_num = 1;
            key.size = 1;
            File.write_info(total, 1);
            File.write(key, 4, 1);
            File.write(key, 4 + MAX * sizeof(T));
            return true;
        }
        T preNode;// 先前与更改之后的索引块
        int target = IndexFind(key);// 索引块的位置
        File.read(preNode, 4 + (target - 1) * sizeof(T), 1);
        // 在数据块中插入
        File.read(block_num[1], 4 + preNode.index_num * MAX * sizeof(T), preNode.size);
        int num = std::lower_bound(block_num + 1, block_num + 1 + preNode.size, key) - block_num - 1;
        if (block_num[num + 1] == key && num < preNode.size) {
            return false; //重复元素
        }
        int preSize = preNode.size;
        int pre_index_num = preNode.index_num;
        int pre_next = preNode.next;
        if (num == 0) {
            // 说明是这个块中最小的元素
            File.write(key, 4 + preNode.index_num * MAX * sizeof(T), 1);
            File.write(block_num[1], 4 + (preNode.index_num * MAX + 1) * sizeof(T), preNode.size);
            preNode = key;// 索引块为块中的第一个块
        } else {
            File.write(block_num[1], 4 + preNode.index_num * MAX * sizeof(T), num);
            File.write(key, 4 + (preNode.index_num * MAX + num) * sizeof(T), 1);
            if (num != preNode.size) {
                // key不是最大的
                File.write(block_num[num + 1], 4 + (preNode.index_num * MAX + num + 1) * sizeof(T),
                           preNode.size - num);
            }
            preNode = block_num[1];
        }
        // 覆盖原有索引块
        preSize++;
        preNode.size = preSize;
        preNode.next = pre_next;
        preNode.index_num = pre_index_num;
        File.write(preNode, 4 + (target - 1) * sizeof(T));
        T check;
        File.read(check, 4 + (target - 1) * sizeof(T));
        if (preNode.size > MIN) {
            // 裂块
            Divide(target, preNode.size);
        }
        return true;
    }


    // 更新某一个数据块
    /*void Update(T &key, int index_num) {
        // 看是否是索引块
        int total;
        File.get_info(total, 1);
        File.read(block_index[1], 4, total);
        bool isIndex = false;
        for (int i = 1; i <= total; i = block_index[i].next) {
            if (i == 0) break;
            if (block_index[i] == key) {
                isIndex = true;
                break;
            }
        }
        if (isIndex) File.update(key, 4 + (index_num - 1) * sizeof(T), 1);
        File.update(key, 4 + index_num * MAX * sizeof(T), 1);
    }*/

    bool Delete(T &key) {
        T block_index[1050], block_num[1050];
        int total;
        int target = IndexFind(key);
        File.get_info(total, 1);
        if (total == 0) {
            return false;
        }
        T tmp;
        File.read(tmp, 4 + (target - 1) * sizeof(T));
        File.read(block_num[1], MAX * sizeof(T) * tmp.index_num + 4, tmp.size);
        int num = std::lower_bound(block_num + 1, block_num + tmp.size + 1, key) - block_num - 1;
        if (key != block_num[num + 1]) return false;
        if (num == 0) {
            //元素在第一
            block_num[2].size = tmp.size;
            block_num[2].index_num = tmp.index_num;
            block_num[2].next = tmp.next;
            //File.write(block_num[2], (target - 1) * sizeof(T) + 4);
            if (tmp.size != 1) {
                File.write(block_num[2], 4 + MAX * tmp.index_num * sizeof(T), tmp.size - 1);
                tmp = block_num[2];
            }
            else {
                T empty;
                File.write(empty, 4 + MAX * tmp.index_num * sizeof(T), 1);
            }
            // 因为key会被更改与再次使用，所以作为索引块的“痕迹”应该去掉
            key.size = 0;
        } else {
            File.write(block_num[1], MAX * tmp.index_num * sizeof(T) + 4, num);
            if (num != tmp.size - 1) File.write(block_num[num + 2], (MAX * tmp.index_num + num) * sizeof(T) + 4, tmp.size - num - 1);
            else {
                T empty;
                File.write(empty, (MAX * tmp.index_num + num) * sizeof(T) + 4, 1);
            }
        }
        // 处理索引块
        tmp.size--;
        if (tmp.size != 0) File.write(tmp, 4 + (target - 1) * sizeof(T));
        else {
            T empty;
            empty.next = tmp.next;
            empty.index_num = tmp.index_num;
            File.write(empty, 4 + (target - 1) * sizeof(T));
            //total--;
            //File.write_info(total,1);
        }
        T check_index[1050], check_num[1050];
        File.read(check_index[1], 4 + (target - 1) * sizeof(T));
        File.read(check_num[1], 4 + (MAX * tmp.index_num) * sizeof(T), tmp.size);

        if (tmp.size < MIN) {
            int next_pos = tmp.next;
            if (next_pos == 0) return true;// 不考虑最后一个块
            T nextNode;
            File.read(nextNode, 4 + (next_pos - 1) * sizeof(T), 1);
            if (nextNode.size > MIN) {
                File.write(nextNode, (MAX * tmp.index_num + tmp.size) * sizeof(T) + 4, 1);
                File.read(block_num[1], MAX * nextNode.index_num * sizeof(T) + 4, nextNode.size);
                T empty;
                File.write(block_num[2], MAX * nextNode.index_num * sizeof(T) + 4, nextNode.size - 1);
                nextNode.size--;
                nextNode = block_num[2];
                File.write(empty, (MAX * nextNode.index_num + nextNode.size) * sizeof(T) + 4, 1);
                File.write(nextNode, 4 + (next_pos - 1) * sizeof(T), 1);
            }
        }
        return true;
    }

    void Update(T &target) {
        T block_index[1050], block_num[1050];
        int total;
        int theIndex = IndexFind(target);
        File.get_info(total, 1);
        T tmp;
        File.read(tmp, 4 + (theIndex - 1) * sizeof(T));
        File.read(block_num[1], 4 + tmp.index_num * MAX * sizeof(T), tmp.size);
        int num = std::lower_bound(block_num + 1, block_num + tmp.size + 1, target) - block_num;
        File.write(target, 4 + (MAX * tmp.index_num + num - 1) * sizeof(T));
    }

/// 输出？存疑
    // 从T查找
    /// 关键词可能相同，所以一个索引可能会对应多个值
    bool Find(T key, T &target) {
        T block_num[1050];
        int total;
        int theIndex = IndexFind(key);
        File.get_info(total, 1);
        if (total == 0) {
            //std::cout << "null" << '\n';
            return false;
        }
        T tmp;
        File.read(tmp, 4 + (theIndex - 1) * sizeof(T));
        File.read(block_num[1], 4 + tmp.index_num * MAX * sizeof(T), tmp.size);
        int num = std::lower_bound(block_num + 1, block_num + tmp.size + 1, key) - block_num;
        if (block_num[num] != key) return false;
        else {
            target = block_num[num];
        }
        return true;
    }

    // 在知道一定存在的条件下使用
    T find_target(T &key) {
        T block_index[1050], block_num[1050];
        int total;
        int theIndex = IndexFind(key);
        File.get_info(total, 1);
        T tmp;
        File.read(tmp, 4 + (theIndex - 1) * sizeof(T));
        File.read(block_num[1], 4 + tmp.index_num * MAX * sizeof(T), tmp.size);
        int num = std::lower_bound(block_num + 1, block_num + tmp.size + 1, key) - block_num;
        return block_num[num];
    }

    bool FindAll(T &key, std::vector<T> &ans) {
        T block_index[1050], block_num[1050];
        int total;
        int theIndex = IndexFind(key);
        File.get_info(total, 1);
        if (total == 0) {
            return false;
        }
        T tmp;
        bool nextOne = true;
        bool flag = false;
        while (nextOne) {
            File.read(tmp, 4 + (theIndex - 1) * sizeof(T));
            File.read(block_num[1], 4 + tmp.index_num * MAX * sizeof(T), tmp.size);
            int num = std::lower_bound(block_num + 1, block_num + tmp.size + 1, key) - block_num;
            for (int i = num; i <= tmp.size; i++) {
                if (strcmp(block_num[i].index, key.index) == 0) {
                    flag = true;
                    //std::cout << block_num[i].value << ' ';
                    ans.push_back(block_num[i]);
                } else {
                    nextOne = false;
                    break;
                }
            }
            theIndex = tmp.next;// 下一个块里面可能还有index
            if (theIndex == 0) {// 找到最后一个块了
                break;
            }
        }
        if (!flag) {
            return false;
        }
        return true;
    }

    void Clear() {
        File.clear();
    }


};
#endif
