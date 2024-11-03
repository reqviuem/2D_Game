#pragma once


#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "CharDraw.h"

class Obstacle {
public:
    sf::Texture texture;
    sf::Sprite sprite;

    Obstacle(sf::Texture& texture, int x, int y,float scaleX,float scaleY) ;


    void draw(sf::RenderWindow &window) ;


    bool checkCollision(const CharDraw &player);

    sf::RectangleShape getHitbox() const ;

};

