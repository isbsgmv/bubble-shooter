#include "collision.hpp"

#include "geometry.hpp"

#include <cmath>
#include <limits>

std::optional<std::pair<std::size_t, std::size_t>> ProjectileCollision::attachAtCeiling(
    Board &board,
    double x,
    Bubble::Color color)
{
    std::optional<std::size_t> bestCol;
    double bestDistance = std::numeric_limits<double>::max();

    for (std::size_t col = 0; col < board.cols(); ++col)
    {
        if (!board.isEmpty(0, static_cast<int>(col)))
        {
            continue;
        }

        const double distance = std::abs(ProjectileGeometry::cellCenterX(0, col) - x);
        if (distance < bestDistance)
        {
            bestDistance = distance;
            bestCol = col;
        }
    }

    if (!bestCol.has_value())
    {
        return std::nullopt;
    }

    board.set(0, *bestCol, color);
    return std::pair<std::size_t, std::size_t>{0, *bestCol};
}

std::optional<std::pair<int, int>> ProjectileCollision::findHitBubble(const Board &board, double x, double y)
{
    int hitRow = -1;
    int hitCol = -1;
    double hitDistance = std::numeric_limits<double>::max();

    for (std::size_t row = 0; row < board.rows(); ++row)
    {
        for (std::size_t col = 0; col < board.cols(); ++col)
        {
            if (board.get(row, col) == Bubble::Color::None)
            {
                continue;
            }

            const double dx = x - ProjectileGeometry::cellCenterX(row, col);
            const double dy = y - ProjectileGeometry::cellCenterY(row);
            const double distanceSquared = dx * dx + dy * dy;

            if (distanceSquared > (ProjectileGeometry::kCollisionRadius * ProjectileGeometry::kCollisionRadius))
            {
                continue;
            }

            if (distanceSquared < hitDistance)
            {
                hitDistance = distanceSquared;
                hitRow = static_cast<int>(row);
                hitCol = static_cast<int>(col);
            }
        }
    }

    if (hitRow < 0 || hitCol < 0)
    {
        return std::nullopt;
    }

    return std::pair<int, int>{hitRow, hitCol};
}

std::optional<std::pair<std::size_t, std::size_t>> ProjectileCollision::attachNearHit(
    Board &board,
    int hitRow,
    int hitCol,
    double x,
    double y,
    Bubble::Color color)
{
    auto neighbors = board.hexNeighbors(hitRow, hitCol);
    std::optional<std::pair<std::size_t, std::size_t>> bestCell;
    double bestCellDistance = std::numeric_limits<double>::max();

    for (const auto &[nRow, nCol] : neighbors)
    {
        if (!board.isEmpty(nRow, nCol))
        {
            continue;
        }

        const double dx = x - ProjectileGeometry::cellCenterX(static_cast<std::size_t>(nRow), static_cast<std::size_t>(nCol));
        const double dy = y - ProjectileGeometry::cellCenterY(static_cast<std::size_t>(nRow));
        const double distanceSquared = dx * dx + dy * dy;

        if (distanceSquared < bestCellDistance)
        {
            bestCellDistance = distanceSquared;
            bestCell = std::make_pair(static_cast<std::size_t>(nRow), static_cast<std::size_t>(nCol));
        }
    }

    if (!bestCell.has_value())
    {
        return std::nullopt;
    }

    board.set(bestCell->first, bestCell->second, color);
    return bestCell;
}
