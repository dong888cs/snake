#include <SFML/Graphics.hpp>
#include <cmath>

#include "SnakeNode.h"

using namespace sfSnake;

const float SnakeNode::Width = 15.f;
const float SnakeNode::Height = 15.f;
const float SnakeNode::CircleRadius = 7.5f;  // 圆球半径
const float SnakeNode::RectLength = CircleRadius * 2;    // 矩形长度等于圆球直径

SnakeNode::SnakeNode(sf::Vector2f position) : position_(position), direction_(1.f, 0.f)
{
    // 设置圆形 - 黄色实心
    circle_.setRadius(CircleRadius);
    circle_.setFillColor(sf::Color::Yellow);
    circle_.setOutlineThickness(0.f);  // 无描边
    circle_.setOrigin(CircleRadius, CircleRadius);
    
    // 设置矩形 - 黑色实心，宽度为长度的0.4
    float rectWidth = RectLength * 0.4f;  // 矩形宽度为长度的0.4
    rect_.setSize(sf::Vector2f(RectLength, rectWidth));
    rect_.setFillColor(sf::Color::Black);
    rect_.setOutlineThickness(0.f);  // 无描边
    rect_.setOrigin(RectLength / 2, rectWidth / 2);  // 设置原点在矩形中心
    
    setPosition(position);
}

void SnakeNode::setPosition(sf::Vector2f position)
{
    position_ = position;
    circle_.setPosition(position_);
    rect_.setPosition(position_);
}

void SnakeNode::setPosition(float x, float y)
{
    setPosition(sf::Vector2f(x, y));
}

void SnakeNode::move(float xOffset, float yOffset)
{
    position_.x += xOffset;
    position_.y += yOffset;
    setPosition(position_);
}

void SnakeNode::render(sf::RenderWindow& window)
{
    // 计算矩形的旋转角度
    float angle = std::atan2(direction_.y, direction_.x) * 180.f / 3.14159f;
    rect_.setRotation(angle);
    
    // 先绘制黄色球体，再绘制黑色矩形（黑色会覆盖部分黄色）
    window.draw(circle_);
    window.draw(rect_);
}

sf::Vector2f SnakeNode::getPosition() const
{
    return position_;
}

sf::FloatRect SnakeNode::getBounds() const
{
    // 使用圆形的边界作为碰撞检测区域
    return circle_.getGlobalBounds();
}

void SnakeNode::setFillColor(const sf::Color& color)
{
    circle_.setFillColor(color);
}

sf::Color SnakeNode::getFillColor() const
{
    return circle_.getFillColor();
}

void SnakeNode::setDirection(const sf::Vector2f& direction)
{
    direction_ = direction;
    // 归一化方向向量
    float length = std::sqrt(direction_.x * direction_.x + direction_.y * direction_.y);
    if (length > 0)
    {
        direction_.x /= length;
        direction_.y /= length;
    }
}

sf::Vector2f SnakeNode::getDirection() const
{
    return direction_;
} 