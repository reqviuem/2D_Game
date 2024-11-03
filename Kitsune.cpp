
#include "Kitsune.h"
#include <cmath>
#include "fmt/core.h"
#include "GameCharacter.h"



Kitsune::Kitsune()     {
    hp=50;
    speed = 90.0f;
    currentWaypoint = 0;
    waypoints.push_back(sf::Vector2f(100, 200));
    waypoints.push_back(sf::Vector2f(1070, 200));
    waypoints.push_back(sf::Vector2f(600, 270));
    attackCooldown = sf::seconds(2.0f);
    isKitsuneFacingCharacter = false;
    idleDuration = sf::seconds(1.0f);
    idleClock.restart();
    sprite.setPosition(100, 200);
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
    currentState = EnemyState::Idle;
    attackPerformed = true;
    animationPerformed = false;
    damageDealt = false;
    lastAttackFrame=false;


    attackTexture.loadFromFile("../ENEMIES/Kitsune/Attack_3.png");
    idleTexture.loadFromFile("../ENEMIES/Kitsune/Idle.png");
    walkTexture.loadFromFile("../ENEMIES/Kitsune/Run.png");
    walkLeftTexture.loadFromFile("../ENEMIES/Kitsune/RunLeft.png");
    attackLeftTexture.loadFromFile("../ENEMIES/Kitsune/Attack_3left.png");
    dead.loadFromFile("../ENEMIES/Kitsune/Dead.png");
    takeHitTexture.loadFromFile("../ENEMIES/Kitsune/Hurt.png");
}

void Kitsune::updateEnemy(sf::Time dt, const sf::RenderWindow &window, CharDraw &charDraw) {

    if (currentState == EnemyState::Dead && deathAnimationFinished) {
        kitsuneIsDead=true;
        return;
    }

    sf::Time attackDuration = sf::seconds(0.5);

    sf::Vector2f characterPosition = charDraw.getPosition() + sf::Vector2f(22 / 2, 24 / 2);
    sf::Vector2f KitsunePosition = sprite.getPosition() + sf::Vector2f(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);


    if (characterPosition.x > KitsunePosition.x) {
        currentDirection = EnemyDirection::Right;
    } else {
        currentDirection = EnemyDirection::Left;
    }

    bool isKitsuneLeftFromCharacter = KitsunePosition.x > characterPosition.x;
    bool isKitsuneRightFromCharacter = KitsunePosition.x < characterPosition.x;

    sf::FloatRect KitsuneHitBox = getHitbox().getGlobalBounds();
    sf::FloatRect characterHitbox = charDraw.getHitbox().getGlobalBounds();
    bool hitboxesIntersect = KitsuneHitBox.intersects(characterHitbox);

    updateEnemyHit(dt);

    if (hitboxesIntersect && isKitsuneFacingCharacter) {
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

    if(isKitsuneLeftFromCharacter && currentDirection == EnemyDirection::Left) {
        isKitsuneFacingCharacter = true;
    }
    else if(isKitsuneRightFromCharacter && currentDirection == EnemyDirection::Right) {
        isKitsuneFacingCharacter = true;
    }
    else {
        isKitsuneFacingCharacter = false;
    }


    if (animationClock.getElapsedTime().asSeconds() > 0.1f) {
        if(currentState == EnemyState::Idle) {
            currentIdleFrame = (currentIdleFrame + 1) % 6;
        }
        else if(currentState == EnemyState::Attacking) {
            if(!animationPerformed) {
                currentAttackFrame = (currentAttackFrame + 1) % 4;
                if(currentAttackFrame == 2) {
                    lastAttackFrame = true;

                }else if(lastAttackFrame) {
                    animationPerformed = true;
                    lastAttackFrame = false;
                }
            }
        }
        else if(currentState == EnemyState::TakingHit) {

            currentHitFrame = (currentHitFrame + 1) % 3;
        }
        else if(currentState == EnemyState::Dead) {

            if (!deathAnimationFinished) {
                currentDeadFrame = (currentDeadFrame + 1) % 6;
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

void Kitsune::drawEnemy(sf::RenderWindow &window, CharDraw &charDraw) {

    switch (currentState) {
        case EnemyState::Idle:
            sprite.setTexture(idleTexture);
            sprite.setScale(1.0f, 1.0f);
            sprite.setTextureRect(sf::IntRect(currentIdleFrame * 128, 0, 128, 128));
            break;
        case EnemyState::Walk:
            if (!waypoints.empty() && sprite.getPosition().x < waypoints[currentWaypoint].x) {
                sprite.setTexture(walkTexture);
            } else {
                sprite.setTexture(walkLeftTexture);
            }
            sprite.setScale(1.0f, 1.0f);
            sprite.setTextureRect(sf::IntRect(currentWalkFrame * 128, 0, 128, 128));
            break;
        case EnemyState::Attacking:
            if (isKitsuneFacingCharacter && currentDirection == EnemyDirection::Left) {
                sprite.setTexture(attackLeftTexture);
            } else if (isKitsuneFacingCharacter && currentDirection == EnemyDirection::Right) {
                sprite.setTexture(attackTexture);
            }
            sprite.setScale(1.0f, 1.0f);
            sprite.setTextureRect(sf::IntRect(currentAttackFrame * 128, 0, 128, 128));
            break;
        case EnemyState::Dead:
            sprite.setTexture(dead);
            sprite.setScale(1.0f, 1.0f);
            sprite.setTextureRect(sf::IntRect(currentDeadFrame*128, 0, 128, 128));
            break;
        case EnemyState::TakingHit:
            sprite.setTexture(takeHitTexture);
            sprite.setScale(1.0f, 1.0f);
            sprite.setTextureRect(sf::IntRect(currentHitFrame * 128, 0, 128, 128));
            break;
    }
    window.draw(sprite);

}

void Kitsune::followWaypoints(sf::Time dt) {
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
sf::RectangleShape Kitsune::getHitbox() const {

    sf::RectangleShape rect;

    rect.setSize({sprite.getGlobalBounds().width-200, sprite.getGlobalBounds().height-50});
    rect.setOrigin(-110, -60);
    rect.setPosition(sprite.getPosition());
    return rect;
}
void Kitsune::dealDamage(int amount) {
    hp -= amount;
    if(hp <= 0) {
        hp = 0;
        currentState = EnemyState::Dead;
    } else {
        hitTimer.restart();
        currentState = EnemyState::TakingHit;
    }
}
void Kitsune::updateEnemyHit(sf::Time dt) {
    sf::Time hitDuration = sf::seconds(1);

    if (currentState == EnemyState::TakingHit) {

        if (hitTimer.getElapsedTime() > hitDuration) {
            currentState = EnemyState::Idle;
        }
    }
}
