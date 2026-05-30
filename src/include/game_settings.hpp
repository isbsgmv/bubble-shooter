#include <iostream>

namespace GameSettings
{
    // Configurable game settings (modifiable at runtime via a menu)
    inline std::size_t Rows = 16;
    inline std::size_t Cols = 16;
    inline std::size_t MissesBeforeNewRow = 3;
    inline std::size_t MinGroupSize = 3;
} // GameSettings