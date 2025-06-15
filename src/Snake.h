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
    bool isSpeedup() const { return speedup_; }

    sf::Vector2f toWindow(sf::Vector2f node);
    
    const std::deque<SnakeNode>& getNodes() const { return nodes_; }

private:
    static constexpr float NODE_SIZE = 15.0f;
    static constexpr float HEAD_SIZE = 18.0f;
    static constexpr float INITIAL_SPEED = 100.0f;
    static constexpr float SPEED_INCREMENT = 20.0f;
    static constexpr float MAX_SPEED = 500.0f;
    static constexpr float SPEEDUP_MULTIPLIER = 2.0f;
    static constexpr float TURN_THRESHOLD = 0.1f;
    static constexpr float GROWTH_INTERVAL = 0.5f;
    static constexpr float MIN_DISTANCE = 5.0f;

    void move();
    void grow(int segments = 1);
    void checkEdgeCollisions();
    void checkSelfCollisions();
    void initNodes();
    void increaseSpeed();
    void toggleSpeedup();

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

    float currentSpeed_;
    sf::Time m_timeSinceLastMove_;
};
}

#endif