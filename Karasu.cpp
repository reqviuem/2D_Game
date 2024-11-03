

#include "Karasu.h"
#include <cmath>
#include "fmt/core.h"
#include "GameCharacter.h"



Karasu::Karasu()    {
    hp=50;
    speed = 90.0f;
    currentWaypoint = 0;
    waypoints.push_back(sf::Vector2f(100, 480));
    waypoints.push_back(sf::Vector2f(600, 410));
    waypoints.push_back(sf::Vector2f(1070, 480));
    attackCooldown = sf::seconds(2.0f);
    isKarasuFacingCharacter = false;
    idleDuration = sf::seconds(1.0f);
    idleClock.restart();
    sprite.setPosition(100, 480);
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
    currentState = EnemyState::Idle;
    attackPerformed = true;
    animationPerformed = false;
    damageDealt = false;
    lastAttackFrame=false;

    attackTexture.loadFromFile("../ENEMIES/Karasu_tengu/Attack_1.png");
    idleTexture.loadFromFile("../ENEMIES/Karasu_tengu/Idle_2.png");
    walkTexture.loadFromFile("../ENEMIES/Karasu_tengu/Run.png");
    walkLeftTexture.loadFromFile("../ENEMIES/Karasu_tengu/Runleft.png");
    attackLeftTexture.loadFromFile("../ENEMIES/Karasu_tengu/Attack_1left.png");
    dead.loadFromFile("../ENEMIES/Karasu_tengu/Dead.png");
    takeHitTexture.loadFromFile("../ENEMIES/Karasu_tengu/Hurt.png");


}

void Karasu::updateEnemy(sf::Time dt, const sf::RenderWindow &window, CharDraw &charDraw) {

    if (currentState == EnemyState::Dead && deathAnimationFinished) {
        karasuIsDead=true;
        return;
    }

    sf::Time attackDuration = sf::seconds(0.5);

    sf::Vector2f characterPosition = charDraw.getPosition() + sf::Vector2f(22 / 2, 24 / 2);
    sf::Vector2f KarasuPosition = sprite.getPosition() + sf::Vector2f(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);


    if (characterPosition.x > KarasuPosition.x) {
        currentDirection = EnemyDirection::Right;
    } else {
        currentDirection = EnemyDirection::Left;
    }

    bool isKarasuLeftFromCharacter = KarasuPosition.x > characterPosition.x;
    bool isKarasuRightFromCharacter = KarasuPosition.x < characterPosition.x;

    sf::FloatRect KarasuHitBox = getHitbox().getGlobalBounds();
    sf::FloatRect characterHitbox = charDraw.getHitbox().getGlobalBounds();
    bool hitboxesIntersect = KarasuHitBox.intersects(characterHitbox);

    updateEnemyHit(dt);

    if (hitboxesIntersect && isKarasuFacingCharacter) {
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

    if(isKarasuLeftFromCharacter && currentDirection == EnemyDirection::Left) {
        isKarasuFacingCharacter = true;
    }
    else if(isKarasuRightFromCharacter && currentDirection == EnemyDirection::Right) {
        isKarasuFacingCharacter = true;
    }
    else {
        isKarasuFacingCharacter = false;
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

void Karasu::drawEnemy(sf::RenderWindow &window, CharDraw &charDraw) {

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
            if (isKarasuFacingCharacter && currentDirection == EnemyDirection::Left) {
                sprite.setTexture(attackLeftTexture);
            } else if (isKarasuFacingCharacter && currentDirection == EnemyDirection::Right) {
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

void Karasu::followWaypoints(sf::Time dt) {
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
sf::RectangleShape Karasu::getHitbox() const {

    sf::RectangleShape rect;

    rect.setSize({sprite.getGlobalBounds().width-200, sprite.getGlobalBounds().height-30});
    rect.setOrigin(-90, -60);
    rect.setPosition(sprite.getPosition());
    return rect;
}
void Karasu::dealDamage(int amount) {
    hp -= amount;
    fmt::print("Hp left :",hp,"{}{}");
    if(hp <= 0) {
        hp = 0;
        currentState = EnemyState::Dead;
    } else {
        hitTimer.restart();
        currentState = EnemyState::TakingHit;
    }
}
void Karasu::updateEnemyHit(sf::Time dt) {
    sf::Time hitDuration = sf::seconds(1);

    if (currentState == EnemyState::TakingHit) {

        if (hitTimer.getElapsedTime() > hitDuration) {
            currentState = EnemyState::Idle;
        }
    }
}
