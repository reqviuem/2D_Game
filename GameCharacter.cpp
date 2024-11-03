#include <SFML/Graphics.hpp>
#include <string>
#include "GameCharacter.h"
#include "fmt/core.h"

GameCharacter::GameCharacter(const std::string &name, const std::string &textureFile, int frameWidth, int frameHeight)
        : name(name), currentFrame(0) {
    if (!texture.loadFromFile(textureFile)) {
        throw std::runtime_error("Failed to load texture file");
    }

    int textureWidth = texture.getSize().x;
    int frameCount = textureWidth / frameWidth;

    for (int i = 0; i < frameCount; ++i) {

        sf::Sprite sprite(texture, sf::IntRect(i * frameWidth, 0, frameWidth, frameHeight));
        sprite.setOrigin( 0.5f * frameWidth, 0.5f * frameHeight);
        sprite.scale(3.5f, 3.5f);
        sprites.push_back(sprite);
    }
}

void GameCharacter::nextFrame() {

    if (name == "CharacterAttack") {
    }
    if (clock.getElapsedTime().asSeconds() >= 1.0f / 10.0f) {
        if (!sprites.empty()) {
            currentFrame++;
            if (currentFrame >= sprites.size()) {
                isAnimationFinished=true;
                currentFrame = 0;
            }
            else
                isAnimationFinished=false;
        }
        clock.restart();
    }
}

void GameCharacter::draw(sf::RenderWindow &window) {
    window.draw(sprites[currentFrame]);
}

void GameCharacter::update(sf::Time dt,const sf::RenderWindow& window) {
    position += velocity * dt.asSeconds();
    keepInWindowBoundary(window);
    for (auto &sprite: sprites) {
        sprite.setPosition(position);
    }
}

void GameCharacter::flipCharacter() {
    if (isFacingRight) {
        for (auto &sprite: sprites) {
            sprite.setScale(-3.5f, sprite.getScale().y);
        }
        isFacingRight = false;
    } else {
        for (auto &sprite: sprites) {
            sprite.setScale(3.5f, sprite.getScale().y);
        }
        isFacingRight = true;
    }
}


void GameCharacter::resetFrame() {
    currentFrame = 0;
}
void GameCharacter::keepInWindowBoundary(const sf::RenderWindow &window) {

    sf::FloatRect spriteBounds = sprites[currentFrame].getGlobalBounds();

    float halfWidth = spriteBounds.width / 2.0f;
    float halfHeight = spriteBounds.height / 2.0f;
    float displacementLeftRight = 10.0f;
    float displacementForBottom=120.0f;
    float displacementForTop=220.0f;

    if (position.x - halfWidth < displacementLeftRight)
        position.x = halfWidth+displacementLeftRight;
    if (position.y - halfHeight < displacementForTop)
        position.y = displacementForTop + halfHeight;
    if (position.x + halfWidth > window.getSize().x-displacementLeftRight)
        position.x = window.getSize().x - halfWidth-displacementLeftRight;
    if (position.y + halfHeight > window.getSize().y-displacementForBottom)
        position.y = window.getSize().y - halfHeight - displacementForBottom;

}

sf::FloatRect GameCharacter::getGlobalBounds() const {
    return sprites[0].getGlobalBounds();
}







