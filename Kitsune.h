#pragma once

#include <string>
#include "Enemy.h"
#include "EnemyState.h"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "EnemyDirection.h"

class Kitsune : public Enemy {

    sf::Clock hitTimer;
    sf::Clock idleClock;
    sf::Clock attackClock;
    sf::Time idleDuration;
    sf::Time attackCooldown;
    sf::Clock animationClock;

    EnemyDirection currentDirection;
    EnemyState currentState;

    bool hasAttacked = false;
    bool deathAnimationFinished = false;
    bool attackPerformed = false;
    bool moved;
    float speed;

    std::vector<sf::Vector2f> waypoints;

    void followWaypoints(sf::Time dt) override;

    int currentWaypoint;
    int hp;
    int currentIdleFrame = 0;
    int currentWalkFrame = 0;
    int currentAttackFrame = 0;
    int currentHitFrame = 0;
    int currentDeadFrame = 0;

    sf::Texture idleTexture;
    sf::Texture walkTexture;
    sf::Texture attackTexture;
    sf::Texture walkLeftTexture;
    sf::Texture attackLeftTexture;
    sf::Texture takeHitTexture;
    sf::Texture dead;
    sf::Texture takeHit;
    sf::Sprite attackLeftSprite;

public:
    bool isKitsuneFacingCharacter;
    bool animationPerformed;
    bool damageDealt;
    bool kitsuneIsDead;
    bool lastAttackFrame;
    sf::Sprite sprite;

    void updateEnemyHit(sf::Time dt) override;

    void dealDamage(int amount) override;

    sf::RectangleShape getHitbox() const override;

    Kitsune();

    void updateEnemy(sf::Time dt, const sf::RenderWindow &window, CharDraw &charDraw) override;

    void drawEnemy(sf::RenderWindow &window, CharDraw &charDraw) override;


};

