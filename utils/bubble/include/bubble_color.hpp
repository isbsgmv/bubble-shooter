#pragma once
#include <cstdint>
#include <cstddef>
#include <unordered_map>

namespace Bubble
{


    /**
     * @brief Enum representing possible bubble colors.
     */
    enum Color : std::uint8_t
    {
        None,
        Red,
        Green,
        Blue,
        Yellow,
        Purple
    };


    /**
     * @brief Manages available bubble colors and their counts.
     */
    class ColorManager
    {
    public:
        /**
         * @brief Construct a new ColorManager object.
         */
        ColorManager();

        /**
         * @brief Get a color from the available colors.
         * @return Color The selected color.
         */
        Color getColor();

        /**
         * @brief Remove a color from the available set.
         * @param color The color to remove.
         */
        void removeColor(Color &color);

        /**
         * @brief Get a random color from the available set.
         * @return Color The randomly selected color.
         */
        Color randomColor();

        /**
         * @brief Get the count of a specific color.
         * @param color The color to query.
         * @return size_t The count of the color.
         */
        size_t getColorCount(Color color) const;
    private:
    
        std::unordered_map<Color, std::size_t> m_counts;
        std::size_t m_n_colors{Color::Purple + 1};
    };

}
