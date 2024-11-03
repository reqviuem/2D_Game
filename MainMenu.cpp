
#include "MainMenu.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "fmt/core.h"
#include "Button.h"
MainMenu::MainMenu()
        : window(sf::VideoMode(1280, 720), "Dark Souls", sf::Style::Titlebar | sf::Style::Close),
        background(),backgroundTexture(),textTexture(),textSprite()
          {
}
void MainMenu::backgroundSetUp() {

    if (!backgroundTexture.loadFromFile("../Backgrounds/wallpaperflare.com_wallpaper.jpg"))
    {
        fmt::print("Error loading background image.\n");
        throw std::runtime_error("Error loading background image.");
    }

    background.setTexture(backgroundTexture);
}

void MainMenu::logoOfMenu() {

    if (!textTexture.loadFromFile("../Text/red with black.png"))
    {
        fmt::print("Error loading background image.\n");
        throw std::runtime_error("Error loading text image.");
    }
    textSprite.setTexture(textTexture);


    sf::FloatRect textBounds = textSprite.getLocalBounds();
    float x = (window.getSize().x - textBounds.width) / 2;
    float y = 20;
    textSprite.setPosition(x, y);
}

