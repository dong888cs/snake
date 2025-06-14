#ifndef GAME_OVER_SCREEN_H
#define GAME_OVER_SCREEN_H

#include <SFML/Graphics.hpp>

#include "Screen.h"

namespace sfSnake
{
class GameOverScreen : public Screen
{
public:
    GameOverScreen(std::size_t score);

    void handleInput(sf::RenderWindow& window) override;
    void update(sf::Time delta) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font_;
    sf::Text gameOverText_;
    sf::Text scoreText_;
    sf::Text restartText_;
    sf::Text exitText_;
    sf::Text topLeftExitText_;

    std::size_t score_;
};
}

#endif 