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

Snake::Snake() : direction_(Direction::Right), hitSelf_(false), speedup_(false), currentSpeed_(INITIAL_SPEED), score_(0), tailOverlap_(0)
{
    initNodes();

    // 加载音效
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
    
    // 加载蛇头纹理并平滑处理
    if (!headTexture_.loadFromFile("assets/textures/head.png")) {
        std::cerr << "无法加载蛇头纹理!" << std::endl;
    }
    headTexture_.setSmooth(true);
    
    // 设置蛇头精灵
    headSprite_.setTexture(headTexture_);
    
    // 调整大小和原点，保持原始宽高比
    sf::FloatRect bounds = headSprite_.getLocalBounds();
    headSprite_.setOrigin(bounds.width / 2, bounds.height / 2);
    
    // 计算缩放比例，使蛇头大小与节点大小相近
    float scale = SnakeNode::Width / std::max(bounds.width, bounds.height);
    headSprite_.setScale(scale, scale);
    
    // 设置蛇头精灵的混合模式
    headSprite_.setColor(sf::Color(255, 255, 255, 255));  // 完全不透明
    
    // 初始化蛇身节点纹理 - 使用简单的颜色替代
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
    for (int i = 0; i < Snake::InitialSize; ++i)
    {
        SnakeNode node(sf::Vector2f(
            Game::Width / 2 - SnakeNode::Width / 2,
            Game::Height / 2 - (SnakeNode::Height / 2) + (SnakeNode::Height * i)));
            
        // 设置蛇节点的颜色交替
        if (i % 2 == 0) {
            node.setFillColor(sf::Color::White);
        } else {
            // 使用与蛇头相似的颜色
            node.setFillColor(sf::Color::Green);
        }
        
        nodes_.push_back(node);
    }
}

void Snake::handleInput(sf::RenderWindow& window)
{
    // 键盘控制
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction_ != Direction::Down)
        direction_ = Direction::Up;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction_ != Direction::Up)
        direction_ = Direction::Down;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction_ != Direction::Right)
        direction_ = Direction::Left;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction_ != Direction::Left)
        direction_ = Direction::Right;
    
    // 加速控制
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
    
    // 鼠标控制 - 添加防抖机制
    static sf::Clock mouseClock;
    static bool mouseEnabled = true;
    
    if (mouseClock.getElapsedTime().asSeconds() > 0.1f) {
        mouseEnabled = true;
    }
    
    if (mouseEnabled && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        mouseEnabled = false;
        mouseClock.restart();
        
        // 获取鼠标位置
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f headPosition = nodes_.front().getPosition();
        
        // 避免在UI区域点击改变方向
        sf::Vector2f uiCorner(Game::Width * 0.9f, Game::Height * 0.1f);
        float distance = std::sqrt(
            std::pow(mousePosition.x - uiCorner.x, 2) +
            std::pow(mousePosition.y - uiCorner.y, 2));
            
        if (distance > Game::Width / 16.0f) {
            // 计算方向向量
            mouseDirection_.x = mousePosition.x - headPosition.x;
            mouseDirection_.y = mousePosition.y - headPosition.y;
            
            // 归一化方向向量
            float length = std::sqrt(mouseDirection_.x * mouseDirection_.x + mouseDirection_.y * mouseDirection_.y);
            if (length > 0) {
                mouseDirection_.x /= length;
                mouseDirection_.y /= length;
                direction_ = Direction::Mouse;
            }
        }
    }
}

void Snake::update(sf::Time delta)
{
    move();
    checkEdgeCollisions();
    checkSelfCollisions();
}

void Snake::checkFruitCollisions(std::vector<Fruit>& fruits)
{
    decltype(fruits.begin()) toRemove = fruits.end();

    for (auto it = fruits.begin(); it != fruits.end(); ++it)
    {
        if (it->getBounds().intersects(nodes_.front().getBounds()))
            toRemove = it;
    }

    if (toRemove != fruits.end())
    {
        pickupSound_.play();
        // 根据水果类型增加长度和分数
        int growAmount = toRemove->getGrowthAmount();
        grow(growAmount);
        score_ += growAmount;
        
        // 增加速度
        increaseSpeed();
        
        fruits.erase(toRemove);
    }
}

