#ifndef FRUIT_H
#define FRUIT_H

#include <SFML/Graphics.hpp>

namespace sfSnake
{
    // 水果类型枚举
    enum class FruitType
    {
        Black,  // 黑色，不增加长度，存在20秒
        Brown,  // 棕色，不增加长度，存在20秒
        Red,    // 红色，增加3个长度，存在6秒
        Blue,   // 蓝色，增加2个长度，存在10秒
        Green   // 绿色，增加1个长度，存在15秒
    };

class Fruit
{
public:
    Fruit(sf::Vector2f position = sf::Vector2f(0, 0));

    void render(sf::RenderWindow& window);
    void update(sf::Time delta);

    sf::FloatRect getBounds() const;
    
    // 获取水果类型和增长长度
    FruitType getType() const;
    int getGrowthAmount() const;
    
    // 检查水果是否应该被移除
    bool shouldRemove() const;

private:
    sf::CircleShape shape_;
    FruitType type_;
    
    // 水果存在时间相关
    sf::Time lifetime_;
    sf::Time maxLifetime_;
    bool isBlinking_;
    float blinkTimer_;
    
    // 随机生成水果类型
    void randomizeType();
    // 根据类型设置最大存在时间
    void setMaxLifetime();

    static const float Radius;
};
}

#endif 