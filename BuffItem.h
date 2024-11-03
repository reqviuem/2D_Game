#pragma once


#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Item.h"

class CharDraw;

class BufferItem {
public:
    Item type;
    sf::Texture texture;
    sf::Sprite sprite;

    BufferItem(sf::Texture &texture, int x, int y, float scaleX, float scaleY);

    void draw(sf::RenderWindow &window);


    bool checkCollision(const CharDraw &player);

    sf::RectangleShape getHitbox() const;

    Item getType() const;

    void setType(Item type);


};


