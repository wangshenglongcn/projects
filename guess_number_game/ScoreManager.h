#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <vector>
#include <iostream>
#include <string>
#include <fstream>


// 管理历史记录，包括添加、打印和文件读写
class ScoreManager {
public:
    // 单局游戏记录结构体
    struct Record {
        int attempts;  // 尝试次数
        int target;  // 正确答案
        bool success;  // 是否成功猜中
        int start, end;  // 范围设置
    };

    // 构造函数：自动加载历史记录文件
    ScoreManager() {
        loadHistoryFromFile();
    }

    // 添加一条游戏记录，并写入文件
    void addRecord(int attempts, int target, bool success, int start, int end) {
        Record r{attempts, target, success, start, end};
        history.emplace_back(r);
        addHistoryToFile(r);
    }

    // 打印所有历史记录
    void printHistory() const {
        if(history.empty()) {
            std::cout << "No games played yet.\n";
            return;
        }

        std::cout << "=== Game History ===\n";
        int idx = 1;
        for (const auto& h : history) {
            std::cout << "Game #" << idx++ << "\n"
                      << "Attempts: " << h.attempts << "\n"
                      << "Target: " << h.target << "\n"
                      << "Success: " << (h.success ? "Yes" : "No") << "\n"
                      << "Range: [" << h.start << ", " << h.end << "]\n"
                      << "------------------\n";
        }
    }

    // 从文件加载历史记录
    void loadHistoryFromFile() {
        std::ifstream fin(filename);
        if (!fin) return;

        Record r;
        while (fin >> r.attempts >> r.target >> r.success >> r.start >> r.end) {
            history.push_back(r);
        }
        fin.close();
    }

    // 将记录追加写入文件
    void addHistoryToFile(const Record& r) {
        std::ofstream fout(filename, std::ios::app);
        if (!fout) {
            std::cerr << "Failed to open file for writing: " << filename << "\n";
            return;
        }

        fout << r.attempts << " " << r.target << " " << r.success
            << " " << r.start << " " << r.end << "\n";
        fout.close();
    }
private:
    std::vector<Record> history;  // 内存中保存的历史记录
    const std::string filename = "history.txt";  // 保存记录的文件名
};

#endif // SCOREMANAGER_H
