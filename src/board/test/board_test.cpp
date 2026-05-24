#include <gtest/gtest.h>
#include "board.hpp"

TEST(BoardTest, Constructor) {
    Board board(8, 8, 4);
    EXPECT_EQ(board.rows(), 8u);
    EXPECT_EQ(board.cols(), 8u);
}

TEST(BoardTest, InBoundsWorks) {
    Board board(8, 8, 4);
    EXPECT_TRUE(board.inBounds(0, 0));
    EXPECT_TRUE(board.inBounds(7, 7));
    EXPECT_FALSE(board.inBounds(-1, 0));
    EXPECT_FALSE(board.inBounds(0, 8));
}

TEST(BoardTest, SetAndGetWorks) {
    Board board(8, 8, 4);
    board.set(2, 3, BubbleColor::Red);
    EXPECT_EQ(board.get(2, 3), BubbleColor::Red);
}