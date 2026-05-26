#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "board.hpp"

TEST(BoardTest, Constructor) {
    // Verifies basic board dimensions are stored correctly.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    EXPECT_EQ(board.rows(), 8u);
    EXPECT_EQ(board.cols(), 8u);
}

TEST(BoardTest, InBoundsWorks) {
    // Confirms coordinate bounds checking accepts valid cells and rejects invalid ones.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    EXPECT_TRUE(board.inBounds(0, 0));
    EXPECT_TRUE(board.inBounds(7, 7));
    EXPECT_FALSE(board.inBounds(-1, 0));
    EXPECT_FALSE(board.inBounds(0, 8));
}

TEST(BoardTest, SetAndGetWorks) {
    // Ensures writing then reading a cell returns the same color.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    board.set(2, 3, Bubble::Color::Red);
    EXPECT_EQ(board.get(2, 3), Bubble::Color::Red);
}

TEST(BoardTest, IsEmpty)
{
    // Checks empty-cell semantics and that out-of-bounds queries are never treated as empty.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);

    // Rows below the initialized top rows start empty.
    EXPECT_TRUE(board.isEmpty(7, 0));

    board.set(7, 0, Bubble::Color::Blue);
    EXPECT_FALSE(board.isEmpty(7, 0));

    EXPECT_FALSE(board.isEmpty(-1, 0));
    EXPECT_FALSE(board.isEmpty(8, 0));
    EXPECT_FALSE(board.isEmpty(0, 8));
}

TEST(BoardTest, HexNeighborsForEvenRowCenter)
{
    // Validates even-row hex neighbor topology (offset pattern differs by row parity).
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    board.print(std::cout);
    const auto neighbors = board.hexNeighbors(4, 4); // even row

    EXPECT_EQ(neighbors.size(), 6u);
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(3, 3)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(3, 4)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(4, 3)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(4, 5)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(5, 3)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(5, 4)), neighbors.end());
}

TEST(BoardTest, HexNeighborsForOddRowCenter)
{
    // Validates odd-row hex neighbor topology.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    const auto neighbors = board.hexNeighbors(5, 4); // odd row

    EXPECT_EQ(neighbors.size(), 6u);
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(4, 4)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(4, 5)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(5, 3)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(5, 5)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(6, 4)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(6, 5)), neighbors.end());
}

TEST(BoardTest, HexNeighborsAtTopLeftCorner)
{
    // Corner cells should only return in-bounds neighbors.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    const auto neighbors = board.hexNeighbors(0, 0);

    EXPECT_EQ(neighbors.size(), 2u);
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(0, 1)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(1, 0)), neighbors.end());
}

TEST(BoardTest, ClearConnectedGroupEmptyCell)
{
    // Clearing should be a no-op when starting on an empty cell.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    EXPECT_EQ(board.clearConnectedGroup(7, 7, 3), 0);
}

TEST(BoardTest, ClearConnectedGroupRespectsMinimumSize)
{
    // A connected component smaller than threshold must remain unchanged.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    board.set(7, 0, Bubble::Color::Green);
    board.set(7, 1, Bubble::Color::Green);
    board.print(std::cout);

    EXPECT_EQ(board.clearConnectedGroup(7, 0, 3), 0);
    EXPECT_EQ(board.get(7, 0), Bubble::Color::Green);
    EXPECT_EQ(board.get(7, 1), Bubble::Color::Green);
}

TEST(BoardTest, ClearConnectedGroupClearsWhenMinimumReached)
{
    // A connected component meeting threshold should be removed and counted.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    board.set(6, 3, Bubble::Color::Yellow);
    board.set(6, 4, Bubble::Color::Yellow);
    board.set(7, 3, Bubble::Color::Yellow);
    board.print(std::cout);

    EXPECT_EQ(board.clearConnectedGroup(6, 3, 3), 3);
    EXPECT_TRUE(board.isEmpty(6, 3));
    EXPECT_TRUE(board.isEmpty(6, 4));
    EXPECT_TRUE(board.isEmpty(7, 3));
}

TEST(BoardTest, ClearConnectedGroupDetachedBubble)
{
    // Removing a matching group can detach lower bubbles that should then drop.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    board.set(3, 3, Bubble::Color::Yellow);
    board.set(3, 4, Bubble::Color::Yellow);
    board.set(3, 5, Bubble::Color::Yellow);
    board.set(4, 4, Bubble::Color::Green);
    board.set(5, 4, Bubble::Color::Blue);
    board.print(std::cout);

    EXPECT_EQ(board.clearConnectedGroup(3, 3, 3), 3);
    EXPECT_TRUE(board.isEmpty(3, 3));
    EXPECT_TRUE(board.isEmpty(3, 4));
    EXPECT_TRUE(board.isEmpty(3, 5));
    EXPECT_TRUE(board.isEmpty(4, 4));
    EXPECT_TRUE(board.isEmpty(5, 4));
    
    board.print(std::cout);
}

TEST(BoardTest, OutOfRangeIndices)
{
    // Public accessors should enforce index safety through exceptions.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);

    EXPECT_THROW(board.get(8, 0), std::out_of_range);
    EXPECT_THROW(board.get(0, 8), std::out_of_range);
    EXPECT_THROW(board.set(8, 0, Bubble::Color::Red), std::out_of_range);
    EXPECT_THROW(board.set(0, 8, Bubble::Color::Red), std::out_of_range);
}