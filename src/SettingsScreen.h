#ifndef SETTINGS_SCREEN_H
#define SETTINGS_SCREEN_H

#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "Game.h"
#include <iostream>

namespace sfSnake {

class SettingsScreen : public Screen {
public:
    SettingsScreen();
    void handleInput(sf::RenderWindow& window) override;
    void update(sf::Time delta) override;
    void render(sf::RenderWindow& window) override;

private:
    // 更新界面颜色，根据当前背景色调整文本和其他元素的颜色
    void updateInterfaceColors();

    sf::Font font_;
    sf::RectangleShape background_;
    sf::Text title_;
    
    // 背景颜色选项
    sf::Text bgColorText_;
    sf::CircleShape bgColorWhite_;
    sf::CircleShape bgColorBlack_;
    sf::CircleShape bgColorBrown_;
    sf::CircleShape bgColorSelected_;
    
    // 网格可见性选项
    sf::Text gridVisibleText_;
    sf::RectangleShape gridVisibleCheckbox_;
    sf::Text gridVisibleStatus_;
    
    // 网格颜色选项
    sf::Text gridColorText_;
    sf::CircleShape gridColorWhite_;
    sf::CircleShape gridColorBlack_;
    sf::CircleShape gridColorBrown_;
    sf::CircleShape gridColorSelected_;
    
    // 返回按钮
    sf::RectangleShape returnButtonBg_;
    sf::Text returnButton_;
    
    // 当前选择状态
    BackgroundColor currentBgColor_;
    GridColor currentGridColor_;
    bool isGridVisible_;
};

} // namespace sfSnake

#endif // SETTINGS_SCREEN_H 