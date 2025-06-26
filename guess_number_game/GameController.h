#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <iostream>
#include "RandomGenerator.h"
#include "ScoreManager.h"

// 游戏主控制器类，负责主逻辑循环、菜单显示、输入处理等
class GameController {
public:
    // 构造函数，初始化猜数字的范围，默认1~100
    GameController() : start(1), end(100) {}

    // 启动游戏主循环，用do-while实现能够先进行操作，然后可以选择是否重复游玩
    void run() {
        char again;
        do {
            showMenu();
            int choice = getChoice();
            bool quit = false;
            switch (choice) {
                case 1:
                    playGame();  // 开始游戏
                    break;
                case 2:
                    setRange();  // 设置范围
                    break;
                case 3:
                    scores.printHistory();  // 打印历史成绩
                    break;
                case 4:
                    quit = true;  // 退出
                    break;
                default:
                    std::cout << "Invalid choice, try again.\n";
            }
            if (quit) break;
            std::cout << "Do you want to continue? (y/n): ";
            std::cin >> again;
            // 该行是去除整行多余的输入，避免污染后续的读取操作，用cin.ignore(1000, '\n')是去除1000个字符
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');;
        } while (again == 'y' || again == 'Y');

        std::cout << "Thanks for playing!\n";
    }

private:
    RandomGenerator randomGen;  // 随机数生成器
    ScoreManager scores;  // 分数管理器
    int start, end;  // 记录当前游戏数字范围

    // const保证不会修改成员变量，使得const对象可以调用该函数，如const GameController controller; controller.showMenu();
    void showMenu() const {
        std::cout << "=== Guess Number Game ===\n"
                  << "1. Start Game\n"
                  << "2. Set Range\n"
                  << "3. Show History\n"
                  << "4. Quit\n";
    }

    // 获取用户的菜单选项（带输入验证）
    int getChoice() {
        int choice;
        while (true) {
            std::cout << "Enter choice (1-4): ";
            if (std::cin >> choice && choice >= 1 && choice <= 4) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');;
                return choice;
            }
            std::cout << "Invalid input. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');;
        }
    }

    // 设置用户自定义的猜数范围
    void setRange() {
        std::cout << "Enter start of range: ";
        while (!(std::cin >> start)) {
            std::cout << "Invalid input. Please enter an integer: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');;
        }
        std::cout << "Enter end of range: ";
        while (!(std::cin >> end) || end < start) {
            std::cout << "Invalid input. Enter an integer >= start: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');;
    }

    // 主游戏逻辑：最多 10 次猜数机会
    void playGame() {
        int target = randomGen.getRandom(start, end);  // 生成随机数
        std::cout << "I'm thinking of a number between " << start << " and " << end << ".\n";

        int attempts = 0, guess;
        const int maxAttempts = 10;
        bool success = false;
        while (attempts < maxAttempts) {
            std::cout << "Enter your guess: ";
            if (!(std::cin >> guess)) {
                std::cout << "Please enter a valid number.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');;
                continue;
            }
            attempts++;
            if (guess == target) {
                std::cout << "Correct! You guessed in " << attempts << " tries.\n";
                success = true;
                break;
            } else if (guess > target) {
                std::cout << "Too high.\n";
            } else {
                std::cout << "Too low.\n";
            }
        }

        if (!success) {
            std::cout << "You failed. The number was: " << target << "\n";
        }

        // 保存历史记录
        scores.addRecord(attempts, target, success, start, end);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');;
    }
};

#endif // GAMECONTROLLER_H
