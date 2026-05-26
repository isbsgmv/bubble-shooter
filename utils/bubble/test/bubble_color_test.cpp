#include <gtest/gtest.h>
#include "bubble_color.hpp"

// This test uses friend access to inspect private m_counts directly
TEST(ColorManagerTest, FriendAccess) {
    Bubble::ColorManager mgr;
    // All color counts should be zero at start
    for (int c = Bubble::Red; c <= Bubble::Purple; ++c) {
        auto color = static_cast<Bubble::Color>(c);
        EXPECT_EQ(mgr.getColorCount(color), 0u);
    }

    // Add some colors
    auto c1 = mgr.randomColor();
    auto c2 = mgr.randomColor();
    EXPECT_EQ(mgr.getColorCount(c1) + mgr.getColorCount(c2), 2u);

    // Remove one
    mgr.removeColor(c1);
    EXPECT_EQ(mgr.getColorCount(c1), 0u);
}
