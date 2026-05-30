#include "projectile.hpp"

#include "collision.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace
{
constexpr int kMaxSteps = 12000;
} // namespace

std::optional<std::pair<std::size_t, std::size_t>> Projectile::shoot(
    Board &board,
    double angleDegrees,
    Bubble::Color color,
    SDL2Renderer *renderer )
{
           
    auto state = ProjectileTrajectory::start(board, angleDegrees);
    std::cout << "Initial projectile state: " << (state.has_value() ? "valid" : "invalid") << "\n";
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
        
        RenderStats stats{
            state->x, state->y,
            color};
        renderer->render(board, stats);
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Adjust for smoother animation

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
