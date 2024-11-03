
#include <stdexcept>
#include "Button.h"

Button::Button() {
    if (!buttonTexture1.loadFromFile("../Button/button1.png") ||
        !buttonTexture2.loadFromFile("../Button/button2.png") ||
        !buttonTexture3.loadFromFile("../Button/button3.png")) {
        throw std::runtime_error("Error loading button texture.\n");
    }
    float xx = 1280 / 2.5f;
    buttonSprite1.setTexture(buttonTexture1);
    buttonSprite1.setScale(0.6f, 0.6f);
    buttonSprite1.setPosition(xx, yy1);

    buttonSprite2.setTexture(buttonTexture2);
    buttonSprite2.setScale(0.6f, 0.6f);
    buttonSprite2.setPosition(xx, yy2);

    buttonSprite3.setTexture(buttonTexture3);
    buttonSprite3.setScale(0.6f, 0.6f);
    buttonSprite3.setPosition(xx, yy3);
}

sf::Sprite &Button::getButton1() {
    return buttonSprite1;
}

sf::Sprite &Button::getButton2() {
    return buttonSprite2;
}

sf::Sprite &Button::getButton3() {
    return buttonSprite3;
}