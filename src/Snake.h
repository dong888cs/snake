#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <deque>

#include "SnakeNode.h"
#include "Fruit.h"

namespace sfSnake
{
    enum class Direction
    {
        Left, Right, Up, Down, Mouse
    };

class Snake
{
public:
    Snake();

    void handleInput(sf::RenderWindow& window);
    void update(sf::Time delta);
    void render(sf::RenderWindow& window);

    void checkFruitCollisions(std::vector<Fruit>& fruits);

    bool hitSelf() const;

    unsigned getSize() const;
    unsigned getScore() const;
    float getSpeed() const;
    bool isSpeedup() const { return speedup_; }  // 新增：检查是否处于加速状态

    sf::Vector2f toWindow(sf::Vector2f node);
    
    // 获取蛇的所有节点
    const std::deque<SnakeNode>& getNodes() const { return nodes_; }

private:
    static constexpr float NODE_SIZE = 15.0f;  // 减小节点大小
    static constexpr float HEAD_SIZE = 18.0f;  // 减小头部大小
    static constexpr float INITIAL_SPEED = 50.0f;    // 初始速度
    static constexpr float SPEED_INCREMENT = 10.0f;  // 速度增量
    static constexpr float MAX_SPEED = 200.0f;       // 最大速度（普通状态）
    static constexpr float SPEEDUP_MULTIPLIER = 2.0f; // 加速状态的速度倍数
    static constexpr float TURN_THRESHOLD = 0.1f;
    static constexpr float GROWTH_INTERVAL = 0.5f;
    static constexpr float MIN_DISTANCE = 5.0f;  // 减小最小距离

    void move();
    void grow(int segments = 1);
    void checkEdgeCollisions();
    void checkSelfCollisions();
    void initNodes();
    void increaseSpeed();  // 增加速度的方法
    void toggleSpeedup();  // 切换加速状态

    bool hitSelf_;
    bool speedup_;
    
    sf::Vector2f position_;
    Direction direction_;
    sf::Vector2f mouseDirection_;

    sf::SoundBuffer pickupBuffer_;
    sf::Sound pickupSound_;

    sf::SoundBuffer dieBuffer_;
    sf::Sound dieSound_;

    std::deque<SnakeNode> nodes_;
    
    sf::Texture headTexture_;
    sf::Sprite headSprite_;
    
    sf::Texture nodeTexture_;
    sf::Sprite nodeSprite_;
    
    static const int InitialSize;
    unsigned score_;
    unsigned tailOverlap_;

    float currentSpeed_;  // 新增：当前速度
};
}

#endif 