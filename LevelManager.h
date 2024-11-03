
#pragma once
#include <SFML/Graphics.hpp>
#include "CharDraw.h"
#include "Goblin.h"
#include "FlyingEye.h"
#include "Skeleton.h"
#include "Mushroom.h"
#include "Beast.h"
#include "Kitsune.h"
#include "Karasu.h"
#include "Obstacle.h"
#include "BuffItem.h"

class LevelManager {
private:
    sf::Sprite extraImageSprite;
    sf::Texture extraImageTexture;
    std::vector<std::string> obstacleFilepaths;
    std::vector<sf::Texture> obstacleTextures;
    Goblin goblin;
    std::vector<Obstacle> obstacles;
    FlyingEye flyingEye;
    Skeleton skeleton;
    Mushroom mushroom;
    Beast beast;
    Kitsune kitsune;
    Karasu karasu;
    sf::Text gameText;
    CharDraw charDraw;

    sf::Texture backgroundTexture;
    sf::Texture whichLevelTexture;
    sf::Sprite whichLevelSprite;
    sf::Sprite backgroundSprite;
    sf::RenderWindow gameWindow;
    bool isLevelComplete;
    std::vector<sf::Texture> buffItemTextures;
    std::vector<BufferItem> buffItems;



public:
    void loadBuffItems(int currentLevel) ;
    int currentLevel;
    void loadObstacles(int level) ;
    void Open(sf::RenderWindow &window);

    LevelManager();

    void loadLevel(int level);

};

