#include "bubble_color.hpp"
namespace Bubble {

ColorManager::ColorManager(): m_rng(std::random_device{}())
{
    m_counts.reserve(m_n_colors - 1);
    // Initialize all color counts to 0.
    for (std::size_t i = 1; i < m_n_colors; ++i)
    {
        m_counts[static_cast<Color>(i)] = 0;
    }
}

Color ColorManager::getColor()
{
    // Pick a random color candidate.
    Color new_color = randomColor();
    // Keep rolling until a color not currently tracked in m_counts is found.
    while (m_counts.at(new_color) <= 1)
    { 
        m_counts.at(new_color)--;
        new_color = randomColor();
    }

    return new_color;
}

void ColorManager::removeColor(Color &color)
{
    m_counts[color] --;
    color = Color::None;
}

Color ColorManager::randomColor()
{
    
    double r = std::generate_canonical<double, 32>(m_rng);
    Color new_color = static_cast<Color>(1 + static_cast<int>(r * (m_n_colors - 1)));
    // Return the first available color.
    m_counts[new_color] ++;
    return new_color;
}

size_t ColorManager::getColorCount(Color color) const
{
    auto it = m_counts.find(color);
    if (it != m_counts.end()) {
        return it->second;
    }
    return 0;
}
} // namespace Bubble