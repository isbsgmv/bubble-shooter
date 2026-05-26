#include <gtest/gtest.h>
#include "projectile.hpp"
#include "board.hpp"

TEST(ProjectileTest, ShootInvalidAngle) {
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    auto result = Projectile::shoot(board, 0.0, Bubble::Color::Red);
    EXPECT_FALSE(result.has_value());
}

// Add more tests as needed for projectile logic