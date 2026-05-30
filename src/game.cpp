#include "game.h"
#include "renderer/include/sdl2_renderer.hpp"

#include "projectile.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

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

bool Game::isGameOver() const
{
    // Check if any bubble reached the bottom row
    for (std::size_t col = 0; col < kCols; ++col)
    {
        if (m_board.get(kRows - 1, col) != Bubble::Color::None)
        {
            return true;
        }
    }
    return false;
}

void Game::loop(SDL2Renderer *renderer)
{
    std::string input;

    renderer->colsRows(m_board.rows(), m_board.cols());
    // Main game loop
    double launcherX = (static_cast<double>(m_board.cols()) - 1.0) / 2.0;
    double launcherY = static_cast<double>(m_board.rows()) - 1.0;
    Bubble::Color projectileColor = m_colorManager.getColor();
    // SDL2-based loop with dual input
    while (renderer->isRunning() && !isGameOver())
    {
        renderer->pollEvents();

        // Check for mouse input from SDL2
        if (renderer->shouldShoot())
        {
            std::cout << "Projectile color: " << static_cast<int>(projectileColor) << "\n";
            auto angle = renderer->getMouseAngle();
            if (angle.has_value())
            {
                std::cout << "Shot at angle: " << angle.value() << " degrees\n";
                const auto attached = Projectile::shoot(m_board, angle.value(), projectileColor, renderer);

                if (!attached.has_value())
                {
                    std::cout << "Shot could not be attached to the grid. Try a different angle.\n";
                }

                int cleared_bubbles = m_board.clearConnectedGroup(attached->first, attached->second, kMinGroupSize);
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

                if (m_missCount >= kMissesBeforeNewRow)
                {
                    std::cout << "Too many misses! A new row appears at the top.\n";
                    m_board.addNewRow(m_colorManager);
                    m_missCount = 0;
                }

            }
                renderer->resetShootFlag();
            
            projectileColor = m_colorManager.getColor();
        }

        // Render the current game state
        RenderStats stats{
            launcherX, launcherY,
            projectileColor};
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
