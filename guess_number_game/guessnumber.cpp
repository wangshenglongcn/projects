#include <iostream>
#include <random>
#include <fstream>

using namespace std;

const string fileanme = "history.txt";

struct Record {
    int attempts;
    int target;
    bool success;
    int start, end;
};

vector<Record> history;


void loadHistory() {
    ifstream fin(fileanme);
    if (!fin) return;

    Record r;
    while (fin >> r.attempts >> r.target >> r.success >> r.start >> r.end) {
        history.push_back(r);
    }
    fin.close();
}

void addHistory() {
    ofstream fout(fileanme, ios::app);
    if (!fout) {
        cout << "Failed to open file for writing.\n";
        return;
    }
    Record r = history.back();
    fout << r.attempts << " " << r.target << " " << r.success << " " << r.start << " " << r.end << "\n";

    fout.close();
}

void playGame(mt19937& generator, int start, int end) {
    uniform_int_distribution<int> distribution(start, end);
    int target = distribution(generator);

    cout << "I'm thinking of a number between " << start << " and " << end << ".\n";
    cout << "Can you guess it?\n";
    int num, attempts = 0;
    while (true) {
        cout << "Enter your guess: ";
        if (!(cin >> num)) {
            cout << "Please enter a valid number.\n";
            cin.clear();  // 清除cin的错误状态
            cin.ignore(1000, '\n');  // 清除输入的字符，目前只限制了1000字符
            continue;
        }

        attempts++;
        if (num == target) {
            cout << num << " is correct. You guessed it in " << attempts << " tries.\n";
            history.push_back({attempts, target, true, start, end});
            addHistory();
            break;
        } else if (num > target) {
            cout << num << " is too big.\n";
        } else {
            cout << num << " is too small.\n";
        }
        if (attempts == 10) {
            cout << "You failed to guess the number. It was " << target << ".\n";
            history.push_back({attempts, target, true, start, end});
            addHistory();
            break;
        }
    }
}

void showMenu() {
    cout << "1. start game\n";
    cout << "2. setting range\n";
    cout << "3. history\n";
    cout << "4. quit\n";
}

void setRange(int& start, int& end) {
    cout << "Please enter a start of the range: ";
    while (!(cin >> start)) {
        cout << "Please enter a valid number\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cout << "Please enter an end of the range: ";
    while (!(cin >> end) || end < start) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please enter a valid number, ans make sure it's bigger than start: ";
    }
}

void printHistory() {
    cout << boolalpha;
    cout << "Below is your histories: \n";
    int idx = 1;
    for (auto h : history) {
        cout << "Game #" << idx++ << endl;
        cout << "attempts: " << h.attempts << endl;
        cout << "target: " << h.target << endl;
        cout << "success: " << h.success << endl;
        cout << "start: " << h.start << ", end: " << h.end << endl;
        cout << "==================\n";
    }
}

int main() {
    random_device rd;
    mt19937 generator(rd());

    loadHistory();
    char again;
    int start = 1, end = 100;
    do {
        showMenu();
        cout << "Please enter your choice(1/2/3/4): ";
        int choice;
        while (!(cin >> choice) || choice < 0 || choice > 4) {
            cout << "Please enter a valid number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        bool q = false;
        switch (choice) {
            case 1: playGame(generator, start, end);
                    break;
            case 2: setRange(start, end);
                    break;
            case 3: printHistory();
                    break;
            case 4: q = true;
                    break;
        }
        if (q) break;
        cout << "Do you want to play again? (y / n): ";
        cin >> again;
        cin.ignore(1000, '\n');
    } while (again == 'y' || again == 'Y');

    cout << "Thanks for playing!\n";

    return 0;
}