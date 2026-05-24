#include <gtest/gtest.h>
#include "projectile.hpp"
#include "board.hpp"

TEST(ProjectileTest, ShootInvalidAngle) {
    Board board(8, 8, 4);
    auto result = Projectile::shoot(board, 0.0, BubbleColor::Red);
    EXPECT_FALSE(result.has_value());
}

// Add more tests as needed for projectile logic