#ifndef SNAKE_NODE_H
#define SNAKE_NODE_H

#include <SFML/Graphics.hpp>

namespace sfSnake
{
class SnakeNode
{
public:
    SnakeNode(sf::Vector2f position = sf::Vector2f(0, 0));

    void setPosition(sf::Vector2f position);
    void setPosition(float x, float y);

    void move(float xOffset, float yOffset);

    void render(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

    // 新增颜色设置方法
    void setFillColor(const sf::Color& color);
    sf::Color getFillColor() const;

    static const float Width;
    static const float Height;
    
    // 设置方向
    void setDirection(const sf::Vector2f& direction);
    sf::Vector2f getDirection() const;

    // 设置颜色
    void setOutlineColor(const sf::Color& color);

private:
    sf::CircleShape circle_;      // 红色小圆球
    sf::RectangleShape rect_;     // 黑色矩形块
    sf::Vector2f position_;       // 位置
    sf::Vector2f direction_;      // 方向
    static const float CircleRadius;  // 圆球半径
    static const float RectLength;    // 矩形长度
    bool isHead_; // 是否是头部节点
};
}

#endif 