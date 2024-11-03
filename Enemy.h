#pragma once

#include "SFML/System/Time.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "CharDraw.h"

#include "SFML/System/Time.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class CharDraw;

class Enemy {
public:

    virtual void updateEnemy(sf::Time dt, const sf::RenderWindow &window, CharDraw &charDraw) = 0;

    virtual void drawEnemy(sf::RenderWindow &window, CharDraw &charDraw) = 0;

    virtual void updateEnemyHit(sf::Time dt) = 0;

    virtual void dealDamage(int amount) = 0;

    virtual sf::RectangleShape getHitbox() const = 0;

    virtual void followWaypoints(sf::Time dt) = 0;

};

