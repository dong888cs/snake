#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>

#include "Fruit.h"

using namespace sfSnake;

const float Fruit::Radius = 5.f;

Fruit::Fruit(sf::Vector2f position)
    : lifetime_(sf::Time::Zero), isBlinking_(false), blinkTimer_(0.f)
{
    shape_.setPosition(position);
    shape_.setRadius(Fruit::Radius);
    randomizeType();
    setMaxLifetime();
}

void Fruit::randomizeType()
{
    // 使用随机数生成器
    static std::default_random_engine engine(static_cast<unsigned>(time(nullptr)));
    static std::uniform_int_distribution<int> distribution(0, 99); // 0-99的随机数
    
    int random = distribution(engine);
    
    // 25%概率生成黑色和棕色水果，75%概率生成其他水果
    if (random < 12.5) {
        type_ = FruitType::Black;
        shape_.setFillColor(sf::Color::Black);
    } else if (random < 25) {
        type_ = FruitType::Brown;
        shape_.setFillColor(sf::Color(139, 69, 19)); // 棕色
    } else if (random < 35) {
        type_ = FruitType::Red;
        shape_.setFillColor(sf::Color::Red);
    } else if (random < 60) {
        type_ = FruitType::Blue;
        shape_.setFillColor(sf::Color::Blue);
    } else {
        type_ = FruitType::Green;
        shape_.setFillColor(sf::Color::Green);
    }
}

void Fruit::setMaxLifetime()
{
    switch (type_) {
        case FruitType::Black:
        case FruitType::Brown:
            maxLifetime_ = sf::seconds(20.f);
            break;
        case FruitType::Green:
            maxLifetime_ = sf::seconds(15.f);
            break;
        case FruitType::Blue:
            maxLifetime_ = sf::seconds(10.f);
            break;
        case FruitType::Red:
            maxLifetime_ = sf::seconds(6.f);
            break;
    }
}

void Fruit::update(sf::Time delta)
{
    lifetime_ += delta;
    
    // 检查是否需要开始闪烁
    if (lifetime_ >= maxLifetime_ - sf::seconds(3.f)) {
        isBlinking_ = true;
        blinkTimer_ += delta.asSeconds();
        
        // 闪烁效果：每0.2秒切换一次可见性
        if (blinkTimer_ >= 0.2f) {
            blinkTimer_ = 0.f;
            shape_.setFillColor(shape_.getFillColor().a == 0 ? 
                (type_ == FruitType::Brown ? sf::Color(139, 69, 19) : 
                 type_ == FruitType::Black ? sf::Color::Black :
                 type_ == FruitType::Red ? sf::Color::Red :
                 type_ == FruitType::Blue ? sf::Color::Blue :
                 sf::Color::Green) : 
                sf::Color(shape_.getFillColor().r, 
                         shape_.getFillColor().g, 
                         shape_.getFillColor().b, 0));
        }
    }
}

bool Fruit::shouldRemove() const
{
    return lifetime_ >= maxLifetime_;
}

void Fruit::render(sf::RenderWindow& window)
{
    window.draw(shape_);
}

sf::FloatRect Fruit::getBounds() const
{
    return shape_.getGlobalBounds();
}

FruitType Fruit::getType() const
{
    return type_;
}

int Fruit::getGrowthAmount() const
{
    // 根据水果类型返回增长长度
    switch (type_)
    {
        case FruitType::Red:
            return 3;
        case FruitType::Blue:
            return 2;
        case FruitType::Green:
            return 1;
        default: // Black和Brown
            return 0;
    }
} 