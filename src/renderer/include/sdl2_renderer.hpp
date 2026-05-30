/**
 * @file sdl2_renderer.hpp
 * @brief SDL2-based graphics renderer and input handler for the bubble shooter game.
 *
 * Manages all visual rendering using SDL2 including the hexagonal grid, bubbles,
 * projectiles, and aiming indicators. Also handles window management and mouse input
 * for gameplay control.
 */
#ifndef SDL2_RENDERER_H
#define SDL2_RENDERER_H

#include "board.hpp"
#include "bubble/include/bubble_color.hpp"

#include <SDL2/SDL.h>
#include <memory>
#include <optional>
#include <cmath>

/**
 * @brief Real-time projectile visualization data.
 *
 * Provides the current position and color of an active projectile for rendering.
 */
struct RenderStats {
    double x, y;                           ///< Current projectile position in game coordinates.
    Bubble::Color projectileColor;         ///< Color of the projectile.
    Bubble::Color nextColor{Bubble::Color::None};         ///< Next Color of the projectile.
};

/**
 * @brief Projected trajectory information for aiming preview.
 *
 * Contains the calculated trajectory of a projectile including start position
 * (at launcher) and end position (impact point) for visualization.
 */
struct ProjectileInfo {
    double startX, startY;                 ///< Launch position coordinates.
    double endX, endY;                     ///< Predicted impact position coordinates.
    bool isValid;                          ///< Whether the trajectory calculation succeeded.
};

/**
 * @brief SDL2-based renderer for the bubble shooter game.
 *
 * Handles all rendering operations, coordinate transformations, window/input management,
 * and maintains the mapping between game logic coordinates and screen pixels. Uses a
 * hexagonal grid layout with proper row offset calculations for neighbor relationships.
 */
class SDL2Renderer {
public:
    /**
     * @brief Default constructor for the SDL2 renderer.
     */
    SDL2Renderer();
    /**
     * @brief Destructor that cleans up SDL2 resources and the game window.
     */
    ~SDL2Renderer();

    // ===== Initialization and Resource Management =====
    /**
     * @brief Initializes SDL2 and creates the main game window.
     *
     * Must be called once before any rendering operations. Sets up the renderer,
     * allocates window resources, and initializes internal state.
     *
     * @param windowWidth Desired window width in pixels (e.g., 1200).
     * @param windowHeight Desired window height in pixels (e.g., 800).
     * @param title Window title string displayed in the title bar.
     * @return True if initialization succeeded, false if SDL2 fails.
     */
    bool init(int windowWidth, int windowHeight, const char* title = "Bubble Shooter");
    /**
     * @brief Sets the dimensions of the logical game board.
     *
     * Configures the hexagonal grid layout to fit the specified dimensions.
     * Should be called after init() with the board's row and column counts.
     *
     */
    void initGameSettings();
    /**
     * @brief Shuts down the renderer and releases all SDL2 resources.
     *
     * Must be called before program exit to free SDL2 resources properly.
     */
    void shutdown();

    // ===== Frame Rendering =====
    /**
     * @brief Renders a complete frame with the board state and projectile.
     *
     * Clears the screen and draws all game elements: hexagonal grid, bubbles,
     * launcher, and active projectile.
     *
     * @param board The current game board state containing bubble positions.
     * @param stats Real-time projectile information (position and color).
     */
    void render(const Board& board, const RenderStats& stats);
    /**
     * @brief Renders the aiming line from the launcher.
     *
     * Draws a line from the launcher position outward at the specified angle
     * to show the player where the projectile will be fired.
     *
     * @param angle Launch angle in degrees (0° = right, 90° = up).
     */
    void renderAimingLine(double angle);
    /**
     * @brief Renders the projectile trajectory preview.
     *
     * Draws a line segment from the launch point to the predicted impact point,
     * providing a preview of where the projectile will travel.
     *
     * @param trajectory Calculated trajectory information with start/end positions.
     */
    void renderProjectile(const ProjectileInfo& trajectory);
    /**
     * @brief Clears the rendering surface to background color.
     *
     * Should be called at the start of each frame before drawing.
     */
    void clear();
    /**
     * @brief Presents the rendered frame to the display.
     *
     * Swaps the back buffer with the front buffer, displaying the newly
     * rendered frame. Should be called once per frame after all render calls.
     */
    void present();

