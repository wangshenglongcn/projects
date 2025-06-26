#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <iostream>
#include "RandomGenerator.h"
#include "ScoreManager.h"

class GameController {
public:
    GameController() : start(1), end(100) {}

    void run() {
        char again;
        do {
            showMenu();
            int choice = getChoice();
            bool quit = false;
            switch (choice) {
                case 1:
                    playGame();
                    break;
                case 2:
                    setRange();
                    break;
                case 3:
                    scores.printHistory();
                    break;
                case 4:
                    quit = true;
                    break;
                default:
                    std::cout << "Invalid choice, try again.\n";
            }
            if (quit) break;
            std::cout << "Do you want to continue? (y/n): ";
            std::cin >> again;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');;
        } while (again == 'y' || again == 'Y');

        std::cout << "Thanks for playing!\n";
    }

private:
    RandomGenerator randomGen;
    ScoreManager scores;
    int start, end;

    void showMenu() const {
        std::cout << "=== Guess Number Game ===\n"
                  << "1. Start Game\n"
                  << "2. Set Range\n"
                  << "3. Show History\n"
                  << "4. Quit\n";
    }

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

    void playGame() {
        int target = randomGen.getRandom(start, end);
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

        scores.addRecord(attempts, target, success, start, end);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');;
    }
};

#endif // GAMECONTROLLER_H
