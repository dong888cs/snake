#include "Grid.h"

using namespace sfSnake;

Grid::Grid(int width, int height, float cellSize)
    : width_(width), height_(height), cellSize_(cellSize), visible_(true), color_(sf::Color(100, 100, 100, 50))
{
    updateLines();
}

void Grid::updateLines()
{
    horizontalLines_.clear();
    verticalLines_.clear();
    
    // 创建水平线
    for (int y = 0; y <= height_; y += cellSize_)
    {
        horizontalLines_.push_back(sf::Vertex(sf::Vector2f(0, y), color_));
        horizontalLines_.push_back(sf::Vertex(sf::Vector2f(width_, y), color_));
    }
    
    // 创建垂直线
    for (int x = 0; x <= width_; x += cellSize_)
    {
        verticalLines_.push_back(sf::Vertex(sf::Vector2f(x, 0), color_));
        verticalLines_.push_back(sf::Vertex(sf::Vector2f(x, height_), color_));
    }
}

void Grid::render(sf::RenderWindow& window)
{
    if (visible_)
    {
        window.draw(&horizontalLines_[0], horizontalLines_.size(), sf::Lines);
        window.draw(&verticalLines_[0], verticalLines_.size(), sf::Lines);
    }
}

void Grid::setVisible(bool visible)
{
    visible_ = visible;
}

bool Grid::isVisible() const
{
    return visible_;
}

void Grid::setColor(const sf::Color& color)
{
    color_ = color;
    updateLines();
}

sf::Color Grid::getColor() const
{
    return color_;
} 