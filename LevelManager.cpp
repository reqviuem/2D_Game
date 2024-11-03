

#include <thread>
#include "LevelManager.h"
#include "MainMenu.h"
#include "Item.h"
#include <random>

LevelManager::LevelManager()
        : currentLevel(0), gameWindow(sf::VideoMode(1280, 720), "Dark Souls", sf::Style::Titlebar | sf::Style::Close) {
    loadObstacles(0);
}


void LevelManager::loadLevel(int level) {
    charDraw.Respawn();
    currentLevel = level;

    std::vector<std::string> backgrounds = {
            "../BATTLEGROUNDS/BATTLEGROUND1.png",
            "../BATTLEGROUNDS/BATTLEGROUND2.png",
            "../BATTLEGROUNDS/BATTLEGROUND3.png"
    };
    std::vector<std::string> levels = {
            "../BATTLEGROUNDS/LEVEL1.png",
            "../BATTLEGROUNDS/LEVEL2.png",
            "../BATTLEGROUNDS/LEVEL3.png"
    };
    loadObstacles(currentLevel);
    loadBuffItems(currentLevel);


    if (currentLevel < levels.size()) {
        whichLevelTexture.loadFromFile(levels[currentLevel]);
        whichLevelSprite.setTexture(whichLevelTexture);
        whichLevelSprite.setScale(0.5f, 0.5f);
        whichLevelSprite.setPosition(40, 660);
    }

    backgroundTexture.loadFromFile(backgrounds[currentLevel]);
    backgroundSprite.setTexture(backgroundTexture);
    if (currentLevel == 2) {
        extraImageTexture.loadFromFile("../BATTLEGROUNDS/BOSS LEVEL.png");
        extraImageSprite.setTexture(extraImageTexture);
        extraImageSprite.setPosition(520, 640);
    }

}

void LevelManager::Open(sf::RenderWindow &window) {
    sf::Clock clock;

    sf::Event gameEvent;
    while (gameWindow.isOpen()) {
        if (charDraw.getHp() == 0) {
            return;
        }
        sf::Time dt = clock.restart();
        while (gameWindow.pollEvent(gameEvent)) {

            if (gameEvent.type == sf::Event::Closed) {
                gameWindow.close();
            }

            if (gameEvent.type == sf::Event::KeyPressed) {
                if (gameEvent.key.code == sf::Keyboard::X) {
                    if (currentLevel == 0 && goblin.goblinIsDead && flyingEye.FlyingEyeIsDead &&
                        mushroom.MashroomIsDead) {
                        isLevelComplete = true;
                    } else if (currentLevel == 1 && skeleton.SkeletonIsDead && karasu.karasuIsDead &&
                               kitsune.kitsuneIsDead) {
                        isLevelComplete = true;
                    } else
                        isLevelComplete = false;
                    if (isLevelComplete) {
                        if (currentLevel + 1 < 3) {
                            this->loadLevel(currentLevel + 1);

                        }
                    }

                }
            }
        }

        charDraw.handleInput(gameWindow, gameEvent, goblin, flyingEye, this, skeleton, mushroom, beast, kitsune,
                             karasu);

        if (currentLevel == 0) {
            goblin.updateEnemy(dt, gameWindow, charDraw);
            flyingEye.updateEnemy(dt, gameWindow, charDraw);
            mushroom.updateEnemy(dt, gameWindow, charDraw);
        }
        if (currentLevel == 1) {
            skeleton.updateEnemy(dt, gameWindow, charDraw);
            kitsune.updateEnemy(dt, gameWindow, charDraw);
            karasu.updateEnemy(dt, gameWindow, charDraw);


        }
        if (currentLevel == 2) {
            beast.updateEnemy(dt, gameWindow, charDraw);
        }


        gameWindow.clear();


        gameWindow.draw(backgroundSprite);
        gameWindow.draw(whichLevelSprite);
        if (currentLevel == 2) {
            gameWindow.draw(extraImageSprite);
        }

        for (auto &obstacle: obstacles) {
            if (obstacle.checkCollision(charDraw)) {
                charDraw.moveBack();

            }
            sf::RectangleShape charHitbox = charDraw.getHitbox();
            sf::RectangleShape goblinHitbox = obstacle.getHitbox();

//            charHitbox.setOutlineColor(sf::Color::Red);
//            charHitbox.setOutlineThickness(1.f);
//            gameWindow.draw(charHitbox);
//
//            goblinHitbox.setOutlineColor(sf::Color::Green);
//            goblinHitbox.setOutlineThickness(1.f);
//            gameWindow.draw(goblinHitbox);

            obstacle.draw(gameWindow);
        }
        for (auto &item: buffItems) {
            item.draw(gameWindow);
        }


        for (int i = 0; i < buffItems.size(); ++i) {
            if (buffItems[i].checkCollision(charDraw)) {
                charDraw.applyBuff(buffItems[i]);
                buffItems.erase(buffItems.begin() + i);
                --i;

            }
        }


        charDraw.draw(gameWindow);
        if (currentLevel == 0) {
            goblin.drawEnemy(gameWindow, charDraw);
            flyingEye.drawEnemy(gameWindow, charDraw);
            mushroom.drawEnemy(gameWindow, charDraw);
        }
        if (currentLevel == 1) {
            skeleton.drawEnemy(gameWindow, charDraw);

            kitsune.drawEnemy(gameWindow, charDraw);
            karasu.drawEnemy(gameWindow, charDraw);


        }
        if (currentLevel == 2) {
            beast.drawEnemy(gameWindow, charDraw);
//            sf::RectangleShape charHitbox = charDraw.getHitbox();
//            sf::RectangleShape goblinHitbox = beast.getHitbox();
//
//            charHitbox.setOutlineColor(sf::Color::Red);
//            charHitbox.setOutlineThickness(1.f);
//            gameWindow.draw(charHitbox);
//
//            goblinHitbox.setOutlineColor(sf::Color::Green);
//            goblinHitbox.setOutlineThickness(1.f);
//            gameWindow.draw(goblinHitbox);
        }

        gameWindow.display();
    }

}

