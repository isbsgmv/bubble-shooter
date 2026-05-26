#include "game.h"

#include "projectile.hpp"

#include <iostream>
#include <string>

namespace
{
    constexpr std::size_t kMinGroupSize = 3;
} // namespace

Game::Game() : m_board(kRows, kCols, m_colorManager)
{
}

void Game::printBoard(std::ostream &out) const
{
    m_board.print(out);
}

void Game::loop()
{
    std::string input;

    while (true)
    {
        std::cout << "Choose an angle (20-160) or type 'exit': ";

        if (!std::getline(std::cin, input))
        {
            std::cout << "\nInput closed. Exiting game loop.\n";
            break;
        }

        if (input == "exit")
        {
            std::cout << "Exiting game loop.\n";
            break;
        }

        try
        {
            const double angle = std::stod(input);

            if (angle < 20.0 || angle > 160.0)
            {
                std::cout << "Invalid angle. Please choose a value from 20 to 160.\n";
                continue;
            }
            
            const Bubble::Color projectileColor = m_colorManager.getColor();
     
            const auto attached = Projectile::shoot(m_board, angle, projectileColor);

            if (!attached.has_value())
            {
                std::cout << "Shot could not be attached to the grid. Try a different angle.\n";
                continue;
            }

            m_board.clearConnectedGroup(attached->first, attached->second, kMinGroupSize);
            

            printBoard(std::cout);
        }
        catch (...)
        {
            std::cout << "Invalid input. Enter an angle like 75 or type 'exit'.\n";
        }
    }
}
