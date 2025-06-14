#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <memory>

#include "Screen.h"

namespace sfSnake
{
// 定义背景颜色选项
enum class BackgroundColor
{
    White, Black, Brown
};

// 定义网格颜色选项
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
    
    // 设置背景颜色
    void setBackgroundColor(BackgroundColor color);
    BackgroundColor getBackgroundColor() const;
    
    // 设置网格颜色和可见性
    void setGridColor(GridColor color);
    GridColor getGridColor() const;
    void setGridVisible(bool visible);
    bool isGridVisible() const;

    static const int Width = 640;
    static const int Height = 480;

    static std::shared_ptr<Screen> Screen;
    
    // 静态方法，用于访问和修改全局设置
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
    
    // 获取实际颜色
    sf::Color getActualBackgroundColor() const;
    sf::Color getActualGridColor() const;
    
    static const sf::Time TimePerFrame;
    
    // 静态成员，用于存储全局设置
    static BackgroundColor globalBgColor_;
    static GridColor globalGridColor_;
    static bool globalGridVisible_;
};
}

#endif 