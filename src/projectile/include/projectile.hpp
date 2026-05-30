/**
 * @file projectile.hpp
 * @brief High-level projectile shooting interface for the bubble shooter gameplay.
 *
 * Provides a unified API for launching projectiles that integrates trajectory simulation,
 * collision detection, and bubble attachment. This is the main entry point for projectile
 * mechanics called by the game logic.
 */
#pragma once

#include "board.hpp"
#include "bubble/include/bubble_color.hpp"

#include "trajectory.hpp"
#include <cstddef>
#include <optional>
#include <utility>

/**
 * @brief High-level projectile shooting and attachment mechanics.
 *
 * Encapsulates the complete projectile lifecycle:
 * 1. Create initial projectile state from launch angle
 * 2. Simulate trajectory frame-by-frame with wall bounces
 * 3. Detect collision with board bubbles
 * 4. Place the projectile's bubble at an appropriate attachment location
 * 5. Return the position where the bubble was placed
 *
 * Optionally provides trajectory visualization via SDL2Renderer for debugging/aiming preview.
 */
class Projectile
{
public:
    /**
     * @brief Launches a projectile and attempts to attach it to the board.
     *
     * Complete shooting sequence:
     * - Initializes projectile with velocity based on launch angle
     * - Advances projectile state frame-by-frame until collision or board boundary
     * - Detects which bubble (if any) was hit
     * - Finds appropriate attachment position (ceiling or near hit bubble)
     * - Places the bubble on the board and triggers match detection
     *
     * The projectile is a static method, so multiple projectiles can be shot in sequence.
     *
     * @param board The game board. This is modified to add the new bubble and clear matches.
     * @param angleDegrees The launch angle in degrees. Typically 0° to 180° for playable angles.
     * @param color The color of the projectile bubble to launch.
     * @param renderer Optional SDL2 renderer for trajectory visualization during development.
     *                 If provided, renders the projectile's path for debugging.
     *
     * @return Optional pair (row, col) where the bubble was successfully attached,
     *         or std::nullopt if the projectile failed to attach (all neighbors occupied,
     *         or other attachment failure).
     */
    static std::optional<std::pair<std::size_t, std::size_t>> shoot(
        Board &board,
        double angleDegrees,
        Bubble::Color color,
        SDL2Renderer *renderer = nullptr);
};
