#include "sdl2_renderer.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "defaults.hpp"

SDL2Renderer::SDL2Renderer() = default;

SDL2Renderer::~SDL2Renderer()
{
    shutdown();
}

bool SDL2Renderer::init(int windowWidth, int windowHeight, const char *title)
{
    m_hexSize = std::min(windowWidth / (GameSettings::Cols * 1.5), windowHeight / (GameSettings::Rows * 1.5));
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    m_window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!m_window)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    m_renderer = SDL_CreateRenderer(
        m_window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!m_renderer)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    // Set renderer color and blend mode
    SDL_SetRenderDrawColor(m_renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g,
                           BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    calculateGridLayout();
    m_isRunning = true;

    return true;
}

void SDL2Renderer::colsRows(size_t rows, size_t cols)
{
    m_rows = rows;
    m_cols = cols;

    double launcher_grid_x = (static_cast<double>(cols) - 1.0) / 2.0,
           launcher_grid_y = static_cast<double>(rows) - 1.0;
    hexGridToScreenCoord(launcher_grid_y, launcher_grid_x, m_launcherX, m_launcherY);
}

void SDL2Renderer::shutdown()
{
    if (m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit();
}

void SDL2Renderer::clear()
{
    if (!m_renderer)
        return;
    SDL_SetRenderDrawColor(m_renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g,
                           BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
    SDL_RenderClear(m_renderer);
}

void SDL2Renderer::present()
{
    if (m_renderer)
    {
        SDL_RenderPresent(m_renderer);
    }
}

void SDL2Renderer::calculateGridLayout()
{
    // Assuming 8x8 board (from Game constants)
    int hexWidth = static_cast<int>(m_hexSize); // Width of hexagon
    int hexHeight = m_hexSize;                  // Height of hexagon

    // Calculate grid dimensions
    m_gridWidth = hexWidth * GameSettings::Cols;   // Account for hex packing
    m_gridHeight = hexHeight * GameSettings::Rows; // Vertical packing

    // Center grid on screen
    m_gridStartX = (m_windowWidth - m_gridWidth) / 2;
    m_gridStartY = (m_windowHeight - m_gridHeight) / 3; // Place upper than center
}

void SDL2Renderer::hexGridToScreenCoord(const int &gridRow, const int &gridCol, int &screenX, int &screenY) const
{
    int hexWidth = m_hexSize * 1.1;
    int hexHeight = m_hexSize * 2;

    // Apply hexagonal offset
    screenX = m_gridStartX + gridCol * hexWidth + (gridRow % 2) * hexWidth / 2;
    screenY = m_gridStartY + gridRow * hexHeight / 2.0;
}

void SDL2Renderer::gridToScreenCoord(const double &gridRow, const double &gridCol, int &screenX, int &screenY) const
{
    int hexWidth = m_hexSize * 1.1;
    int hexHeight = m_hexSize * 2;

    // Apply hexagonal offset
    screenX = m_gridStartX + gridCol * hexWidth;
    screenY = m_gridStartY + gridRow * hexHeight/2.0;
}

SDL_Color SDL2Renderer::bubbleColorToSDL(Bubble::Color color) const
{
    switch (color)
    {
    case Bubble::Color::Red:
        return {255, 80, 80, 255};
    case Bubble::Color::Green:
        return {80, 255, 80, 255};
    case Bubble::Color::Blue:
        return {80, 120, 255, 255};
    case Bubble::Color::Yellow:
        return {255, 255, 80, 255};
    case Bubble::Color::Purple:
        return {200, 80, 255, 255};
    case Bubble::Color::None:
    default:
        return {0, 0, 0, 0}; // black for empty
    }
}

void SDL2Renderer::drawCircle(int centerX, int centerY, int radius, const SDL_Color &color, bool filled)
{
    if (!m_renderer)
        return;

    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

    if (filled)
    {
        // Draw filled circle using Midpoint Circle Algorithm
        for (int w = 0; w < radius * 2; w++)
        {
            for (int h = 0; h < radius * 2; h++)
            {
                int dx = w - radius;
                int dy = h - radius;
                if ((dx * dx + dy * dy) <= (radius * radius))
                {
                    SDL_RenderDrawPoint(m_renderer, centerX + dx, centerY + dy);
                }
            }
        }
    }
    else
    {
        // Draw circle outline
        int x = 0;
        int y = radius;
        int d = 3 - 2 * radius;

        while (x <= y)
        {
            SDL_RenderDrawPoint(m_renderer, centerX + x, centerY + y);
            SDL_RenderDrawPoint(m_renderer, centerX - x, centerY + y);
            SDL_RenderDrawPoint(m_renderer, centerX + x, centerY - y);
            SDL_RenderDrawPoint(m_renderer, centerX - x, centerY - y);
            SDL_RenderDrawPoint(m_renderer, centerX + y, centerY + x);
            SDL_RenderDrawPoint(m_renderer, centerX - y, centerY + x);
            SDL_RenderDrawPoint(m_renderer, centerX + y, centerY - x);
            SDL_RenderDrawPoint(m_renderer, centerX - y, centerY - x);

            if (d < 0)
            {
                d = d + 4 * x + 6;
            }
            else
            {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
}

void SDL2Renderer::drawLine(double x1, double y1, double x2, double y2, const SDL_Color &color)
{
    if (!m_renderer)
        return;
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(m_renderer, static_cast<int>(x1), static_cast<int>(y1),
                       static_cast<int>(x2), static_cast<int>(y2));
}

void SDL2Renderer::drawRect(int x, int y, int w, int h, const SDL_Color &color, bool filled)
{
    if (!m_renderer)
        return;
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    if (filled)
    {
        SDL_RenderFillRect(m_renderer, &rect);
    }
    else
    {
        SDL_RenderDrawRect(m_renderer, &rect);
    }
}

void SDL2Renderer::render(const Board &board, const RenderStats &stats)
{
    clear();

    // Draw game board
    bool parityOffset = board.getParityOffset();
    for (std::size_t row = 0; row < m_rows; ++row)
    {
        for (std::size_t col = 0; col < m_cols; ++col)
        {
            int screenX, screenY;
            hexGridToScreenCoord(row + parityOffset, col, screenX, screenY);

            Bubble::Color bubbleColor = board.get(row, col);
            SDL_Color color = bubbleColorToSDL(bubbleColor);
            drawCircle(screenX, screenY, m_hexSize / std::sqrt(3), color, true);
        }
    }


    // Draw launcher indicator
    SDL_Color next_color = bubbleColorToSDL(stats.projectileColor);
    int screenX, screenY;
    gridToScreenCoord(stats.y, stats.x, screenX, screenY);
    drawCircle(screenX, screenY, m_hexSize / std::sqrt(3), next_color, true);

    //Draw next color preview
    SDL_Color next_color_preview = bubbleColorToSDL(stats.nextColor);
    gridToScreenCoord(stats.y, stats.x + m_cols/2, screenX, screenY);
    drawCircle(screenX, screenY, m_hexSize / std::sqrt(3), next_color_preview, true);

    // Draw aiming line if mouse is over game area
    if (m_mouseX > m_gridStartX && m_mouseX < m_gridStartX + m_gridWidth &&
        m_mouseY > 0 && m_mouseY < m_windowHeight)
    {
        renderAimingLine(calculateAngleFromMouse());
    }

    present();
}

double SDL2Renderer::calculateAngleFromMouse() const
{
    int dx = m_mouseX - m_launcherX;
    int dy = m_mouseY - m_launcherY;

    // Calculate angle in degrees (0° is straight up)
    double radians = std::atan2(dx, -dy);
    double degrees = radians * 180.0 / M_PI;

    return degrees;
}

std::optional<double> SDL2Renderer::getMouseAngle() const
{
    double angle = calculateAngleFromMouse();
    // Only return angle if within valid range (-90 to 90 degrees)
    if (angle >= -90 && angle <= 90)
    {
        return angle;
    }
    return std::nullopt;
}

void SDL2Renderer::renderAimingLine(double angle)
{
    if (!m_renderer)
        return;

    // Convert angle to radians (0° is up, increases clockwise)
    double radians = angle * M_PI / 180.0;

    // bound the angle to prevent shooting backwards
    if (radians < -M_PI / 2)
    {
        radians = -M_PI / 2;
    }
    else if (radians > M_PI / 2)
    {
        radians = M_PI / 2;
    }

    // Calculate end point of aiming line
    int length = 100;
    double endX = m_launcherX + length * std::sin(radians);
    double endY = m_launcherY - length * std::cos(radians);

    // Draw aiming line
    drawLine(m_launcherX, m_launcherY, endX, endY, AIMING_LINE_COLOR);
}

void SDL2Renderer::renderProjectile(const ProjectileInfo &trajectory)
{
    if (!trajectory.isValid || !m_renderer)
        return;

    // Draw trajectory line
    drawLine(trajectory.startX, trajectory.startY, trajectory.endX, trajectory.endY,
             {200, 100, 100, 128});

    // Draw projectile at final position
    SDL_SetRenderDrawColor(m_renderer, 200, 100, 100, 255);
    drawCircle(static_cast<int>(trajectory.endX), static_cast<int>(trajectory.endY),
               m_hexSize / 3, {200, 100, 100, 255}, true);
}

void SDL2Renderer::pollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            m_isRunning = false;
            break;
        case SDL_MOUSEMOTION:
            m_mouseX = event.motion.x;
            m_mouseY = event.motion.y;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                auto angle = getMouseAngle();
                if (angle.has_value())
                {
                    m_shouldShoot = true;
                }
            }
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                m_isRunning = false;
            }
            break;
        }
    }
}

bool SDL2Renderer::isRunning() const
{
    return m_isRunning;
}

void SDL2Renderer::requestClose()
{
    m_isRunning = false;
}

bool SDL2Renderer::shouldShoot() const
{
    return m_shouldShoot;
}

void SDL2Renderer::resetShootFlag()
{
    m_shouldShoot = false;
}
