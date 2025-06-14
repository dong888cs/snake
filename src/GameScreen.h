#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <SFML/Graphics.hpp>

#include <vector>

#include "Screen.h"
#include "Snake.h"
#include "Fruit.h"
#include "Grid.h"

namespace sfSnake
{
class GameScreen : public Screen
{
public:
    GameScreen();

    void handleInput(sf::RenderWindow& window) override;
    void update(sf::Time delta) override;
    void render(sf::RenderWindow& window) override;

    void generateFruit();
    
    // 更新网格设置，应用全局设置到当前游戏
    void updateGridSettings();

private:
    Snake snake_;
    std::vector<Fruit> fruit_;
    Grid grid_;
    
    // 分数显示
    sf::Font font_;
    sf::Text scoreText_;
    sf::Text speedText_;
    sf::Text speedValueText_;
    sf::Text pauseText_;
    
    // 暂停功能
    bool paused_;
};
}

#endif 