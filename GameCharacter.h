#pragma once


#include <SFML/Graphics.hpp>
#include <string>
#include <filesystem>

struct GameCharacter {
   bool isAnimationFinished;
    sf::Texture texture;
    std::vector<sf::Sprite> sprites;
    std::string name;
    int currentFrame;
    sf::Clock clock;
    sf::Vector2f velocity;
    sf::Vector2f position;
    bool isFacingRight = true;
    sf::FloatRect getGlobalBounds() const;

    void keepInWindowBoundary(const sf::RenderWindow& window);

    GameCharacter(const std::string &name, const std::string &textureFile, int frameWidth, int frameHeight);


    void nextFrame() ;


    void draw(sf::RenderWindow &window) ;


    void update(sf::Time dt,const sf::RenderWindow& window) ;


    void flipCharacter() ;
    void resetFrame();
};


