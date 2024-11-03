
#include "Mushroom.h"
#include <cmath>
#include "GameCharacter.h"

Mushroom::Mushroom()   {
    hp=50;
    speed = 90.0f;
    currentWaypoint = 0;
    waypoints.push_back(sf::Vector2f(100, 60));
    waypoints.push_back(sf::Vector2f(800, 60));
    attackCooldown = sf::seconds(2.0f);
    isMushroomFacingCharacter = false;
    idleDuration = sf::seconds(1.0f);
    idleClock.restart();
    sprite.setPosition(100, 60);
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
    currentState = EnemyState::Idle;
    attackPerformed = true;
    animationPerformed = false;
    damageDealt = false;
    lastAttackFrame=false;


    attackTexture.loadFromFile("../ENEMIES/Mushroom/Attack.png");
    idleTexture.loadFromFile("../ENEMIES/Mushroom/Idle.png");
    walkTexture.loadFromFile("../ENEMIES/Mushroom/Run.png");
    walkLeftTexture.loadFromFile("../ENEMIES/Mushroom/RunLeft.png");
    attackLeftTexture.loadFromFile("../ENEMIES/Mushroom/AttackLeft.png");
    dead.loadFromFile("../ENEMIES/Mushroom/Death.png");
    takeHitTexture.loadFromFile("../ENEMIES/Mushroom/Take Hit.png");

}

void Mushroom::updateEnemy(sf::Time dt, const sf::RenderWindow &window, CharDraw &charDraw) {

    if (currentState == EnemyState::Dead && deathAnimationFinished) {
        MashroomIsDead=true;
        return;
    }

    sf::Time attackDuration = sf::seconds(0.5);

    sf::Vector2f characterPosition = charDraw.getPosition() + sf::Vector2f(22 / 2, 24 / 2);
    sf::Vector2f MushroomPosition = sprite.getPosition() + sf::Vector2f(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);


    if (characterPosition.x > MushroomPosition.x) {
        currentDirection = EnemyDirection::Right;
    } else {
        currentDirection = EnemyDirection::Left;
    }

    bool isMushroomLeftFromCharacter = MushroomPosition.x > characterPosition.x;
    bool isMushroomRightFromCharacter = MushroomPosition.x < characterPosition.x;

    sf::FloatRect MushroomHitBox = getHitbox().getGlobalBounds();
    sf::FloatRect characterHitbox = charDraw.getHitbox().getGlobalBounds();
    bool hitboxesIntersect = MushroomHitBox.intersects(characterHitbox);

    updateEnemyHit(dt);

    if (hitboxesIntersect && isMushroomFacingCharacter) {
        if (currentState != EnemyState::Attacking && attackClock.getElapsedTime() > attackCooldown) {
            currentState = EnemyState::Attacking;
            attackClock.restart();
            animationPerformed = false;
            damageDealt = false;
        }
        else if (currentState == EnemyState::Attacking) {
            if (animationPerformed && damageDealt
                && attackClock.getElapsedTime() > attackDuration) {
                currentState = EnemyState::Idle;
                idleClock.restart();
                animationPerformed = false;
                damageDealt = false;
            }
            else if (!damageDealt && animationPerformed
                     && attackClock.getElapsedTime() > attackDuration / 2.0f) {
                charDraw.dealDamage(10);
                damageDealt = true;
            }
        }
    }
    else {
        currentState = EnemyState::Walk;
        followWaypoints(dt);
    }

    if(isMushroomLeftFromCharacter && currentDirection == EnemyDirection::Left) {
        isMushroomFacingCharacter = true;
    }
    else if(isMushroomRightFromCharacter && currentDirection == EnemyDirection::Right) {
       isMushroomFacingCharacter = true;
    }
    else {
        isMushroomFacingCharacter = false;
    }


    if (animationClock.getElapsedTime().asSeconds() > 0.1f) {
        if(currentState == EnemyState::Idle) {
            currentIdleFrame = (currentIdleFrame + 1) % 4;
        }
        else if(currentState == EnemyState::Attacking) {
            if(!animationPerformed) {
                currentAttackFrame = (currentAttackFrame + 1) % 8;
                if(currentAttackFrame == 6) {
                    lastAttackFrame = true;

                }else if(lastAttackFrame) {
                    animationPerformed = true;
                    lastAttackFrame = false;
                }
            }
        }
        else if(currentState == EnemyState::TakingHit) {

            currentHitFrame = (currentHitFrame + 1) % 4;
        }
        else if(currentState == EnemyState::Dead) {

            if (!deathAnimationFinished) {
                currentDeadFrame = (currentDeadFrame + 1) % 4;
                if (currentDeadFrame == 3) {
                    deathAnimationFinished = true;
                    currentDeadFrame = 3;
                }
            }
        }
        else {
            currentWalkFrame = (currentWalkFrame + 1) % 8;
        }

        animationClock.restart();
    }


}

