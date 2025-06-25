#include <iostream>
#include <random>

using namespace std;

int main() {
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(1, 100);
    int target = distribution(generator);

    cout << "I'm thinking of a number between 1 and 100.\n";
    cout << "Can you guess it?\n";
    int num;
    while (true) {
        cout << "Enter your guess: ";
        if (!(cin >> num)) {
            cout << "Please enter a valid number.\n";
            cin.clear();  // 清除cin的错误状态
            cin.ignore(1000, '\n');  // 清除输入的字符，目前只限制了1000字符
            continue;
        }
        if (num == target) {
            cout << num << " is correct.\n";
            break;
        } else if (num > target) {
            cout << num << " is bigger.\n";
        } else {
            cout << num << " is smaller.\n";
        }
    }

    return 0;
}