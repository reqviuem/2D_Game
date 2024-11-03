

#include "Obstacle.h"
Obstacle::Obstacle(sf::Texture& texture, int x, int y,float scaleX,float scaleY) {


    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(scaleX, scaleY);
}


void Obstacle:: draw(sf::RenderWindow &window) {
    window.draw(sprite);
}


bool Obstacle:: checkCollision(const CharDraw &player) {
    if (player.getHitbox().getGlobalBounds().intersects(getHitbox().getGlobalBounds())) {
        return true;
    }
    return false;
}
sf::RectangleShape Obstacle::getHitbox() const {
    sf::RectangleShape rect;
    rect.setSize({sprite.getGlobalBounds().width - 90, sprite.getGlobalBounds().height - 90});
    rect.setOrigin(-50, -50);
    rect.setPosition(sprite.getPosition());
    return rect;
}