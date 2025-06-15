#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>
#include <cmath>

#include "Snake.h"
#include "Game.h"
#include "Fruit.h"
#include "SnakeNode.h"
#include "GameOverScreen.h"

using namespace sfSnake;

const int Snake::InitialSize = 5;

// 恢复旧的构造函数逻辑
Snake::Snake() 
: direction_(Direction::Up), 
  hitSelf_(false), 
  speedup_(false), 
  currentSpeed_(INITIAL_SPEED), 
  score_(0), 
  tailOverlap_(0), 
  m_timeSinceLastMove_(sf::Time::Zero)
{
    initNodes();

    if (!pickupBuffer_.loadFromFile("assets/sounds/pickup.wav")) {
        std::cerr << "无法加载pickup音效!" << std::endl;
    }
    pickupSound_.setBuffer(pickupBuffer_);
    pickupSound_.setVolume(30);

    if (!dieBuffer_.loadFromFile("assets/sounds/die.wav")) {
        std::cerr << "无法加载die音效!" << std::endl;
    }
    dieSound_.setBuffer(dieBuffer_);
    dieSound_.setVolume(50);
    
    if (!headTexture_.loadFromFile("assets/textures/head.png")) {
        std::cerr << "无法加载蛇头纹理!" << std::endl;
    }
    headTexture_.setSmooth(true);
    
    headSprite_.setTexture(headTexture_);
    
    sf::FloatRect bounds = headSprite_.getLocalBounds();
    headSprite_.setOrigin(bounds.width / 2, bounds.height / 2);
    
    float scale = SnakeNode::Width / std::max(bounds.width, bounds.height);
    headSprite_.setScale(scale, scale);
    
    headSprite_.setColor(sf::Color(255, 255, 255, 255));
    
    if (!nodeTexture_.loadFromFile("assets/textures/node.png")) {
        std::cerr << "无法加载节点纹理!" << std::endl;
    }
    nodeTexture_.setSmooth(true);
    nodeSprite_.setTexture(nodeTexture_);
    sf::FloatRect nodeBounds = nodeSprite_.getLocalBounds();
    nodeSprite_.setOrigin(nodeBounds.width / 2, nodeBounds.height / 2);
    nodeSprite_.setScale(SnakeNode::Width / nodeBounds.width, SnakeNode::Height / nodeBounds.height);
}

void Snake::initNodes()
{
    nodes_.clear();
    sf::Vector2f headPos(Game::Width / 2.f, Game::Height / 2.f);

    for (int i = 0; i < InitialSize; ++i)
    {
        sf::Vector2f position(headPos.x, headPos.y + i * SnakeNode::Height);
        SnakeNode node(position);
        node.setDirection(sf::Vector2f(0, -1));
        nodes_.push_back(node);
    }
}

void Snake::handleInput(sf::RenderWindow& window)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction_ != Direction::Down)
        direction_ = Direction::Up;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction_ != Direction::Up)
        direction_ = Direction::Down;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction_ != Direction::Right)
        direction_ = Direction::Left;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction_ != Direction::Left)
        direction_ = Direction::Right;
    
    static bool spaceReleased = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (spaceReleased)
        {
            toggleSpeedup();
            spaceReleased = false;
        }
    }
    else
    {
        spaceReleased = true;
    }
    
    static sf::Clock mouseClock;
    if (mouseClock.getElapsedTime().asSeconds() > 0.1f)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            mouseClock.restart();
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2f headPosition = nodes_.front().getPosition();
            
            mouseDirection_ = sf::Vector2f(mousePosition.x - headPosition.x, mousePosition.y - headPosition.y);
            float len = std::sqrt(std::pow(mouseDirection_.x, 2) + std::pow(mouseDirection_.y, 2));
            if (len > 0)
            {
                mouseDirection_ /= len;
            }

            direction_ = Direction::Mouse;
        }
    }
}

// 恢复基于计时器的跳跃式移动
void Snake::update(sf::Time delta)
{
    float speed = currentSpeed_ * (speedup_ ? SPEEDUP_MULTIPLIER : 1.f);
    if (speed > 0.f)
    {
        m_timeSinceLastMove_ += delta;
        sf::Time timePerMove = sf::seconds(SnakeNode::Width / speed);
        while (m_timeSinceLastMove_ >= timePerMove)
        {
            m_timeSinceLastMove_ -= timePerMove;
            move();
        }
    }
    checkEdgeCollisions();
    checkSelfCollisions();
}

