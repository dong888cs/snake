#include "SettingsScreen.h"
#include "Game.h"
#include "MenuScreen.h"
#include <iostream>

namespace sfSnake {

SettingsScreen::SettingsScreen() 
    : currentBgColor_(Game::getGlobalBackgroundColor()),
      currentGridColor_(Game::getGlobalGridColor()),
      isGridVisible_(Game::isGlobalGridVisible())
{
    // 加载字体
    if (!font_.loadFromFile("assets/fonts/game_over.ttf")) {
        std::cerr << "无法加载字体!" << std::endl;
    }

    // 设置背景 - 使用当前实际背景颜色
    background_.setSize(sf::Vector2f(Game::Width, Game::Height));
    
    // 根据当前背景颜色设置背景
    switch (currentBgColor_) {
        case BackgroundColor::White:
            background_.setFillColor(sf::Color(240, 240, 240));
            break;
        case BackgroundColor::Black:
            background_.setFillColor(sf::Color(40, 40, 40));
            break;
        case BackgroundColor::Brown:
            background_.setFillColor(sf::Color(139, 69, 19));
            break;
    }

    // 设置标题 - 根据背景颜色调整标题颜色
    title_.setFont(font_);
    title_.setString("SETTINGS");
    title_.setCharacterSize(50);
    title_.setFillColor(currentBgColor_ == BackgroundColor::White ? sf::Color(50, 50, 50) : sf::Color(240, 240, 240));
    title_.setPosition(Game::Width / 2 - title_.getLocalBounds().width / 2, 30);

    // 设置所有文本颜色 - 根据背景颜色调整
    sf::Color textColor = currentBgColor_ == BackgroundColor::White ? sf::Color::Black : sf::Color::White;

    // 设置背景颜色选项
    bgColorText_.setFont(font_);
    bgColorText_.setString("BACKGROUND COLOR:");
    bgColorText_.setCharacterSize(24);
    bgColorText_.setFillColor(textColor);
    bgColorText_.setPosition(Game::Width / 2 - bgColorText_.getLocalBounds().width / 2, 100);

    float colorOptionY = 140;
    float colorRadius = 15.0f;  // 确保所有圆形大小一致
    float colorSpacing = 60.0f;
    float startX = Game::Width / 2 - colorSpacing;

    // 白色选项
    bgColorWhite_.setRadius(colorRadius);
    bgColorWhite_.setFillColor(sf::Color::White);
    bgColorWhite_.setOutlineThickness(2);
    bgColorWhite_.setOutlineColor(sf::Color::Black);
    bgColorWhite_.setPosition(startX - colorRadius, colorOptionY);

    // 黑色选项
    bgColorBlack_.setRadius(colorRadius);
    bgColorBlack_.setFillColor(sf::Color::Black);
    bgColorBlack_.setOutlineThickness(2);
    bgColorBlack_.setOutlineColor(sf::Color::White);
    bgColorBlack_.setPosition(startX + colorSpacing - colorRadius, colorOptionY);

    // 褐色选项
    bgColorBrown_.setRadius(colorRadius);
    bgColorBrown_.setFillColor(sf::Color(139, 69, 19)); // 褐色
    bgColorBrown_.setOutlineThickness(2);
    bgColorBrown_.setOutlineColor(sf::Color::Black);
    bgColorBrown_.setPosition(startX + colorSpacing * 2 - colorRadius, colorOptionY);

    // 选择指示器
    bgColorSelected_.setRadius(5.0f);
    bgColorSelected_.setFillColor(sf::Color::Red);
    
    // 根据当前背景色设置选择指示器位置
    switch (currentBgColor_) {
        case BackgroundColor::White:
            bgColorSelected_.setPosition(bgColorWhite_.getPosition().x + colorRadius - 5.0f, 
                                       bgColorWhite_.getPosition().y + colorRadius * 2 + 10.0f);
            break;
        case BackgroundColor::Black:
            bgColorSelected_.setPosition(bgColorBlack_.getPosition().x + colorRadius - 5.0f, 
                                       bgColorBlack_.getPosition().y + colorRadius * 2 + 10.0f);
            break;
        case BackgroundColor::Brown:
            bgColorSelected_.setPosition(bgColorBrown_.getPosition().x + colorRadius - 5.0f, 
                                       bgColorBrown_.getPosition().y + colorRadius * 2 + 10.0f);
            break;
    }

    // 设置网格可见性选项
    gridVisibleText_.setFont(font_);
    gridVisibleText_.setString("GRID VISIBLE:");
    gridVisibleText_.setCharacterSize(24);
    gridVisibleText_.setFillColor(textColor);
    gridVisibleText_.setPosition(Game::Width / 2 - gridVisibleText_.getLocalBounds().width / 2, 200);

    gridVisibleCheckbox_.setSize(sf::Vector2f(20, 20));
    gridVisibleCheckbox_.setPosition(Game::Width / 2 - 10, 240);
    gridVisibleCheckbox_.setFillColor(isGridVisible_ ? sf::Color::Green : sf::Color::Transparent);
    gridVisibleCheckbox_.setOutlineThickness(2);
    gridVisibleCheckbox_.setOutlineColor(textColor);
    
    gridVisibleStatus_.setFont(font_);
    gridVisibleStatus_.setString(isGridVisible_ ? "YES" : "NO");
    gridVisibleStatus_.setCharacterSize(20);
    gridVisibleStatus_.setFillColor(textColor);
    gridVisibleStatus_.setPosition(Game::Width / 2 + 20, 240);

    // 设置网格颜色选项
    gridColorText_.setFont(font_);
    gridColorText_.setString("GRID COLOR:");
    gridColorText_.setCharacterSize(24);
    gridColorText_.setFillColor(textColor);
    gridColorText_.setPosition(Game::Width / 2 - gridColorText_.getLocalBounds().width / 2, 280);

    float gridColorY = 320;

    // 白色选项 - 确保与背景色选项大小一致
    gridColorWhite_.setRadius(colorRadius);
    gridColorWhite_.setFillColor(sf::Color::White);
    gridColorWhite_.setOutlineThickness(2);
    gridColorWhite_.setOutlineColor(sf::Color::Black);
    gridColorWhite_.setPosition(startX - colorRadius, gridColorY);

    // 黑色选项
    gridColorBlack_.setRadius(colorRadius);
    gridColorBlack_.setFillColor(sf::Color::Black);
    gridColorBlack_.setOutlineThickness(2);
    gridColorBlack_.setOutlineColor(sf::Color::White);
    gridColorBlack_.setPosition(startX + colorSpacing - colorRadius, gridColorY);

    // 褐色选项
    gridColorBrown_.setRadius(colorRadius);
    gridColorBrown_.setFillColor(sf::Color(139, 69, 19)); // 褐色
    gridColorBrown_.setOutlineThickness(2);
    gridColorBrown_.setOutlineColor(sf::Color::Black);
    gridColorBrown_.setPosition(startX + colorSpacing * 2 - colorRadius, gridColorY);

    // 选择指示器
    gridColorSelected_.setRadius(5.0f);
    gridColorSelected_.setFillColor(sf::Color::Red);
    
    // 根据当前网格色设置选择指示器位置
    switch (currentGridColor_) {
        case GridColor::White:
            gridColorSelected_.setPosition(gridColorWhite_.getPosition().x + colorRadius - 5.0f, 
                                         gridColorWhite_.getPosition().y + colorRadius * 2 + 10.0f);
            break;
        case GridColor::Black:
            gridColorSelected_.setPosition(gridColorBlack_.getPosition().x + colorRadius - 5.0f, 
                                         gridColorBlack_.getPosition().y + colorRadius * 2 + 10.0f);
            break;
        case GridColor::Brown:
            gridColorSelected_.setPosition(gridColorBrown_.getPosition().x + colorRadius - 5.0f, 
                                         gridColorBrown_.getPosition().y + colorRadius * 2 + 10.0f);
            break;
    }

    // 设置返回按钮
    returnButtonBg_.setSize(sf::Vector2f(200, 40));
    returnButtonBg_.setFillColor(sf::Color(100, 100, 100));
    returnButtonBg_.setPosition(Game::Width / 2 - 100, 400);
    
    returnButton_.setFont(font_);
    returnButton_.setString("RETURN TO MENU");
    returnButton_.setCharacterSize(20);
    returnButton_.setFillColor(sf::Color::White);
    sf::FloatRect returnBounds = returnButton_.getLocalBounds();
    returnButton_.setPosition(
        Game::Width / 2 - returnBounds.width / 2,
        400 + 20 - returnBounds.height / 2
    );
}

void SettingsScreen::handleInput(sf::RenderWindow& window) {
    // 添加防抖处理
    static sf::Clock inputClock;
    static bool inputEnabled = true;
    
    if (inputClock.getElapsedTime().asSeconds() > 0.2f) {
        inputEnabled = true;
    }
    
    if (!inputEnabled) {
        return;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        inputEnabled = false;
        inputClock.restart();
        
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        
        // 背景颜色选项
        if (bgColorWhite_.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            currentBgColor_ = BackgroundColor::White;
            Game::setGlobalBackgroundColor(BackgroundColor::White);
            bgColorSelected_.setPosition(
                bgColorWhite_.getPosition().x + bgColorWhite_.getRadius() - 5.0f,
                bgColorWhite_.getPosition().y + bgColorWhite_.getRadius() * 2 + 10.0f
            );
            updateInterfaceColors();
        }
        else if (bgColorBlack_.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            currentBgColor_ = BackgroundColor::Black;
            Game::setGlobalBackgroundColor(BackgroundColor::Black);
            bgColorSelected_.setPosition(
                bgColorBlack_.getPosition().x + bgColorBlack_.getRadius() - 5.0f,
                bgColorBlack_.getPosition().y + bgColorBlack_.getRadius() * 2 + 10.0f
            );
            updateInterfaceColors();
        }
        else if (bgColorBrown_.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            currentBgColor_ = BackgroundColor::Brown;
            Game::setGlobalBackgroundColor(BackgroundColor::Brown);
            bgColorSelected_.setPosition(
                bgColorBrown_.getPosition().x + bgColorBrown_.getRadius() - 5.0f,
                bgColorBrown_.getPosition().y + bgColorBrown_.getRadius() * 2 + 10.0f
            );
            updateInterfaceColors();
        }
        
        // 网格可见性选项
        if (gridVisibleCheckbox_.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            isGridVisible_ = !isGridVisible_;
            Game::setGlobalGridVisible(isGridVisible_);
            gridVisibleCheckbox_.setFillColor(isGridVisible_ ? sf::Color::Green : sf::Color::Transparent);
            gridVisibleStatus_.setString(isGridVisible_ ? "YES" : "NO");
        }
        
        // 网格颜色选项
        if (gridColorWhite_.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            currentGridColor_ = GridColor::White;
            Game::setGlobalGridColor(GridColor::White);
            gridColorSelected_.setPosition(
                gridColorWhite_.getPosition().x + gridColorWhite_.getRadius() - 5.0f,
                gridColorWhite_.getPosition().y + gridColorWhite_.getRadius() * 2 + 10.0f
            );
        }
        else if (gridColorBlack_.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            currentGridColor_ = GridColor::Black;
            Game::setGlobalGridColor(GridColor::Black);
            gridColorSelected_.setPosition(
                gridColorBlack_.getPosition().x + gridColorBlack_.getRadius() - 5.0f,
                gridColorBlack_.getPosition().y + gridColorBlack_.getRadius() * 2 + 10.0f
            );
        }
        else if (gridColorBrown_.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            currentGridColor_ = GridColor::Brown;
            Game::setGlobalGridColor(GridColor::Brown);
            gridColorSelected_.setPosition(
                gridColorBrown_.getPosition().x + gridColorBrown_.getRadius() - 5.0f,
                gridColorBrown_.getPosition().y + gridColorBrown_.getRadius() * 2 + 10.0f
            );
        }
        
        // 返回按钮
        if (returnButtonBg_.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
            Game::Screen = std::make_shared<MenuScreen>();
            return; // 立即返回，避免后续代码执行
        }
    }

    // 支持键盘控制
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        inputEnabled = false;
        inputClock.restart();
        Game::Screen = std::make_shared<MenuScreen>();
    }
}