void LevelManager::loadObstacles(int currentLevel) {
    std::vector<std::string> obstacleFilepaths = {
            "../Obstacle/Dark_red_ crystal1.png",
            "../Obstacle/Green_crystal1.png",

            "../Obstacle/Black_crystal1.png",
            "../Obstacle/Bones_shadow1_6.png"

    };
    obstacles.clear();
    obstacleTextures.clear();
    for (int i = 0; i < obstacleFilepaths.size(); ++i) {
        obstacleTextures.emplace_back();
        if (!obstacleTextures.back().loadFromFile(obstacleFilepaths[i])) {
            fmt::println("Failed to load obstacle texture...");
            return;
        }
    }


    srand(static_cast<unsigned int>(time(nullptr)));
    int randomIndex1 = rand() % obstacleTextures.size();
    int randomIndex2 = rand() % obstacleTextures.size();


    switch (currentLevel) {
        case 0:
            obstacles.push_back(Obstacle(obstacleTextures[randomIndex1], 1100, 480, 2.5f, 2.5f));
            obstacles.push_back(Obstacle(obstacleTextures[randomIndex2], 100, 200, 2.5f, 2.5f));
            obstacles.push_back(Obstacle(obstacleTextures[randomIndex2], 600, 350, 2.5f, 2.5f));
            break;
        case 1:
            obstacles.push_back(Obstacle(obstacleTextures[randomIndex2], 1100, 480, 2.5f, 2.5f));
            obstacles.push_back(Obstacle(obstacleTextures[randomIndex1], 1100, 200, 2.5f, 2.5f));
            obstacles.push_back(Obstacle(obstacleTextures[randomIndex2], 30, 200, 2.5f, 2.5f));
            obstacles.push_back(Obstacle(obstacleTextures[randomIndex1], 30, 480, 2.5f, 2.5f));
            obstacles.push_back(Obstacle(obstacleTextures[randomIndex1], 600, 350, 2.5f, 2.5f));
            break;
        case 2:
            obstacles.push_back(Obstacle(obstacleTextures[randomIndex2], 1100, 480, 2.5f, 2.5f));
            obstacles.push_back(Obstacle(obstacleTextures[randomIndex1], 1100, 220, 2.5f, 2.5f));
            obstacles.push_back(Obstacle(obstacleTextures[randomIndex1], 40, 230, 2.5f, 2.5f));
            obstacles.push_back(Obstacle(obstacleTextures[randomIndex2], 40, 480, 2.5f, 2.5f));
            break;

    }


}

void LevelManager::loadBuffItems(int currentLevel) {

    std::vector<std::string> buffItemFilepaths = {
            "../BufItems/Item__67.png",
            "../BufItems/Item__14.png"

    };

    std::vector<Item> intToItemType = {
            Item::HP,
            Item::DAMAGE
    };


    buffItems.clear();
    buffItemTextures.clear();


    for (const auto &filepath: buffItemFilepaths) {
        sf::Texture texture;
        texture.loadFromFile(filepath);
        buffItemTextures.push_back(texture);
    }


    srand(static_cast<unsigned int>(time(nullptr)));


    std::vector<sf::Vector2f> level0Positions{{60,   280},
                                              {1080, 550},
                                              {1080, 280}};
    std::vector<sf::Vector2f> level1Positions{{1110, 340},
                                              {630,  470},
                                              {400,  370}};
    std::vector<sf::Vector2f> level2Positions{{500,  270},
                                              {1080, 550},
                                              {1665, 600}};

    switch (currentLevel) {
        case 0:
            for (int i = 0; i < level0Positions.size(); i++) {
                int randomIndex = rand() % buffItemTextures.size();
                buffItems.push_back(
                        BufferItem(buffItemTextures[randomIndex], level0Positions[i].x, level0Positions[i].y, 2.5f,
                                   2.5f));
                buffItems.back().setType(intToItemType[randomIndex]);
            }
            break;
        case 1:
            for (int i = 0; i < level1Positions.size(); i++) {
                int randomIndex = rand() % buffItemTextures.size();
                buffItems.push_back(
                        BufferItem(buffItemTextures[randomIndex], level1Positions[i].x, level1Positions[i].y, 2.5f,
                                   2.5f));
                buffItems.back().setType(intToItemType[randomIndex]);
            }
            break;
        case 2:
            int randomIndex = rand() % buffItemTextures.size();
            int randomPosIndex = rand() % level2Positions.size();
            buffItems.push_back(BufferItem(buffItemTextures[randomIndex], level2Positions[randomPosIndex].x,
                                           level2Positions[randomPosIndex].y, 2.5f, 2.5f));
            buffItems.back().setType(intToItemType[randomIndex]);
            break;
    }

}


