#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <memory>
#include <iostream>

#include "Game.h"
#include "MenuScreen.h"
#include "GameScreen.h"

using namespace sfSnake;

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 8.f);

std::shared_ptr<Screen> Game::Screen = nullptr;

// 初始化静态成员
BackgroundColor Game::globalBgColor_ = BackgroundColor::White;
GridColor Game::globalGridColor_ = GridColor::Black;
bool Game::globalGridVisible_ = true;

Game::Game()
: window_(sf::VideoMode(Game::Width, Game::Height), "sfSnake"),
  bgColor_(BackgroundColor::White),
  gridColor_(GridColor::Black),
  gridVisible_(true)
{
    if (!bgMusic_.openFromFile("assets/sounds/bg_music.wav")) {
        std::cerr << "无法加载背景音乐!" << std::endl;
    }
    bgMusic_.setLoop(true);
    bgMusic_.play();

    Game::Screen = std::make_shared<MenuScreen>();
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (window_.isOpen())
    {
        sf::Time delta = clock.restart();
        timeSinceLastUpdate += delta;

        while (timeSinceLastUpdate > Game::TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            
            // 应用全局设置
            setBackgroundColor(globalBgColor_);
            setGridColor(globalGridColor_);
            setGridVisible(globalGridVisible_);
            
            handleInput();
            update(TimePerFrame);
        }

        render();
    }
}

void Game::handleInput()
{
    sf::Event event;
    
    while (window_.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window_.close();
    }

    Game::Screen->handleInput(window_);
}

void Game::update(sf::Time delta)
{
    Game::Screen->update(delta);
}

void Game::render()
{
    window_.clear(getActualBackgroundColor());
    
    Game::Screen->render(window_);
    
    window_.display();
}

void Game::setBackgroundColor(BackgroundColor color)
{
    bgColor_ = color;
}

BackgroundColor Game::getBackgroundColor() const
{
    return bgColor_;
}

void Game::setGridColor(GridColor color)
{
    gridColor_ = color;
}

GridColor Game::getGridColor() const
{
    return gridColor_;
}

void Game::setGridVisible(bool visible)
{
    gridVisible_ = visible;
}

bool Game::isGridVisible() const
{
    return gridVisible_;
}

sf::Color Game::getActualBackgroundColor() const
{
    switch (bgColor_)
    {
        case BackgroundColor::White:
            return sf::Color(240, 240, 240); // 白色
        case BackgroundColor::Black:
            return sf::Color(40, 40, 40);    // 黑色
        case BackgroundColor::Brown:
            return sf::Color(139, 69, 19);   // 棕色
        default:
            return sf::Color::White;
    }
}

sf::Color Game::getActualGridColor() const
{
    switch (gridColor_)
    {
        case GridColor::White:
            return sf::Color(255, 255, 255, 50); // 半透明白色
        case GridColor::Black:
            return sf::Color(0, 0, 0, 50);       // 半透明黑色
        case GridColor::Brown:
            return sf::Color(139, 69, 19, 50);   // 半透明棕色
        default:
            return sf::Color(100, 100, 100, 50);
    }
}

// 实现静态方法
void Game::setGlobalBackgroundColor(BackgroundColor color)
{
    globalBgColor_ = color;
}

BackgroundColor Game::getGlobalBackgroundColor()
{
    return globalBgColor_;
}

void Game::setGlobalGridColor(GridColor color)
{
    globalGridColor_ = color;
}

GridColor Game::getGlobalGridColor()
{
    return globalGridColor_;
}

void Game::setGlobalGridVisible(bool visible)
{
    globalGridVisible_ = visible;
}

bool Game::isGlobalGridVisible()
{
    return globalGridVisible_;
} 