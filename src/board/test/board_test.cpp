#include <gtest/gtest.h>

#include <algorithm>
#include <stdexcept>

#include "board.hpp"

TEST(BoardTest, Constructor) {
    // Verifies basic board dimensions are stored correctly.
    Board board(8, 8, 4);
    EXPECT_EQ(board.rows(), 8u);
    EXPECT_EQ(board.cols(), 8u);
}

TEST(BoardTest, InBoundsWorks) {
    // Confirms coordinate bounds checking accepts valid cells and rejects invalid ones.
    Board board(8, 8, 4);
    EXPECT_TRUE(board.inBounds(0, 0));
    EXPECT_TRUE(board.inBounds(7, 7));
    EXPECT_FALSE(board.inBounds(-1, 0));
    EXPECT_FALSE(board.inBounds(0, 8));
}

TEST(BoardTest, SetAndGetWorks) {
    // Ensures writing then reading a cell returns the same color.
    Board board(8, 8, 4);
    board.set(2, 3, BubbleColor::Red);
    EXPECT_EQ(board.get(2, 3), BubbleColor::Red);
}

TEST(BoardTest, IsEmpty) {
    // Checks empty-cell semantics and that out-of-bounds queries are never treated as empty.
    Board board(8, 8, 4);

    // Rows below the initialized top rows start empty.
    EXPECT_TRUE(board.isEmpty(7, 0));

    board.set(7, 0, BubbleColor::Blue);
    EXPECT_FALSE(board.isEmpty(7, 0));

    EXPECT_FALSE(board.isEmpty(-1, 0));
    EXPECT_FALSE(board.isEmpty(8, 0));
    EXPECT_FALSE(board.isEmpty(0, 8));
}

TEST(BoardTest, HexNeighborsForEvenRowCenter) {
    // Validates even-row hex neighbor topology (offset pattern differs by row parity).
    Board board(8, 8, 4);
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

TEST(BoardTest, HexNeighborsForOddRowCenter) {
    // Validates odd-row hex neighbor topology.
    Board board(8, 8, 4);
    const auto neighbors = board.hexNeighbors(5, 4); // odd row

    EXPECT_EQ(neighbors.size(), 6u);
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(4, 4)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(4, 5)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(5, 3)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(5, 5)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(6, 4)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(6, 5)), neighbors.end());
}

TEST(BoardTest, HexNeighborsAtTopLeftCorner) {
    // Corner cells should only return in-bounds neighbors.
    Board board(8, 8, 4);
    const auto neighbors = board.hexNeighbors(0, 0);

    EXPECT_EQ(neighbors.size(), 2u);
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(0, 1)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(1, 0)), neighbors.end());
}

TEST(BoardTest, ClearConnectedGroupReturnsZeroForEmptyStartCell) {
    // Clearing should be a no-op when starting on an empty cell.
    Board board(8, 8, 4);
    EXPECT_EQ(board.clearConnectedGroup(7, 7, 3), 0);
}

TEST(BoardTest, ClearConnectedGroupRespectsMinimumSize) {
    // A connected component smaller than threshold must remain unchanged.
    Board board(8, 8, 4);
    board.set(7, 0, BubbleColor::Green);
    board.set(7, 1, BubbleColor::Green);

    EXPECT_EQ(board.clearConnectedGroup(7, 0, 3), 0);
    EXPECT_EQ(board.get(7, 0), BubbleColor::Green);
    EXPECT_EQ(board.get(7, 1), BubbleColor::Green);
}

TEST(BoardTest, ClearConnectedGroupClearsWhenMinimumReached) {
    // A connected component meeting threshold should be removed and counted.
    Board board(8, 8, 4);
    board.set(6, 3, BubbleColor::Yellow);
    board.set(6, 4, BubbleColor::Yellow);
    board.set(7, 3, BubbleColor::Yellow);

    EXPECT_EQ(board.clearConnectedGroup(6, 3, 3), 3);
    EXPECT_TRUE(board.isEmpty(6, 3));
    EXPECT_TRUE(board.isEmpty(6, 4));
    EXPECT_TRUE(board.isEmpty(7, 3));
}

TEST(BoardTest, OutOfRangeIndices) {
    // Public accessors should enforce index safety through exceptions.
    Board board(8, 8, 4);

    EXPECT_THROW(board.get(8, 0), std::out_of_range);
    EXPECT_THROW(board.get(0, 8), std::out_of_range);
    EXPECT_THROW(board.set(8, 0, BubbleColor::Red), std::out_of_range);
    EXPECT_THROW(board.set(0, 8, BubbleColor::Red), std::out_of_range);
}