void Snake::grow(int segments)
{
    // 如果segments为0，则不增长
    if (segments <= 0)
        return;
        
    // 增加尾部重叠计数，稍后在移动时处理
    tailOverlap_ += segments;
}

void Snake::increaseSpeed()
{
    // 增加速度，但不超过最大速度
    currentSpeed_ = std::min(currentSpeed_ + SPEED_INCREMENT, MAX_SPEED);
}

void Snake::toggleSpeedup()
{
    speedup_ = !speedup_;
    // 如果退出加速状态，确保速度不超过最大速度
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

    // 健壮性检查：确保蛇的最小长度
    if (nodes_.size() < 5)
    {
        dieSound_.play();
        sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
        hitSelf_ = true;
        return;
    }
    
    // 跳过前几个节点的碰撞检测，因为蛇头无法与紧挨着的节点相撞
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

    if (headNode.getPosition().x <= 0)
        headNode.setPosition(Game::Width, headNode.getPosition().y);
    else if (headNode.getPosition().x >= Game::Width)
        headNode.setPosition(0, headNode.getPosition().y);
    else if (headNode.getPosition().y <= 0)
        headNode.setPosition(headNode.getPosition().x, Game::Height);
    else if (headNode.getPosition().y >= Game::Height)
        headNode.setPosition(headNode.getPosition().x, 0);
}

sf::Vector2f Snake::toWindow(sf::Vector2f node)
{
    while (node.x < 0)
        node.x = node.x + Game::Width;
    while (node.x > Game::Width)
        node.x = node.x - Game::Width;
    while (node.y < 0)
        node.y = node.y + Game::Height;
    while (node.y > Game::Height)
        node.y = node.y - Game::Height;
    
    return node;
}

void Snake::move()
{
    // 根据加速状态决定移动次数
    int times = speedup_ ? 2 : 1;
    
    for (int t = 0; t < times; ++t)
    {
        // 创建新的头部节点
        SnakeNode newHead = nodes_.front();
        
        // 保存当前方向
        sf::Vector2f currentDirection;
        
        // 根据方向移动新头部
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
            // 使用鼠标方向向量移动
            newHead.move(mouseDirection_.x * SnakeNode::Width, 
                        mouseDirection_.y * SnakeNode::Height);
            currentDirection = mouseDirection_;
            break;
        }
        
        // 设置新头部的方向
        newHead.setDirection(currentDirection);
        
        // 将新头部添加到队列前端
        nodes_.push_front(newHead);
        
        // 更新其他节点的方向
        for (size_t i = 1; i < nodes_.size(); ++i)
        {
            sf::Vector2f dir = nodes_[i-1].getPosition() - nodes_[i].getPosition();
            nodes_[i].setDirection(dir);
        }
        
        // 只有在没有增长时才移除尾部
        if (tailOverlap_ > 0)
            tailOverlap_--;
        else
            nodes_.pop_back();
    }
}

void Snake::render(sf::RenderWindow& window)
{
    // 绘制蛇身，跳过头部节点
    for (size_t i = 1; i < nodes_.size(); ++i)
    {
        nodes_[i].render(window);
    }
    
    // 绘制蛇头
    headSprite_.setPosition(nodes_.front().getPosition());
    
    // 根据方向设置蛇头旋转
    switch (direction_)
    {
    case Direction::Up:
        headSprite_.setRotation(0);
        break;
    case Direction::Down:
        headSprite_.setRotation(180);
        break;
    case Direction::Left:
        headSprite_.setRotation(270);
        break;
    case Direction::Right:
        headSprite_.setRotation(90);
        break;
    case Direction::Mouse:
        // 根据方向向量设置蛇头旋转角度
        float angle = std::atan2(mouseDirection_.y, mouseDirection_.x) * 180 / 3.14159f;
        headSprite_.setRotation(angle + 90); // +90是因为精灵默认朝向
        break;
    }
    
    // 设置蛇头精灵的混合模式
    headSprite_.setColor(sf::Color(255, 255, 255, 255));  // 完全不透明
    window.draw(headSprite_);
}

float Snake::getSpeed() const
{
    // 返回当前速度，如果处于加速状态则翻倍，但不超过最大速度的两倍
    return speedup_ ? std::min(currentSpeed_ * SPEEDUP_MULTIPLIER, MAX_SPEED * SPEEDUP_MULTIPLIER) : currentSpeed_;
} 