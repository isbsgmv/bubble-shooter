#include "game.h"

Game::Game() : m_rng(std::random_device{}()),
               m_board(kRows, kCols, kNColors, m_rng)
{}

void Game::printBoard(std::ostream &out) const
{
    m_board.print(out);
}


