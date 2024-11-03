
#include "CharDraw.h"
#include "BuffItem.h"

BufferItem::BufferItem(sf::Texture &texture, int x, int y, float scaleX, float scaleY) {


    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(scaleX, scaleY);
}


void BufferItem::draw(sf::RenderWindow &window) {
    window.draw(sprite);
}


bool BufferItem::checkCollision(const CharDraw &player) {
    if (player.getHitbox().getGlobalBounds().intersects(getHitbox().getGlobalBounds())) {
        return true;
    }
    return false;
}

sf::RectangleShape BufferItem::getHitbox() const {
    sf::RectangleShape rect;
    rect.setSize({sprite.getGlobalBounds().width - 90, sprite.getGlobalBounds().height - 90});
    rect.setOrigin(-50, -50);
    rect.setPosition(sprite.getPosition());
    return rect;
}

Item BufferItem::getType() const {
    return type;
}


void BufferItem::setType(Item type) {
    this->type = type;
}