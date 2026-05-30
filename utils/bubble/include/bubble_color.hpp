/**
 * @file bubble_color.hpp
 * @brief Bubble color enumeration and color management system.
 *
 * Defines available bubble colors and provides a ColorManager for tracking
 * and distributing colors during gameplay.
 */
#pragma once
#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include <random>

namespace Bubble
{
    /**
     * @brief Enumeration of valid bubble colors.
     *
     * None represents an empty cell, while Red through Purple are playable colors.
     */
    enum Color : std::uint8_t
    {
        None,    ///< Empty cell (no bubble)
        Red,     ///< Red bubble
        Green,   ///< Green bubble
        Blue,    ///< Blue bubble
        Yellow,  ///< Yellow bubble
        Purple   ///< Purple bubble
    };


    /**
     * @brief Manages available bubble colors and their inventory counts.
     *
     * Tracks how many bubbles of each color are available and provides
     * methods to retrieve colors for game initialization and projectiles.
     */
    class ColorManager
    {
    public:
        /**
         * @brief Constructs a new ColorManager with initial color counts.
         *
         * Initializes the color distribution and random number generator
         * for color selection.
         */
        ColorManager();

        /**
         * @brief Retrieves a color from the available pool (deprecated method).
         *
         * @return The selected color.
         *
         * @note Consider using randomColor() instead for more predictable behavior.
         */
        Color getColor();

        /**
         * @brief Removes a specific color from the inventory.
         *
         * Decrements the count of the given color, typically called when
         * a bubble of that color is placed on the board.
         *
         * @param color Reference to the color to decrement. Value is unchanged.
         */
        void removeColor(Color &color);

        /**
         * @brief Selects a random color from the available set.
         *
         * Uses the internal random number generator to select one of the
         * available colors uniformly at random.
         *
         * @return A randomly selected color from the available colors.
         */
        Color randomColor();

        /**
         * @brief Queries the current inventory count of a specific color.
         *
         * @param color The color to query.
         * @return The number of bubbles of the specified color available.
         */
        size_t getColorCount(Color color) const;

    private:
        /// Tracks the inventory count for each color.
        std::unordered_map<Color, std::size_t> m_counts;
        /// Total number of distinct colors (excluding None).
        std::size_t m_n_colors{Color::Purple + 1};
        /// Random number generator for color selection.
        std::mt19937 m_rng;
    };

}
