#include <iostream>
#include <random>

using namespace std;

void playGame(mt19937& generator) {
    uniform_int_distribution<int> distribution(1, 100);
    int target = distribution(generator);

    cout << "I'm thinking of a number between 1 and 100.\n";
    cout << "Can you guess it?\n";

    int num, attempts = 0;
    while (true) {
        cout << "Enter your guess: ";
        if (!(cin >> num)) {
            cout << "Please enter a valid number.\n";
            cin.clear(); // 清除cin的错误状态
            cin.ignore(1000, '\n'); // 丢弃错误输入
            continue;
        }

        attempts++;
        if (num == target) {
            cout << num << " is correct. You guessed it in " << attempts << " tries.\n";
            break;
        } else if (num > target) {
            cout << num << " is too big.\n";
        } else {
            cout << num << " is too small.\n";
        }

        if (attempts == 10) {
            cout << "You failed to guess the number. It was " << target << ".\n";
            break;
        }
    }
}

int main() {
    random_device rd;
    mt19937 generator(rd());

    char again;
    do {
        playGame(generator);
        cout << "Do you want to play again? (y/n): ";
        cin >> again;
        cin.ignore(1000, '\n'); // 丢掉多余字符
    } while (again == 'y' || again == 'Y');

    cout << "Thanks for playing!\n";
    return 0;
}
