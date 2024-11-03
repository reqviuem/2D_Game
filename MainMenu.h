#pragma once


#include "SFML/Graphics/RenderWindow.hpp"
#include <SFML/Graphics.hpp>

#include "fmt/core.h"
#include "Button.h"


class MainMenu {
public:
    MainMenu();
    void backgroundSetUp();
    void logoOfMenu();


    sf::RenderWindow window;

    sf::Sprite background;
    sf::Event ev;
    Button mainMenu;

    sf::Texture textTexture;

    sf::Texture backgroundTexture;
    sf::Sprite textSprite;

};

