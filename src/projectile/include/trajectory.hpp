/**
 * @file trajectory.hpp
 * @brief Projectile trajectory simulation and physics calculations.
 *
 * Handles projectile state management, initial velocity calculation from launch angles,
 * step-by-step trajectory advancement with wall collision handling, and bounce mechanics.
 */
#pragma once

#include "board.hpp"
#include "sdl2_renderer.hpp"
#include <optional>

/**
 * @brief Represents the dynamic state of a projectile in flight.
 *
 * Tracks position, velocity, and collision history of a projectile as it
 * travels through the game world. Updated each game frame by ProjectileTrajectory::advance().
 */
struct ProjectileState
{
    double x;               ///< Current x position in game coordinates.
    double y;               ///< Current y position in game coordinates.
    double vx;              ///< Current x velocity in game units/frame.
    double vy;              ///< Current y velocity in game units/frame.
    int bounceCount;        ///< Number of wall collisions encountered so far.
};

/**
 * @brief Manages projectile physics simulation and trajectory calculation.
 *
 * Provides static methods for computing initial projectile velocities from a launch angle
 * and for advancing the projectile state frame-by-frame with wall bounce physics.
 * Works on the game's logical coordinate system independent of rendering.
 */
class ProjectileTrajectory
{
public:
    /**
     * @brief Computes the initial projectile state for a given launch angle.
     *
     * Creates a ProjectileState positioned at the launcher with velocity
     * components calculated from the launch angle and a fixed speed magnitude.
     *
     * @param board The game board (used to determine launcher position).
     * @param angleDegrees The launch angle in degrees (0° = right, 90° = up, etc.).
     * @return Optional containing initial ProjectileState, or std::nullopt if angle is invalid.
     */
    static std::optional<ProjectileState> start(const Board &board, double angleDegrees);

    /**
     * @brief Advances the projectile state by one frame with physics and collisions.
     *
     * Updates position based on velocity, handles wall bounces by reversing appropriate
     * velocity components, and updates the bounce counter. Stops simulation if max bounces exceeded.
     *
     * @param board The game board (used for boundary checking).
     * @param state The projectile state to update in-place. Position and velocity are modified.
     * @return True if projectile remains in play, false if max bounces exceeded and simulation should stop.
     */
    static bool advance(const Board &board, ProjectileState &state);
};
