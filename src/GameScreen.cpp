#include <SFML/Graphics.hpp>

#include <random>
#include <memory>
#include <iostream>

#include "GameScreen.h"
#include "GameOverScreen.h"
#include "Game.h"

using namespace sfSnake;

GameScreen::GameScreen() 
: snake_(), grid_(Game::Width, Game::Height, SnakeNode::Width), paused_(false)
{
    // 加载字体
    if (!font_.loadFromFile("assets/fonts/game_over.ttf")) {
        std::cerr << "无法加载字体!" << std::endl;
    }
    
    // 设置分数文本
    scoreText_.setFont(font_);
    scoreText_.setCharacterSize(24);
    scoreText_.setFillColor(sf::Color::White);
    scoreText_.setOutlineColor(sf::Color::Black);
    scoreText_.setOutlineThickness(1.f);
    scoreText_.setPosition(10.f, 10.f);
    
    // 设置速度状态文本
    speedText_.setFont(font_);
    speedText_.setString("Speed: Normal");
    speedText_.setCharacterSize(18);
    speedText_.setFillColor(sf::Color::White);
    speedText_.setOutlineColor(sf::Color::Black);
    speedText_.setOutlineThickness(1.f);
    speedText_.setPosition(10.f, 40.f);
    
    // 设置速度值文本
    speedValueText_.setFont(font_);
    speedValueText_.setCharacterSize(18);
    speedValueText_.setFillColor(sf::Color::White);
    speedValueText_.setOutlineColor(sf::Color::Black);
    speedValueText_.setOutlineThickness(1.f);
    speedValueText_.setPosition(Game::Width - 200.f, 10.f);  // 放置在右上角
    
    // 设置暂停文本
    pauseText_.setFont(font_);
    pauseText_.setString("PAUSED\nPress P to continue");
    pauseText_.setCharacterSize(36);
    pauseText_.setFillColor(sf::Color::White);
    pauseText_.setOutlineColor(sf::Color::Black);
    pauseText_.setOutlineThickness(2.f);
    
    // 居中暂停文本
    sf::FloatRect pauseTextBounds = pauseText_.getLocalBounds();
    pauseText_.setOrigin(pauseTextBounds.width / 2, pauseTextBounds.height / 2);
    pauseText_.setPosition(Game::Width / 2, Game::Height / 2);
    
    // 应用当前网格设置
    updateGridSettings();
    
    generateFruit();
}

void GameScreen::handleInput(sf::RenderWindow& window)
{
    // 暂停/继续控制
    static bool pKeyReleased = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        if (pKeyReleased)
        {
            paused_ = !paused_;
            pKeyReleased = false;
        }
    }
    else
    {
        pKeyReleased = true;
    }
    
    // 只有在非暂停状态下才处理蛇的输入
    if (!paused_)
    {
        snake_.handleInput(window);
    }
}

void GameScreen::update(sf::Time delta)
{
    // 在暂停状态下不更新游戏逻辑
    if (paused_)
        return;

    // 更新分数显示
    scoreText_.setString("Score: " + std::to_string(snake_.getScore()));
    
    // 更新速度状态显示
    speedText_.setString(snake_.isSpeedup() ? "Speed: FAST" : "Speed: Normal");
    
    // 更新速度值显示
    float currentSpeed = snake_.getSpeed();
    speedValueText_.setString("Speed: " + std::to_string(static_cast<int>(currentSpeed)));
    
    // 更新网格设置
    updateGridSettings();
    
    // 更新水果状态并移除过期水果
    for (auto it = fruit_.begin(); it != fruit_.end();) {
        it->update(delta);
        if (it->shouldRemove()) {
            it = fruit_.erase(it);
        } else {
            ++it;
        }
    }
    
    // 保持水果数量在4个左右
    while (fruit_.size() < 4) {
        generateFruit();
    }

    snake_.update(delta);
    snake_.checkFruitCollisions(fruit_);

    if (snake_.hitSelf())
        Game::Screen = std::make_shared<GameOverScreen>(snake_.getScore());
}

void GameScreen::render(sf::RenderWindow& window)
{
    // 绘制网格 - 只有在网格可见时才绘制
    if (Game::isGlobalGridVisible()) {
        grid_.render(window);
    }
    
    // 绘制蛇和水果
    snake_.render(window);

    for (auto& fruit : fruit_)
        fruit.render(window);
    
    // 绘制分数和速度信息
    window.draw(scoreText_);
    window.draw(speedText_);
    window.draw(speedValueText_);
    
    // 如果游戏暂停，显示暂停信息
    if (paused_)
    {
        // 创建半透明背景
        sf::RectangleShape overlay(sf::Vector2f(Game::Width, Game::Height));
        overlay.setFillColor(sf::Color(0, 0, 0, 128)); // 半透明黑色
        window.draw(overlay);
        
        // 显示暂停文本
        window.draw(pauseText_);
    }
}

void GameScreen::generateFruit()
{
    // 使用更安全的随机数生成方式
    static std::random_device rd;
    static std::mt19937 rng(rd());
    static std::uniform_int_distribution<int> xDistribution(SnakeNode::Width, Game::Width - SnakeNode::Width * 2);
    static std::uniform_int_distribution<int> yDistribution(SnakeNode::Height, Game::Height - SnakeNode::Height * 2);

    // 生成一个水果并确保它不在蛇身上
    sf::Vector2f position;
    bool validPosition = false;
    int attempts = 0;
    const int maxAttempts = 20; // 增加尝试次数以确保能找到有效位置
    
    while (!validPosition && attempts < maxAttempts)
    {
        position = sf::Vector2f(xDistribution(rng), yDistribution(rng));
        validPosition = true;
        
        // 检查是否与蛇重叠
        sf::FloatRect fruitBounds(position, sf::Vector2f(SnakeNode::Width, SnakeNode::Height));
        
        // 检查是否与其他水果重叠
        for (const auto& existingFruit : fruit_) {
            if (fruitBounds.intersects(existingFruit.getBounds())) {
                validPosition = false;
                break;
            }
        }
        
        // 如果位置有效，检查是否与蛇重叠
        if (validPosition) {
            for (const auto& node : snake_.getNodes()) {
                if (fruitBounds.intersects(node.getBounds())) {
                    validPosition = false;
                    break;
                }
            }
        }
        
        attempts++;
    }
    
    if (validPosition) {
        fruit_.push_back(Fruit(position));
    }
}

void GameScreen::updateGridSettings()
{
    // 应用当前的网格设置
    grid_.setVisible(Game::isGlobalGridVisible());
    
    // 根据当前网格颜色设置实际颜色
    sf::Color gridColor;
    switch (Game::getGlobalGridColor()) {
        case GridColor::White:
            gridColor = sf::Color(255, 255, 255, 80); // 半透明白色
            break;
        case GridColor::Black:
            gridColor = sf::Color(0, 0, 0, 80);       // 半透明黑色
            break;
        case GridColor::Brown:
            gridColor = sf::Color(139, 69, 19, 80);   // 半透明棕色
            break;
    }
    grid_.setColor(gridColor);
} 