void Mushroom::drawEnemy(sf::RenderWindow &window, CharDraw &charDraw) {

    switch (currentState) {
        case EnemyState::Idle:
            sprite.setTexture(idleTexture);
            sprite.setScale(2.5f, 2.5f);
            sprite.setTextureRect(sf::IntRect(currentIdleFrame * 150, 0, 150, 150));
            break;
        case EnemyState::Walk:
            if (!waypoints.empty() && sprite.getPosition().x < waypoints[currentWaypoint].x) {
                sprite.setTexture(walkTexture);
            } else {
                sprite.setTexture(walkLeftTexture);
            }
            sprite.setScale(2.5f, 2.5f);
            sprite.setTextureRect(sf::IntRect(currentWalkFrame * 150, 0, 150, 150));
            break;
        case EnemyState::Attacking:
            if (isMushroomFacingCharacter && currentDirection == EnemyDirection::Left) {
                sprite.setTexture(attackLeftTexture);
            } else if (isMushroomFacingCharacter && currentDirection == EnemyDirection::Right) {
                sprite.setTexture(attackTexture);
            }
            sprite.setScale(2.5f, 2.5f);
            sprite.setTextureRect(sf::IntRect(currentAttackFrame * 150, 0, 150, 150));
            break;
        case EnemyState::Dead:
            sprite.setTexture(dead);
            sprite.setScale(2.5f, 2.5f);
            sprite.setTextureRect(sf::IntRect(currentDeadFrame*150, 0, 150, 150));
            break;
        case EnemyState::TakingHit:
            sprite.setTexture(takeHitTexture);
            sprite.setScale(2.5f, 2.5f);
            sprite.setTextureRect(sf::IntRect(currentHitFrame * 150, 0, 150, 150));
            break;
    }
    window.draw(sprite);

}


void Mushroom::followWaypoints(sf::Time dt) {
    if(currentState == EnemyState::Attacking)
        return;

    if (currentWaypoint >= waypoints.size()) return;

    sf::Vector2f targetPosition = waypoints[currentWaypoint];
    sf::Vector2f direction = targetPosition - sprite.getPosition();
    float distance = std::hypot(direction.x, direction.y);

    if (distance < 10) {
        currentState = EnemyState::Idle;

        if(idleClock.getElapsedTime() > idleDuration) {
            currentWaypoint = (currentWaypoint + 1) % waypoints.size();
            idleClock.restart();
        }

        return;
    }

    if (targetPosition.x > sprite.getPosition().x) {
        currentDirection = EnemyDirection::Right;
    } else {
        currentDirection = EnemyDirection::Left;
    }


    direction /= distance;

    sprite.move(direction * speed * dt.asSeconds());

}
sf::RectangleShape Mushroom::getHitbox() const {

    sf::RectangleShape rect;

    rect.setSize({sprite.getGlobalBounds().width-300, sprite.getGlobalBounds().height-440});
    rect.setOrigin(-170, -250);
    rect.setPosition(sprite.getPosition());
    return rect;
}
void Mushroom::dealDamage(int amount) {
    hp -= amount;
    if(hp <= 0) {
        hp = 0;
        currentState = EnemyState::Dead;
    } else {
        hitTimer.restart();
        currentState = EnemyState::TakingHit;
    }
}
void Mushroom::updateEnemyHit(sf::Time dt) {
    sf::Time hitDuration = sf::seconds(1);

    if (currentState == EnemyState::TakingHit) {

        if (hitTimer.getElapsedTime() > hitDuration) {
            currentState = EnemyState::Idle;
        }
    }
}
