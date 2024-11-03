#pragma once


#include "CharacterState.h"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "GameCharacter.h"


class LevelManager;

class Goblin;

class FlyingEye;

class Skeleton;

class Mushroom;

class Beast;

class Kitsune;

class Karasu;

class BufferItem;

class CharDraw {
    int hp;
    int damage;
    float scaleNewHeart;
    float xPos;
    float yPos;
    sf::Vector2f prevPosition;
    sf::Clock evadeClock;
    sf::Time evadeCooldown;
    std::vector<sf::Sprite> hearts;
    sf::Texture heartTexture;
    sf::Sprite heart;
    sf::Clock attackTimer;
    sf::Clock hitTimer;
    sf::Time attackDuration = sf::seconds(1.0f);
    GameCharacter characterIdle;
    GameCharacter characterRun;
    GameCharacter characterAttack;
    GameCharacter characterEvade;
    GameCharacter characterTakeHit;
    sf::Clock clock, collisionClock;
    CharacterState state;

public:
    CharDraw();

    void moveBack();

    sf::RectangleShape getHitbox() const;

    void dealDamage(int amount);

    bool checkCollision(Goblin &goblin) const;

    bool checkCollisionFlyingEye(FlyingEye &FlyingEye) const;

    bool checkCollisionKitsune(Kitsune &kitsune) const;

    bool checkCollisionKarasu(Karasu &karasu) const;

    void handleInput(sf::RenderWindow &window, sf::Event gameEvent, Goblin &goblin, FlyingEye &FlyingEye,
                     LevelManager *levelManager, Skeleton &skeleton, Mushroom &mushroom, Beast &beast, Kitsune &kitsune,
                     Karasu &karasu);

    void draw(sf::RenderWindow &window);

    void attack(Goblin &attackedGoblin, FlyingEye &attackedFlyingEye, Skeleton &skeleton, LevelManager *levelManager,
                Mushroom &mushroom, Beast &beast, Kitsune &kitsune, Karasu &karasu);

    bool checkCollisionBeast(Beast &beast) const;

    bool checkCollisionMushroom(Mushroom &mushroom) const;

    sf::Vector2f getPosition() const;

    void Respawn();

    bool checkCollisionSkeleton(Skeleton &skeleton) const;

    int getHp();

    void applyBuff(BufferItem &buffItem);
};



