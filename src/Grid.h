#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>

namespace sfSnake
{
class Grid
{
public:
    Grid(int width, int height, float cellSize);
    
    void render(sf::RenderWindow& window);
    
    // 设置网格可见性
    void setVisible(bool visible);
    bool isVisible() const;
    
    // 设置网格颜色
    void setColor(const sf::Color& color);
    sf::Color getColor() const;

private:
    int width_;
    int height_;
    float cellSize_;
    bool visible_;
    sf::Color color_;
    
    std::vector<sf::Vertex> horizontalLines_;
    std::vector<sf::Vertex> verticalLines_;
    
    void updateLines();
};
}

#endif 