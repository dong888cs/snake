#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>

#include "MenuScreen.h"
#include "GameScreen.h"
#include "Game.h"
#include "SettingsScreen.h"

using namespace sfSnake;

MenuScreen::MenuScreen() : showSettings_(false)
{
    // 加载字体
    if (!font_.loadFromFile("assets/fonts/game_over.ttf")) {
        std::cerr << "无法加载字体!" << std::endl;
    }
    
    // 设置标题
    title_.setFont(font_);
    title_.setString("Snake Game");
    title_.setCharacterSize(64);
    title_.setFillColor(sf::Color::Green);
    title_.setOutlineColor(sf::Color(0, 100, 0)); // 深绿色边框
    title_.setOutlineThickness(3.f);
    
    sf::FloatRect titleBounds = title_.getLocalBounds();
    title_.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    title_.setPosition(Game::Width / 2, Game::Height / 4);
    
    // 加载Logo图像
    if (!logoTexture_.loadFromFile("assets/textures/logo.png")) {
        std::cerr << "无法加载Logo图像!" << std::endl;
    } else {
        logoTexture_.setSmooth(true);
        logoSprite_.setTexture(logoTexture_);
        
        // 设置Logo位置和大小，让它充满整个屏幕
        logoSprite_.setPosition(0, 0);
        
        // 计算缩放比例，使logo填满整个窗口
        float scaleX = static_cast<float>(Game::Width) / logoTexture_.getSize().x;
        float scaleY = static_cast<float>(Game::Height) / logoTexture_.getSize().y;
        logoSprite_.setScale(scaleX, scaleY);
    }
    
    // 设置菜单选项
    playText_.setFont(font_);
    playText_.setString("Play");
    playText_.setCharacterSize(32);
    playText_.setFillColor(sf::Color::White);
    
    sf::FloatRect playBounds = playText_.getLocalBounds();
    playText_.setOrigin(playBounds.width / 2, playBounds.height / 2);
    playText_.setPosition(Game::Width / 2, Game::Height / 2);
    
    settingsText_.setFont(font_);
    settingsText_.setString("Settings");
    settingsText_.setCharacterSize(32);
    settingsText_.setFillColor(sf::Color::White);
    
    sf::FloatRect settingsBounds = settingsText_.getLocalBounds();
    settingsText_.setOrigin(settingsBounds.width / 2, settingsBounds.height / 2);
    settingsText_.setPosition(Game::Width / 2, Game::Height / 2 + 50);
    
    exitText_.setFont(font_);
    exitText_.setString("Exit");
    exitText_.setCharacterSize(24);
    exitText_.setFillColor(sf::Color::White);
    
    sf::FloatRect exitBounds = exitText_.getLocalBounds();
    exitText_.setOrigin(exitBounds.width / 2, exitBounds.height / 2);
    exitText_.setPosition(50, 10);
    
    // 设置选项文本
    bgColorText_.setFont(font_);
    bgColorText_.setString("Background: White");
    bgColorText_.setCharacterSize(24);
    bgColorText_.setFillColor(sf::Color::White);
    bgColorText_.setPosition(Game::Width / 2, Game::Height / 2);
    
    gridColorText_.setFont(font_);
    gridColorText_.setString("Grid Color: Black");
    gridColorText_.setCharacterSize(24);
    gridColorText_.setFillColor(sf::Color::White);
    gridColorText_.setPosition(Game::Width / 2 - 150, Game::Height / 2 + 50);
    
    gridVisibilityText_.setFont(font_);
    gridVisibilityText_.setString("Grid: Visible");
    gridVisibilityText_.setCharacterSize(24);
    gridVisibilityText_.setFillColor(sf::Color::White);
    gridVisibilityText_.setPosition(Game::Width / 2 - 150, Game::Height / 2 + 100);
    
    // 设置控制说明
    instructionsText_.setFont(font_);
    instructionsText_.setString(
        "Controls:\n"
        "- Arrow Keys: Move snake\n"
        "- Space: Toggle speed boost\n"
        "- P: Pause game\n"
        "- Mouse: Aim and click to change direction"
    );
    instructionsText_.setCharacterSize(18);
    instructionsText_.setFillColor(sf::Color::Red);
    instructionsText_.setPosition(50, Game::Height / 2 + 100);
}