    // ===== Event and Input Handling =====
    /**
     * @brief Processes pending SDL2 events (mouse, keyboard, window events).
     *
     * Should be called once per frame to handle user input and window events
     * (resize, close, focus changes, etc.).
     */
    void pollEvents();
    /**
     * @brief Checks if the game window is still open and running.
     *
     * @return True if the window is open, false if close was requested or
     *         the user clicked the window close button.
     */
    bool isRunning() const;
    /**
     * @brief Requests that the game window close on the next frame.
     *
     * Sets an internal flag that causes isRunning() to return false.
     */
    void requestClose();

    // ===== Input Queries =====
    /**
     * @brief Calculates the launch angle based on current mouse position.
     *
     * Computes the angle from the launcher to the current mouse cursor.
     * Restricts angle to valid shooting range (typically 0° to 180°).
     *
     * @return Optional angle in degrees, or std::nullopt if position is invalid.
     */
    std::optional<double> getMouseAngle() const;
    /**
     * @brief Checks if the player has triggered a shoot action.
     *
     * Returns true once per mouse click, then must be reset with resetShootFlag().
     *
     * @return True if a shoot was requested this frame, false otherwise.
     */
    bool shouldShoot() const;
    /**
     * @brief Resets the shoot flag after the current shot has been processed.
     *
     * Must be called by the game logic after handling a shoot request
     * to prevent repeated shooting on the same click.
     */
    void resetShootFlag();

    // ===== Window Properties =====
    /**
     * @brief Gets the current window width in pixels.
     * @return Window width as set by init().
     */
    int getWindowWidth() const { return m_windowWidth; }
    /**
     * @brief Gets the current window height in pixels.
     * @return Window height as set by init().
     */
    int getWindowHeight() const { return m_windowHeight; }

private:
    // ===== Private Rendering Helpers =====
    /**
     * @brief Draws a line segment between two points in screen coordinates.
     * @param x1 Start point x coordinate.
     * @param y1 Start point y coordinate.
     * @param x2 End point x coordinate.
     * @param y2 End point y coordinate.
     * @param color SDL color for the line.
     */
    void drawLine(double x1, double y1, double x2, double y2, const SDL_Color& color);
    /**
     * @brief Draws a circle at the specified screen position.
     * @param centerX Circle center x coordinate in pixels.
     * @param centerY Circle center y coordinate in pixels.
     * @param radius Radius of the circle in pixels.
     * @param color SDL color for the circle.
     * @param filled If true, draws filled circle; if false, draws outline only.
     */
    void drawCircle(int centerX, int centerY, int radius, const SDL_Color& color, bool filled = true);
    /**
     * @brief Draws a rectangle in screen space.
     * @param x Top-left x coordinate in pixels.
     * @param y Top-left y coordinate in pixels.
     * @param w Width of rectangle in pixels.
     * @param h Height of rectangle in pixels.
     * @param color SDL color for the rectangle.
     * @param filled If true, draws filled rectangle; if false, draws outline.
     */
    void drawRect(int x, int y, int w, int h, const SDL_Color& color, bool filled = true);
    /**
     * @brief Converts a game bubble color to SDL2 color format.
     * @param color The bubble color enumeration value.
     * @return Corresponding SDL_Color with RGB and alpha values.
     */
    SDL_Color bubbleColorToSDL(Bubble::Color color) const;

