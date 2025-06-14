#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <SFML/Graphics.hpp>

#include "Screen.h"

namespace sfSnake
{
class MenuScreen : public Screen
{
public:
    MenuScreen();

    void handleInput(sf::RenderWindow& window) override;
    void update(sf::Time delta) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font_;
    sf::Text title_;
    sf::Text playText_;
    sf::Text settingsText_;
    sf::Text exitText_;
    sf::Text instructionsText_;
    
    // 设置选项
    sf::Text bgColorText_;
    sf::Text gridColorText_;
    sf::Text gridVisibilityText_;
    
    // 设置界面状态
    bool showSettings_;
    
    // 设置界面图像
    sf::Texture logoTexture_;
    sf::Sprite logoSprite_;
};
}

#endif 