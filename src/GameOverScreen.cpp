#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>

#include "GameOverScreen.h"
#include "MenuScreen.h"
#include "GameScreen.h"
#include "Game.h"

using namespace sfSnake;

GameOverScreen::GameOverScreen(std::size_t score) : score_(score)
{
    if (!font_.loadFromFile("assets/fonts/game_over.ttf")) {
        std::cerr << "无法加载字体!" << std::endl;
    }
    
    gameOverText_.setFont(font_);
    gameOverText_.setString("Game Over");
    gameOverText_.setCharacterSize(48);
    gameOverText_.setFillColor(sf::Color::Red);
    sf::FloatRect gameOverBounds = gameOverText_.getLocalBounds();
    gameOverText_.setOrigin(gameOverBounds.width / 2, gameOverBounds.height / 2);
    gameOverText_.setPosition(Game::Width / 2, Game::Height / 2 - 100);
    
    scoreText_.setFont(font_);
    scoreText_.setString("Your score: " + std::to_string(score_));
    scoreText_.setCharacterSize(32);
    scoreText_.setFillColor(sf::Color::Red);
    sf::FloatRect scoreBounds = scoreText_.getLocalBounds();
    scoreText_.setOrigin(scoreBounds.width / 2, scoreBounds.height / 2);
    scoreText_.setPosition(Game::Width / 2, Game::Height / 2);
    
    restartText_.setFont(font_);
    restartText_.setString("Play Again");
    restartText_.setCharacterSize(32);
    restartText_.setFillColor(sf::Color::Red);
    sf::FloatRect restartBounds = restartText_.getLocalBounds();
    restartText_.setOrigin(restartBounds.width / 2, restartBounds.height / 2);
    restartText_.setPosition(Game::Width / 2, Game::Height / 2 + 50);
    
    exitText_.setFont(font_);
    exitText_.setString("Main Menu");
    exitText_.setCharacterSize(32);
    exitText_.setFillColor(sf::Color::Red);
    sf::FloatRect exitBounds = exitText_.getLocalBounds();
    exitText_.setOrigin(exitBounds.width / 2, exitBounds.height / 2);
    exitText_.setPosition(Game::Width / 2, Game::Height / 2 + 100);

    // 添加左上角的退出按钮
    topLeftExitText_.setFont(font_);
    topLeftExitText_.setString("Exit");
    topLeftExitText_.setCharacterSize(24);
    topLeftExitText_.setFillColor(sf::Color::Red);
    sf::FloatRect topLeftExitBounds = topLeftExitText_.getLocalBounds();
    topLeftExitText_.setOrigin(topLeftExitBounds.width / 2, topLeftExitBounds.height / 2);
    topLeftExitText_.setPosition(50, 10);
}

void GameOverScreen::handleInput(sf::RenderWindow& window)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        
        if (restartText_.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
        {
            Game::Screen = std::make_shared<GameScreen>();
        }
        else if (exitText_.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
        {
            Game::Screen = std::make_shared<MenuScreen>();
        }
        else if (topLeftExitText_.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
        {
            window.close();  // 直接关闭窗口
        }
    }
}

void GameOverScreen::update(sf::Time delta)
{
    // 添加鼠标悬停效果
    sf::Vector2i mousePos = sf::Mouse::getPosition();
    
    // 更新左上角退出按钮的颜色
    topLeftExitText_.setFillColor(topLeftExitText_.getGlobalBounds().contains(mousePos.x, mousePos.y) ? 
                                 sf::Color(200, 0, 0) : sf::Color::Red);
}

void GameOverScreen::render(sf::RenderWindow& window)
{
    window.draw(gameOverText_);
    window.draw(scoreText_);
    window.draw(restartText_);
    window.draw(exitText_);
    window.draw(topLeftExitText_);  // 绘制左上角的退出按钮
} 