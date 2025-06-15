#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <memory>

#include "Screen.h"

namespace sfSnake
{
enum class BackgroundColor
{
    White, Black, Brown
};

enum class GridColor
{
    White, Black, Brown
};

class Game
{
public:
    Game();

    void run();

    void handleInput();
    void update(sf::Time delta);
    void render();
    
    void setBackgroundColor(BackgroundColor color);
    BackgroundColor getBackgroundColor() const;
    
    void setGridColor(GridColor color);
    GridColor getGridColor() const;
    void setGridVisible(bool visible);
    bool isGridVisible() const;

    static const int Width = 640;
    static const int Height = 480;

    static std::shared_ptr<Screen> Screen;
    
    static void setGlobalBackgroundColor(BackgroundColor color);
    static BackgroundColor getGlobalBackgroundColor();
    static void setGlobalGridColor(GridColor color);
    static GridColor getGlobalGridColor();
    static void setGlobalGridVisible(bool visible);
    static bool isGlobalGridVisible();

private:
    sf::RenderWindow window_;
    sf::Music bgMusic_;
    
    BackgroundColor bgColor_;
    GridColor gridColor_;
    bool gridVisible_;
    
    sf::Color getActualBackgroundColor() const;
    sf::Color getActualGridColor() const;
    
    // 恢复固定的时间步长
    static const sf::Time TimePerFrame;
    
    static BackgroundColor globalBgColor_;
    static GridColor globalGridColor_;
    static bool globalGridVisible_;
};
}

#endif