// 恢复经典的move函数
void Snake::move()
{
    SnakeNode newHead = nodes_.front();
    sf::Vector2f currentDirection;
    
    switch (direction_)
    {
    case Direction::Up:
        newHead.move(0, -SnakeNode::Height);
        currentDirection = sf::Vector2f(0, -1);
        break;
    case Direction::Down:
        newHead.move(0, SnakeNode::Height);
        currentDirection = sf::Vector2f(0, 1);
        break;
    case Direction::Left:
        newHead.move(-SnakeNode::Width, 0);
        currentDirection = sf::Vector2f(-1, 0);
        break;
    case Direction::Right:
        newHead.move(SnakeNode::Width, 0);
        currentDirection = sf::Vector2f(1, 0);
        break;
    case Direction::Mouse:
        newHead.move(mouseDirection_.x * SnakeNode::Width, 
                    mouseDirection_.y * SnakeNode::Height);
        currentDirection = mouseDirection_;
        break;
    }
    
    newHead.setDirection(currentDirection);
    nodes_.push_front(newHead);
    
    for (size_t i = 1; i < nodes_.size(); ++i)
    {
        sf::Vector2f dir = nodes_[i-1].getPosition() - nodes_[i].getPosition();
        nodes_[i].setDirection(dir);
    }
    
    if (tailOverlap_ > 0)
        tailOverlap_--;
    else
        nodes_.pop_back();
}


void Snake::checkFruitCollisions(std::vector<Fruit>& fruits)
{
    auto toRemove = fruits.end();
    for (auto it = fruits.begin(); it != fruits.end(); ++it)
    {
        if (it->getBounds().intersects(nodes_.front().getBounds()))
            toRemove = it;
    }

    if (toRemove != fruits.end())
    {
        pickupSound_.play();
        int growAmount = toRemove->getGrowthAmount();
        grow(growAmount);
        score_ += growAmount;
        increaseSpeed();
        fruits.erase(toRemove);
    }
}

// 恢复旧的grow逻辑
void Snake::grow(int segments)
{
    if (segments <= 0)
        return;
    tailOverlap_ += segments;
}

void Snake::increaseSpeed()
{
    currentSpeed_ = std::min(currentSpeed_ + SPEED_INCREMENT, MAX_SPEED);
}

void Snake::toggleSpeedup()
{
    speedup_ = !speedup_;
    if (!speedup_ && currentSpeed_ > MAX_SPEED) {
        currentSpeed_ = MAX_SPEED;
    }
}

unsigned Snake::getSize() const
{
    return nodes_.size();
}

unsigned Snake::getScore() const
{
    return score_;
}

bool Snake::hitSelf() const
{
    return hitSelf_;
}

void Snake::checkSelfCollisions()
{
    SnakeNode& headNode = nodes_.front();
    if (nodes_.size() < 5)
        return;
    
    for (size_t i = 5; i < nodes_.size(); ++i)
    {
        if (headNode.getBounds().intersects(nodes_[i].getBounds()))
        {
            dieSound_.play();
            sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
            hitSelf_ = true;
            break;
        }
    }
}

void Snake::checkEdgeCollisions()
{
    SnakeNode& headNode = nodes_.front();

    if (headNode.getPosition().x <= 0 || headNode.getPosition().x >= Game::Width ||
        headNode.getPosition().y <= 0 || headNode.getPosition().y >= Game::Height)
    {
        dieSound_.play();
        sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
        hitSelf_ = true;
    }
}

sf::Vector2f Snake::toWindow(sf::Vector2f node)
{
    while (node.x < 0) node.x += Game::Width;
    while (node.x > Game::Width) node.x -= Game::Width;
    while (node.y < 0) node.y += Game::Height;
    while (node.y > Game::Height) node.y -= Game::Height;
    return node;
}

void Snake::render(sf::RenderWindow& window)
{
    for (size_t i = 1; i < nodes_.size(); ++i)
    {
        nodes_[i].render(window);
    }
    
    headSprite_.setPosition(nodes_.front().getPosition());
    
    float angle = atan2(nodes_.front().getDirection().y, nodes_.front().getDirection().x) * 180 / 3.14159f;
    headSprite_.setRotation(angle + 90);

    window.draw(headSprite_);
}

float Snake::getSpeed() const
{
    return speedup_ ? std::min(currentSpeed_ * SPEEDUP_MULTIPLIER, MAX_SPEED * SPEEDUP_MULTIPLIER) : currentSpeed_;
}