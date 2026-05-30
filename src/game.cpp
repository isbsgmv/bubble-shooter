#include "game.h"
#include "renderer/include/sdl2_renderer.hpp"

#include "projectile.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>



Game::Game() : m_board(GameSettings::Rows, GameSettings::Cols, m_colorManager)
{
}

void Game::printBoard(std::ostream &out) const
{
    m_board.print(out);
}

bool Game::isGameOver() const
{
    // Check if any bubble reached the bottom row
    for (std::size_t col = 0; col < GameSettings::Cols; ++col)
    {
        if (m_board.get(GameSettings::Rows - 1, col) != Bubble::Color::None)
        {
            return true;
        }
    }
    return false;
}

bool Game::hasWon() const
{
    for (std::size_t row = 0; row < m_board.rows(); ++row)
    {
        for (std::size_t col = 0; col < m_board.cols(); ++col)
        {
            if (m_board.get(row, col) != Bubble::Color::None)
            {
                return false;
            }
        }
    }

    return true;
}

void Game::loop(SDL2Renderer *renderer)
{
    std::string input;

    renderer->colsRows(m_board.rows(), m_board.cols());
    double launcherX = (static_cast<double>(m_board.cols()) - 1.0) / 2.0;
    double launcherY = static_cast<double>(m_board.rows()) - 1.0;
    Bubble::Color projectileColor = m_colorManager.getColor();
    Bubble::Color nextColor = m_colorManager.getColor();

    // Main game loop
    while (renderer->isRunning() && !isGameOver() && !hasWon())
    {
        renderer->pollEvents();

        // Check for mouse input from SDL2
        if (renderer->shouldShoot())
        {
            auto angle = renderer->getMouseAngle();
            if (angle.has_value())
            {
                std::cout << "Shot at angle: " << angle.value() << " degrees\n";
                const auto attached = Projectile::shoot(m_board, angle.value(), projectileColor, renderer);

                if (!attached.has_value())
                {
                    std::cout << "Shot could not be attached to the grid. Try a different angle.\n";
                    renderer->resetShootFlag();
                    continue;
                }

                int cleared_bubbles = m_board.clearConnectedGroup(attached->first, attached->second, GameSettings::MinGroupSize);
                std::cout << "Cleared " << cleared_bubbles << " bubbles.\n";

                if (cleared_bubbles == 0)
                {
                    ++m_missCount;
                    std::cout << "No bubbles cleared. Number of misses: " << m_missCount << ". Try a different angle.\n";
                }
                else
                {
                    m_missCount = 0; // Reset miss counter on successful shot
                }

                if (hasWon())
                {
                    std::cout << "You win! No bubbles remain.\n";
                    renderer->requestClose();
                    break;
                }

                if (m_missCount >= GameSettings::MissesBeforeNewRow)
                {
                    std::cout << "Too many misses! A new row appears at the top.\n";
                    m_board.addNewRow(m_colorManager);
                    m_missCount = 0;
                }

            }
            renderer->resetShootFlag();

            projectileColor = nextColor;
            nextColor = m_colorManager.getColor();
        }

        // Render the current game state
        RenderStats stats{
            launcherX, launcherY,
            projectileColor,
            nextColor};
        renderer->render(m_board, stats);

        // Print board to CLI as well (optional, can be disabled)
        // Uncomment if you want both CLI and graphics output

        // Small sleep to prevent CPU spinning
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    if (isGameOver())
    {
        std::cout << "Game Over! Bubbles reached the bottom.\n";
    }
}
