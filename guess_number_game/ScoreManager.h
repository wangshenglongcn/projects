#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <vector>
#include <iostream>
#include <string>
#include <fstream>



class ScoreManager {
public:
    struct Record {
        int attempts;
        int target;
        bool success;
        int start, end;
    };

    ScoreManager() {
        loadHistoryFromFile();
    }

    void addRecord(int attempts, int target, bool success, int start, int end) {
        Record r{attempts, target, success, start, end};
        history.emplace_back(r);
        addHistoryToFile(r);
    }

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

    void loadHistoryFromFile() {
        std::ifstream fin(filename);
        if (!fin) return;

        Record r;
        while (fin >> r.attempts >> r.target >> r.success >> r.start >> r.end) {
            history.push_back(r);
        }
        fin.close();
    }
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
    std::vector<Record> history;
    const std::string filename = "history.txt";
};

#endif // SCOREMANAGER_H
