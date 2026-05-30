#include "level_menu.hpp"
#include "game_settings.hpp"

#include <iostream>
#include <sstream>
#include <string>

namespace Utils {

void showLevelMenuTerminal()
{
    int choice = 1; // default to Medium
    std::cout << "Select difficulty:\n"
              << "0: Easy\n"
              << "1: Medium (default)\n"
              << "2: Hard\n"
              << "Enter choice (0-2, press Enter for default): ";
    std::string input;
    if (std::getline(std::cin, input)) {
        if (!input.empty()) {
            std::stringstream ss(input);
            int tmp;
            if (ss >> tmp && tmp >= 0 && tmp <= 2) {
                choice = tmp;
            } else {
                std::cout << "Unrecognized input, using default (Medium)\n";
            }
        }
    }

        // Map choices to GameSettings values
    switch (choice)
    {
    case 0: // Easy
        GameSettings::Rows = 12;
        GameSettings::Cols = 8;
        GameSettings::MissesBeforeNewRow = 5;
        GameSettings::MinGroupSize = 3;
        break;
    case 1: // Medium (default)
        GameSettings::Rows = 20;
        GameSettings::Cols = 16;
        GameSettings::MissesBeforeNewRow = 3;
        GameSettings::MinGroupSize = 3;
        break;
    case 2: // Hard
        GameSettings::Rows = 24;
        GameSettings::Cols = 20;
        GameSettings::MissesBeforeNewRow = 2;
        GameSettings::MinGroupSize = 3;
        break;
    default:
        // Cancel or unrecognized -> use game_settings
        break;
    }

}

} // namespace Utils