void SettingsScreen::updateInterfaceColors() {
    // 根据当前背景颜色更新界面元素颜色
    sf::Color textColor = currentBgColor_ == BackgroundColor::White ? sf::Color::Black : sf::Color::White;
    
    // 更新背景颜色
    switch (currentBgColor_) {
        case BackgroundColor::White:
            background_.setFillColor(sf::Color(240, 240, 240));
            break;
        case BackgroundColor::Black:
            background_.setFillColor(sf::Color(40, 40, 40));
            break;
        case BackgroundColor::Brown:
            background_.setFillColor(sf::Color(139, 69, 19));
            break;
    }
    
    // 更新文本颜色
    title_.setFillColor(textColor);
    bgColorText_.setFillColor(textColor);
    gridVisibleText_.setFillColor(textColor);
    gridVisibleStatus_.setFillColor(textColor);
    gridColorText_.setFillColor(textColor);
    
    // 更新复选框边框颜色
    gridVisibleCheckbox_.setOutlineColor(textColor);
}

void SettingsScreen::update(sf::Time delta) {
    // 获取鼠标位置
    sf::Vector2i mousePos = sf::Mouse::getPosition();
    
    // 返回按钮悬停效果
    if (returnButtonBg_.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        returnButtonBg_.setFillColor(sf::Color(150, 150, 150));
    } else {
        returnButtonBg_.setFillColor(sf::Color(100, 100, 100));
    }
}

void SettingsScreen::render(sf::RenderWindow& window) {
    window.draw(background_);
    window.draw(title_);
    
    // 背景颜色选项
    window.draw(bgColorText_);
    window.draw(bgColorWhite_);
    window.draw(bgColorBlack_);
    window.draw(bgColorBrown_);
    window.draw(bgColorSelected_);
    
    // 网格可见性选项
    window.draw(gridVisibleText_);
    window.draw(gridVisibleCheckbox_);
    window.draw(gridVisibleStatus_);
    
    // 网格颜色选项
    window.draw(gridColorText_);
    window.draw(gridColorWhite_);
    window.draw(gridColorBlack_);
    window.draw(gridColorBrown_);
    window.draw(gridColorSelected_);
    
    // 返回按钮
    window.draw(returnButtonBg_);
    window.draw(returnButton_);
}

} // namespace sfSnake 