#include "projectile.hpp"

#include "collision.hpp"
#include "trajectory.hpp"

namespace
{
constexpr int kMaxSteps = 12000;
} // namespace

std::optional<std::pair<std::size_t, std::size_t>> Projectile::shoot(
    Board &board,
    double angleDegrees,
    Bubble::Color color)
{
           
    auto state = ProjectileTrajectory::start(board, angleDegrees);
    if (!state.has_value())
    {
        return std::nullopt;
    }

    for (int step = 0; step < kMaxSteps; ++step)
    {
        if (!ProjectileTrajectory::advance(board, *state))
        {
            return std::nullopt;
        }

        if (state->y <= 0.0)
        {
            return ProjectileCollision::attachAtCeiling(board, state->x, color);
        }

        const auto hit = ProjectileCollision::findHitBubble(board, state->x, state->y);
        if (!hit.has_value())
        {
            continue;
        }

        return ProjectileCollision::attachNearHit(
            board,
            hit->first,
            hit->second,
            state->x,
            state->y,
            color);
    }

    return std::nullopt;
}
