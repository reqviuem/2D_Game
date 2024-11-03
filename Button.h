#pragma once


#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

class Button {
    sf::Texture buttonTexture1;
    sf::Texture buttonTexture2;
    sf::Texture buttonTexture3;
    sf::Sprite buttonSprite1;
    sf::Sprite buttonSprite2;
    sf::Sprite buttonSprite3;
    float x = 1280 / 2.5f;
    float gap = 20.0f;
    float buttonHeight = 80.0f;
    float yy1 = 720 / 4.0f;
    float yy2 = 720 / 4.0f + buttonHeight + gap;
    float yy3 = yy2 + buttonHeight + gap;
public:
    Button();
    sf::Sprite& getButton1();
    sf::Sprite& getButton2();
    sf::Sprite& getButton3();
};