void MenuScreen::handleInput(sf::RenderWindow& window)
{
    // 添加防抖处理
    static sf::Clock inputClock;
    static bool inputEnabled = true;
    
    if (inputClock.getElapsedTime().asSeconds() > 0.2f) {
        inputEnabled = true;
    }
    
    if (!inputEnabled) {
        return;
    }
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        inputEnabled = false;
        inputClock.restart();
        
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        
        if (!showSettings_)
        {
            // 主菜单
            if (playText_.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
            {
                auto gameScreen = std::make_shared<GameScreen>();
                Game::Screen = gameScreen;
            }
            else if (settingsText_.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
            {
                // 切换到独立的设置界面
                Game::Screen = std::make_shared<SettingsScreen>();
            }
            else if (exitText_.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
            {
                window.close();
            }
        }
        else
        {
            // 设置菜单
            if (bgColorText_.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
            {
                // 切换背景颜色
                BackgroundColor currentColor = Game::getGlobalBackgroundColor();
                
                switch (currentColor)
                {
                    case BackgroundColor::White:
                        Game::setGlobalBackgroundColor(BackgroundColor::Black);
                        bgColorText_.setString("Background: Black");
                        break;
                    case BackgroundColor::Black:
                        Game::setGlobalBackgroundColor(BackgroundColor::Brown);
                        bgColorText_.setString("Background: Brown");
                        break;
                    case BackgroundColor::Brown:
                        Game::setGlobalBackgroundColor(BackgroundColor::White);
                        bgColorText_.setString("Background: White");
                        break;
                }
            }
            else if (gridColorText_.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
            {
                // 切换网格颜色
                GridColor currentColor = Game::getGlobalGridColor();
                
                switch (currentColor)
                {
                    case GridColor::White:
                        Game::setGlobalGridColor(GridColor::Black);
                        gridColorText_.setString("Grid Color: Black");
                        break;
                    case GridColor::Black:
                        Game::setGlobalGridColor(GridColor::Brown);
                        gridColorText_.setString("Grid Color: Brown");
                        break;
                    case GridColor::Brown:
                        Game::setGlobalGridColor(GridColor::White);
                        gridColorText_.setString("Grid Color: White");
                        break;
                }
            }
            else if (gridVisibilityText_.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
            {
                // 切换网格可见性
                bool visible = Game::isGlobalGridVisible();
                
                Game::setGlobalGridVisible(!visible);
                
                if (visible)
                    gridVisibilityText_.setString("Grid: Hidden");
                else
                    gridVisibilityText_.setString("Grid: Visible");
            }
            else if (exitText_.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
            {
                // 返回主菜单
                showSettings_ = false;
            }
        }
    }
    
    // 支持键盘控制
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        inputEnabled = false;
        inputClock.restart();
        
        if (!showSettings_)
        {
            Game::Screen = std::make_shared<GameScreen>();
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        inputEnabled = false;
        inputClock.restart();
        
        if (showSettings_)
            showSettings_ = false;
        else
            window.close();
    }
}

void MenuScreen::update(sf::Time delta)
{
    // 标题动画
    static bool growingTitle = true;
    static float titleScale = 1.0f;
    
    if (growingTitle)
    {
        titleScale += delta.asSeconds() * 0.1f;
        if (titleScale >= 1.1f)
            growingTitle = false;
    }
    else
    {
        titleScale -= delta.asSeconds() * 0.1f;
        if (titleScale <= 0.95f)
            growingTitle = true;
    }
    
    title_.setScale(titleScale, titleScale);
    
    // 菜单项高亮效果
    sf::Vector2i mousePos = sf::Mouse::getPosition();
    
    if (!showSettings_)
    {
        playText_.setFillColor(playText_.getGlobalBounds().contains(mousePos.x, mousePos.y) ? 
                               sf::Color::Green : sf::Color::White);
        settingsText_.setFillColor(settingsText_.getGlobalBounds().contains(mousePos.x, mousePos.y) ? 
                                  sf::Color::Green : sf::Color::White);
        exitText_.setFillColor(exitText_.getGlobalBounds().contains(mousePos.x, mousePos.y) ? 
                              sf::Color::Green : sf::Color::White);
    }
    else
    {
        bgColorText_.setFillColor(bgColorText_.getGlobalBounds().contains(mousePos.x, mousePos.y) ? 
                                 sf::Color::Green : sf::Color::White);
        gridColorText_.setFillColor(gridColorText_.getGlobalBounds().contains(mousePos.x, mousePos.y) ? 
                                   sf::Color::Green : sf::Color::White);
        gridVisibilityText_.setFillColor(gridVisibilityText_.getGlobalBounds().contains(mousePos.x, mousePos.y) ? 
                                        sf::Color::Green : sf::Color::White);
    }
}

void MenuScreen::render(sf::RenderWindow& window)
{
    // 首先绘制logo作为背景
    window.draw(logoSprite_);
    
    // 然后绘制其他UI元素
    window.draw(title_);
    window.draw(instructionsText_);
    
    if (!showSettings_)
    {
        // 显示主菜单
        window.draw(playText_);
        window.draw(settingsText_);
        window.draw(exitText_);
    }
    else
    {
        // 显示设置菜单
        window.draw(bgColorText_);
        window.draw(gridColorText_);
        window.draw(gridVisibilityText_);
        window.draw(exitText_); // 返回按钮
    }
} 