    // ===== Coordinate Transformation Helpers =====
    /**
     * @brief Recalculates the hexagonal grid layout based on current window size.
     *
     * Computes grid positioning, cell sizes, and launcher position based on
     * the window dimensions. Called during init() and on window resize.
     */
    void calculateGridLayout();
    /**
     * @brief Transforms a screen pixel position to hexagonal grid coordinates.
     *
     * Converts from window coordinates to logical grid coordinates, accounting
     * for the hexagonal layout and row offset pattern.
     *
     * @param screenX Screen x coordinate in pixels.
     * @param screenY Screen y coordinate in pixels.
     * @param gridRow Output grid row index.
     * @param gridCol Output grid column index.
     * @return True if the position is within the grid bounds, false otherwise.
     */
    bool screenToGridCoord(int screenX, int screenY, int& gridRow, int& gridCol) const;
    /**
     * @brief Converts integer hexagonal grid coordinates to screen position.
     *
     * Transforms logical grid coordinates to window pixels, accounting for
     * hexagonal geometry and row offset adjustments.
     *
     * @param gridRow Row index in the game grid.
     * @param gridCol Column index in the game grid.
     * @param screenX Output screen x coordinate in pixels.
     * @param screenY Output screen y coordinate in pixels.
     */
    void hexGridToScreenCoord(const int &gridRow, const int &gridCol, int &screenX, int &screenY) const;
    /**
     * @brief Converts floating-point grid coordinates to screen position.
     *
     * Transforms continuous (floating-point) grid coordinates to screen pixels.
     * Useful for projectile rendering and smooth movement.
     *
     * @param gridRow Row coordinate (can be fractional).
     * @param gridCol Column coordinate (can be fractional).
     * @param screenX Output screen x coordinate in pixels.
     * @param screenY Output screen y coordinate in pixels.
     */
    void gridToScreenCoord(const double &gridRow, const double &gridCol, int &screenX, int &screenY) const;
    /**
     * @brief Computes the launch angle from the launcher to the mouse cursor.
     *
     * Calculates angle in degrees (0° = right, 90° = up) relative to
     * the launcher position and current mouse location.
     *
     * @return Launch angle in degrees.
     */
    double calculateAngleFromMouse() const;

    // ===== SDL2 Resources =====
    SDL_Window* m_window{nullptr};         ///< SDL2 window handle.
    SDL_Renderer* m_renderer{nullptr};     ///< SDL2 renderer for drawing. 
    // ===== Window State =====
    int m_windowWidth{0};                  ///< Window width in pixels.
    int m_windowHeight{0};                 ///< Window height in pixels.

    // ===== Hexagonal Grid Layout =====
    int m_hexSize{};                     ///< Size of each hexagon cell in pixels.
    int m_gridStartX{0};                   ///< Left edge of grid area in window.
    int m_gridStartY{0};                   ///< Top edge of grid area in window.
    int m_gridWidth{0};                    ///< Total width of grid area in pixels.
    int m_gridHeight{0};                   ///< Total height of grid area in pixels.

    // ===== Launcher Configuration =====
    int m_launcherX{0};                    ///< Launcher position x coordinate in pixels.
    int m_launcherY{0};                    ///< Launcher position y coordinate in pixels.

    // ===== Input State =====
    int m_mouseX{0};                       ///< Current mouse x position in window.
    int m_mouseY{0};                       ///< Current mouse y position in window.
    bool m_mousePressed{false};            ///< Whether mouse button is currently held down.
    bool m_shouldShoot{false};             ///< Flag indicating a shot was triggered.
    bool m_isRunning{true};                ///< Whether the window is open and running.

    // ===== Rendering Color Constants =====
    static constexpr SDL_Color BACKGROUND_COLOR = {20, 20, 30, 255};      ///< Dark blue background.
    static constexpr SDL_Color GRID_LINE_COLOR = {50, 50, 70, 255};       ///< Dark purple grid lines.
    static constexpr SDL_Color AIMING_LINE_COLOR = {200, 200, 200, 255};  ///< Light gray aiming indicator.
    static constexpr SDL_Color TEXT_COLOR = {200, 200, 200, 255};         ///< Light gray for UI text.
};

#endif // SDL2_RENDERER_H
