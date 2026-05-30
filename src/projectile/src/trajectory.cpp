#include "trajectory.hpp"

#include "geometry.hpp"
#include <iostream>
#include <cmath>

namespace
{
    constexpr double kStep = 0.5;
    constexpr int kMaxBounces = 16;
} // namespace

std::optional<ProjectileState> ProjectileTrajectory::start(const Board &board, double angleDegrees)
{
    const double radians = ProjectileGeometry::toRadians(angleDegrees);
    const double vy = -std::cos(radians);
    const double vx = std::sin(radians);
    // Only allow shots with an upward (negative y) velocity; reject horizontal or downward shots.
    if (vy >= 1e-8)
    {
        return std::nullopt;
    }
    ProjectileState state{};
    state.x = (static_cast<double>(board.cols()) - 1.0) / 2.0;
    state.y = static_cast<double>(board.rows()) - 1.0;
    state.vx = vx;
    state.vy = vy;
    state.bounceCount = 0;
    return state;
}

bool ProjectileTrajectory::advance(const Board &board, ProjectileState &state)
{
    state.x += state.vx * kStep;
    state.y += state.vy * kStep;

    while (state.x < 0.0 || state.x > static_cast<double>(board.cols() - 1))
    {
        if (state.x < 0.0)
        {
            state.x = -state.x;
            state.vx = -state.vx;
            ++state.bounceCount;
        }
        else if (state.x > static_cast<double>(board.cols() - 1))
        {
            const double maxX = static_cast<double>(board.cols() - 1);
            state.x = 2.0 * maxX - state.x;
            state.vx = -state.vx;
            ++state.bounceCount;
        }

        if (state.bounceCount > kMaxBounces)
        {
            return false;
        }
    }

    return true;
}
