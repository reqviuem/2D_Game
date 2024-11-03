#include "CharDraw.h"
#include "fmt/core.h"
#include <iostream>
#include <cmath>
#include "Goblin.h"
#include "FlyingEye.h"
#include "Skeleton.h"
#include "LevelManager.h"
#include "Mushroom.h"
#include "Beast.h"
#include "Kitsune.h"
#include "Karasu.h"
#include "BuffItem.h"

CharDraw::CharDraw()
        : characterIdle("CharacterIdle",
                        "../Knight Hero/Combat Ready Idle.png", 22,
                        24),
          characterRun("CharacterRun", "../Knight Hero/Run.png", 22,
                       24),
          characterAttack("CharacterAttack",
                          "../Knight Hero/Attack 2.png", 40, 30),
          characterEvade("CharacterEvade",
                         "../Knight Hero/Shield Raise.png", 22, 24),
          characterTakeHit("CharacterTakehit",
                           "../Knight Hero/Hit Front.png", 22, 24),
          hp(100), damage(20) {
    heartTexture.loadFromFile("../Sprites/heart.png");

    for (int i = 0; i < 5; ++i) {
        hearts.emplace_back(heartTexture);
        hearts[i].setPosition((float) i * heartTexture.getSize().x * 2.f * 1.2f, 0.f);
        hearts[i].setScale(2.5f, 2.5f);
    }
    sf::Vector2f middleOfTheWindow(-70, 300);
    characterIdle.position = middleOfTheWindow;
    characterRun.position = middleOfTheWindow;
    characterAttack.position = middleOfTheWindow;
    characterEvade.position = middleOfTheWindow;
    characterTakeHit.position = middleOfTheWindow;
    evadeCooldown = sf::seconds(3.f);

}

void CharDraw::handleInput(sf::RenderWindow &window, sf::Event gameEvent, Goblin &goblin, FlyingEye &flyingEye,
                           LevelManager *levelManager, Skeleton &skeleton, Mushroom &mushroom, Beast &beast,
                           Kitsune &kitsune, Karasu &karasu) {
    prevPosition = characterIdle.position;
    if (state == CharacterState::TakeHit) {
        if (characterTakeHit.isAnimationFinished) {
            state = CharacterState::Idle;
        }
        return;
    }

    if (gameEvent.type == sf::Event::MouseButtonPressed && gameEvent.mouseButton.button == sf::Mouse::Left) {
        attack(goblin, flyingEye, skeleton, levelManager, mushroom, beast, kitsune, karasu);
        return;
    }


    static bool wasKeyPressed = false;
    sf::Time dt = clock.restart();

    static bool wasFacingRightWhenEvadeStarted;
    static bool wasRightMouseButtonPressed = false;
    bool isRightMouseButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);
    bool isMoving =
            sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::A);


    if (wasRightMouseButtonPressed && !isRightMouseButtonPressed) {
        if (isMoving) state = CharacterState::Moving;
        else state = CharacterState::Idle;
    }
    wasRightMouseButtonPressed = isRightMouseButtonPressed;

    if (state == CharacterState::Attacking) {
        if (attackTimer.getElapsedTime() > attackDuration) {
            state = CharacterState::Idle;
            characterRun.velocity = sf::Vector2f();
            characterIdle.velocity = sf::Vector2f();
        }
        return;
    }
    bool isHorizontalKeyPressed =
            sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool isVerticalKeyPressed =
            sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S);


    if (!isHorizontalKeyPressed && !isVerticalKeyPressed) {
        state = CharacterState::Idle;
        characterRun.velocity = sf::Vector2f();
        characterIdle.velocity = sf::Vector2f();
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {


        state = CharacterState::Evade;

        wasFacingRightWhenEvadeStarted = characterIdle.isFacingRight;
        if (wasFacingRightWhenEvadeStarted && !characterEvade.isFacingRight) {
            characterEvade.flipCharacter();

        } else if (!wasFacingRightWhenEvadeStarted && characterEvade.isFacingRight) {
            characterEvade.flipCharacter();

        }
        characterEvade.velocity.x = 200;

        characterRun.velocity = sf::Vector2f();
        characterIdle.velocity = sf::Vector2f();

        evadeClock.restart();

    }


    if (isMoving && state != CharacterState::Evade && state != CharacterState::TakeHit) {

        state = CharacterState::Moving;


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            characterRun.velocity.x = 200;
            characterIdle.velocity.x = 200;
            if (!characterRun.isFacingRight) {
                characterRun.flipCharacter();
                characterIdle.flipCharacter();

            }
            characterIdle.position.x += 200 * dt.asSeconds();
            characterRun.position.x += 200 * dt.asSeconds();


        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            characterRun.velocity.x = -200;
            characterIdle.velocity.x = -200;
            if (characterRun.isFacingRight) {
                characterRun.flipCharacter();
                characterIdle.flipCharacter();

            }
            characterIdle.position.x -= 200 * dt.asSeconds();
            characterRun.position.x -= 200 * dt.asSeconds();


        } else {
            characterRun.velocity.x = 0;
            characterIdle.velocity.x = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            characterRun.velocity.y = -200;
            characterIdle.velocity.y = -200;
            characterIdle.position.y -= 200 * dt.asSeconds();
            characterRun.position.y -= 200 * dt.asSeconds();

        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            characterRun.velocity.y = 200;
            characterIdle.velocity.y = 200;
            characterIdle.position.y += 200 * dt.asSeconds();
            characterRun.position.y += 200 * dt.asSeconds();


        } else {
            characterRun.velocity.y = 0;
            characterIdle.velocity.y = 0;
        }


        switch (levelManager->currentLevel) {
            case 0:

                if (!flyingEye.FlyingEyeIsDead && checkCollisionFlyingEye(flyingEye)) {
                    sf::RectangleShape playerHitbox = getHitbox();
                    sf::RectangleShape flyingEyeHitbox = flyingEye.getHitbox();
                    sf::FloatRect intersect;
                    playerHitbox.getGlobalBounds().intersects(flyingEyeHitbox.getGlobalBounds(), intersect);
                    if (intersect.width < intersect.height) {

                        if (playerHitbox.getGlobalBounds().left < flyingEyeHitbox.getGlobalBounds().left) {

                            characterIdle.position.x -= intersect.width;
                            characterRun.position.x -= intersect.width;

                        } else {

                            characterIdle.position.x += intersect.width;
                            characterRun.position.x += intersect.width;

                        }
                    } else {

                        if (playerHitbox.getGlobalBounds().top < flyingEyeHitbox.getGlobalBounds().top) {

                            characterIdle.position.y -= intersect.height;
                            characterRun.position.y -= intersect.height;

                        } else {

                            characterIdle.position.y += intersect.height;
                            characterRun.position.y += intersect.height;

                        }
                    }

                }


                if (!goblin.goblinIsDead && checkCollision(goblin)) {
                    sf::RectangleShape playerHitbox = getHitbox();
                    sf::RectangleShape goblinHitBox = goblin.getHitbox();

                    sf::FloatRect intersect;
                    playerHitbox.getGlobalBounds().intersects(goblinHitBox.getGlobalBounds(), intersect);

                    if (intersect.width < intersect.height) {

                        if (playerHitbox.getGlobalBounds().left < goblinHitBox.getGlobalBounds().left) {

                            characterIdle.position.x -= intersect.width;
                            characterRun.position.x -= intersect.width;

                        } else {

                            characterIdle.position.x += intersect.width;
                            characterRun.position.x += intersect.width;

                        }
                    } else {

                        if (playerHitbox.getGlobalBounds().top < goblinHitBox.getGlobalBounds().top) {

                            characterIdle.position.y -= intersect.height;
                            characterRun.position.y -= intersect.height;

                        } else {

                            characterIdle.position.y += intersect.height;
                            characterRun.position.y += intersect.height;

                        }
                    }

                }


                if (!mushroom.MashroomIsDead && checkCollisionMushroom(mushroom)) {
                    sf::RectangleShape playerHitbox = getHitbox();
                    sf::RectangleShape mushroomHitBox = mushroom.getHitbox();

                    sf::FloatRect intersect;
                    playerHitbox.getGlobalBounds().intersects(mushroomHitBox.getGlobalBounds(), intersect);

                    if (intersect.width < intersect.height) {

                        if (playerHitbox.getGlobalBounds().left < mushroomHitBox.getGlobalBounds().left) {

                            characterIdle.position.x -= intersect.width;
                            characterRun.position.x -= intersect.width;

                        } else {

                            characterIdle.position.x += intersect.width;
                            characterRun.position.x += intersect.width;

                        }
                    } else {

                        if (playerHitbox.getGlobalBounds().top < mushroomHitBox.getGlobalBounds().top) {

                            characterIdle.position.y -= intersect.height;
                            characterRun.position.y -= intersect.height;

                        } else {

                            characterIdle.position.y += intersect.height;
                            characterRun.position.y += intersect.height;

                        }
                    }
                }


                break;

            case 1:

                if (!skeleton.SkeletonIsDead && checkCollisionSkeleton(skeleton)) {
                    sf::RectangleShape playerHitbox = getHitbox();
                    sf::RectangleShape skeletonHitBox = skeleton.getHitbox();

                    sf::FloatRect intersect;
                    playerHitbox.getGlobalBounds().intersects(skeletonHitBox.getGlobalBounds(), intersect);

                    if (intersect.width < intersect.height) {

                        if (playerHitbox.getGlobalBounds().left < skeletonHitBox.getGlobalBounds().left) {

                            characterIdle.position.x -= intersect.width;
                            characterRun.position.x -= intersect.width;

                        } else {

                            characterIdle.position.x += intersect.width;
                            characterRun.position.x += intersect.width;

                        }
                    } else {

                        if (playerHitbox.getGlobalBounds().top < skeletonHitBox.getGlobalBounds().top) {
                            characterIdle.position.y -= intersect.height;
                            characterRun.position.y -= intersect.height;

                        } else {

                            characterIdle.position.y += intersect.height;
                            characterRun.position.y += intersect.height;

                        }
                    }
                }
                if (!kitsune.kitsuneIsDead && checkCollisionKitsune(kitsune)) {

                    sf::RectangleShape playerHitbox = getHitbox();
                    sf::RectangleShape kitsuneHitBox = kitsune.getHitbox();

                    sf::FloatRect intersect;
                    playerHitbox.getGlobalBounds().intersects(kitsuneHitBox.getGlobalBounds(), intersect);

                    if (intersect.width < intersect.height) {

                        if (playerHitbox.getGlobalBounds().left < kitsuneHitBox.getGlobalBounds().left) {

                            characterIdle.position.x -= intersect.width;
                            characterRun.position.x -= intersect.width;
                        } else {

                            characterIdle.position.x += intersect.width;
                            characterRun.position.x += intersect.width;
                        }
                    } else {

                        if (playerHitbox.getGlobalBounds().top < kitsuneHitBox.getGlobalBounds().top) {

                            characterIdle.position.y -= intersect.height;
                            characterRun.position.y -= intersect.height;

                        } else {

                            characterIdle.position.y += intersect.height;
                            characterRun.position.y += intersect.height;

                        }
                    }
                }

                if (!karasu.karasuIsDead && checkCollisionKarasu(karasu)) {
                    sf::RectangleShape playerHitbox = getHitbox();
                    sf::RectangleShape karasuHitBox = karasu.getHitbox();

                    sf::FloatRect intersect;
                    playerHitbox.getGlobalBounds().intersects(karasuHitBox.getGlobalBounds(), intersect);

                    if (intersect.width < intersect.height) {

                        if (playerHitbox.getGlobalBounds().left < karasuHitBox.getGlobalBounds().left) {

                            characterIdle.position.x -= intersect.width;
                            characterRun.position.x -= intersect.width;

                        } else {

                            characterIdle.position.x += intersect.width;
                            characterRun.position.x += intersect.width;

                        }
                    } else {

                        if (playerHitbox.getGlobalBounds().top < karasuHitBox.getGlobalBounds().top) {

                            characterIdle.position.y -= intersect.height;
                            characterRun.position.y -= intersect.height;

                        } else {

                            characterIdle.position.y += intersect.height;
                            characterRun.position.y += intersect.height;

                        }
                    }
                }
                break;
            case 2:
                if (!beast.BeastIsDead && checkCollisionBeast(beast)) {

                    sf::RectangleShape playerHitbox = getHitbox();
                    sf::RectangleShape beastHitBox = beast.getHitbox();

                    sf::FloatRect intersect;
                    playerHitbox.getGlobalBounds().intersects(beastHitBox.getGlobalBounds(), intersect);

                    if (intersect.width < intersect.height) {

                        if (playerHitbox.getGlobalBounds().left < beastHitBox.getGlobalBounds().left) {

                            characterIdle.position.x -= intersect.width;
                            characterRun.position.x -= intersect.width;

                        } else {

                            characterIdle.position.x += intersect.width;
                            characterRun.position.x += intersect.width;

                        }
                    } else {

                        if (playerHitbox.getGlobalBounds().top < beastHitBox.getGlobalBounds().top) {

                            characterIdle.position.y -= intersect.height;
                            characterRun.position.y -= intersect.height;

                        } else {

                            characterIdle.position.y += intersect.height;
                            characterRun.position.y += intersect.height;

                        }
                    }
                }
        }


    }


    characterAttack.position = characterIdle.position;
    characterEvade.position = characterIdle.position;
    characterTakeHit.position = characterIdle.position;

    characterIdle.keepInWindowBoundary(window);
    characterRun.keepInWindowBoundary(window);
    characterAttack.keepInWindowBoundary(window);
    characterEvade.keepInWindowBoundary(window);
    characterTakeHit.keepInWindowBoundary(window);

    characterIdle.update(dt, window);
    characterRun.update(dt, window);
    characterAttack.update(dt, window);
    characterEvade.update(dt, window);
    characterTakeHit.update(dt, window);
}

void CharDraw::draw(sf::RenderWindow &window) {
    for (auto &heart: hearts) {
        window.draw(heart);
    }
    sf::Time dt = clock.restart();
    switch (state) {
        case CharacterState::Idle:
            characterIdle.nextFrame();
            characterIdle.draw(window);
            break;
        case CharacterState::Moving:
            characterRun.nextFrame();
            characterRun.draw(window);
            break;
        case CharacterState::Attacking:
            characterAttack.nextFrame();
            characterAttack.draw(window);
            break;
        case CharacterState::Evade:
            characterEvade.nextFrame();
            characterEvade.draw(window);
            break;
        case CharacterState::TakeHit:
            characterTakeHit.nextFrame();
            characterTakeHit.draw(window);

            if (characterTakeHit.isAnimationFinished) {
                state = CharacterState::Idle;
            }
            break;

    }

}

void CharDraw::attack(Goblin &attackedGoblin, FlyingEye &flyingEye, Skeleton &skeleton, LevelManager *levelManager,
                      Mushroom &mushroom, Beast &beast, Kitsune &kitsune, Karasu &karasu) {
    static sf::Clock clickClock;
    static bool wasPressed = false;

    bool isLeftMouseButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    if (!isLeftMouseButtonPressed) {
        wasPressed = false;
        return;
    }

    sf::Time elapsedTime = clickClock.getElapsedTime();

    if (!wasPressed || elapsedTime.asMilliseconds() >= 300) {
        clickClock.restart();

        if (state != CharacterState::Attacking) {
            characterAttack.position = characterIdle.position;
            characterAttack.resetFrame();
            attackTimer.restart();


            if (!characterIdle.isFacingRight) {

                if (characterAttack.isFacingRight) {
                    characterAttack.flipCharacter();
                }
            } else {

                if (!characterAttack.isFacingRight) {
                    characterAttack.flipCharacter();
                }
            }

            state = CharacterState::Attacking;
            sf::RectangleShape attackHitbox = getHitbox();
            switch (levelManager->currentLevel) {
                case 0:
                    if (attackHitbox.getGlobalBounds().intersects(attackedGoblin.getHitbox().getGlobalBounds())) {

                        attackedGoblin.dealDamage(damage);
                    }
                    if (attackHitbox.getGlobalBounds().intersects(flyingEye.getHitbox().getGlobalBounds())) {

                        flyingEye.dealDamage(damage);
                    }

                    if (attackHitbox.getGlobalBounds().intersects(mushroom.getHitbox().getGlobalBounds())) {

                        mushroom.dealDamage(damage);
                    }
                    break;
                case 1:
                    if (attackHitbox.getGlobalBounds().intersects(skeleton.getHitbox().getGlobalBounds())) {

                        skeleton.dealDamage(damage);
                    }
                    if (attackHitbox.getGlobalBounds().intersects(kitsune.getHitbox().getGlobalBounds())) {

                        kitsune.dealDamage(damage);
                    }
                    if (attackHitbox.getGlobalBounds().intersects(karasu.getHitbox().getGlobalBounds())) {

                        karasu.dealDamage(damage);
                    }

                    break;
                case 2:
                    if (attackHitbox.getGlobalBounds().intersects(beast.getHitbox().getGlobalBounds())) {
                        beast.dealDamage(damage);
                    }
            }


        }

        wasPressed = true;
    }
}

sf::Vector2f CharDraw::getPosition() const {
    return characterIdle.position;
}

void CharDraw::Respawn() {

    sf::Vector2f initialPosition(-70, 400);

    characterIdle.position = initialPosition;
    characterRun.position = initialPosition;
    characterAttack.position = initialPosition;
    characterTakeHit.position = initialPosition;
}

void CharDraw::dealDamage(int amount) {

    if (state == CharacterState::Evade) {
        return;
    }

    state = CharacterState::TakeHit;
    sf::Vector2f currentPosition = characterIdle.position;
    characterTakeHit.position = currentPosition;
    characterTakeHit.velocity = sf::Vector2f(0, 0);
    characterEvade.position = currentPosition;
    characterRun.position = currentPosition;
    characterAttack.position = currentPosition;
    hp -= amount;


    if (characterIdle.isFacingRight) {
        characterTakeHit.flipCharacter();
    }
    if (hp < 0) {
        hp = 0;
    }

    if (characterIdle.isFacingRight && !characterTakeHit.isFacingRight) {
        characterTakeHit.flipCharacter();
    } else if (!characterIdle.isFacingRight && characterTakeHit.isFacingRight) {
        characterTakeHit.flipCharacter();
    }

    switch (hp) {
        case 80:
            hearts.pop_back();
            break;
        case 70:
            hearts.pop_back();
            break;
        case 50:
            hearts.pop_back();
            break;
        case 30:
            hearts.pop_back();
            break;
        case 10:
            hearts.pop_back();
            break;
    }
}


sf::RectangleShape CharDraw::getHitbox() const {
    sf::RectangleShape rect;
    rect.setSize({characterIdle.getGlobalBounds().width - 120, characterIdle.getGlobalBounds().height - 140});
    rect.setOrigin(-10, -30);
    rect.setPosition(characterIdle.position);
    return rect;
}

bool CharDraw::checkCollision(Goblin &goblin) const {

    sf::RectangleShape playerHitbox = getHitbox();
    sf::RectangleShape goblinHitbox = goblin.getHitbox();
    return playerHitbox.getGlobalBounds().intersects(goblinHitbox.getGlobalBounds());

}

bool CharDraw::checkCollisionFlyingEye(FlyingEye &flyingEye) const {
    sf::RectangleShape playerHitbox = getHitbox();
    sf::RectangleShape flyingEyeHitbox = flyingEye.getHitbox();
    return playerHitbox.getGlobalBounds().intersects(flyingEyeHitbox.getGlobalBounds());
}

bool CharDraw::checkCollisionSkeleton(Skeleton &skeleton) const {
    sf::RectangleShape playerHitbox = getHitbox();
    sf::RectangleShape SkeletonHitbox = skeleton.getHitbox();
    return playerHitbox.getGlobalBounds().intersects(SkeletonHitbox.getGlobalBounds());
}

bool CharDraw::checkCollisionMushroom(Mushroom &mushroom) const {
    sf::RectangleShape playerHitbox = getHitbox();
    sf::RectangleShape SkeletonHitbox = mushroom.getHitbox();
    return playerHitbox.getGlobalBounds().intersects(SkeletonHitbox.getGlobalBounds());
}

bool CharDraw::checkCollisionBeast(Beast &beast) const {
    sf::RectangleShape playerHitbox = getHitbox();
    sf::RectangleShape beastHitbox = beast.getHitbox();
    return playerHitbox.getGlobalBounds().intersects(beastHitbox.getGlobalBounds());
}

bool CharDraw::checkCollisionKitsune(Kitsune &kitsune) const {
    sf::RectangleShape playerHitbox = getHitbox();
    sf::RectangleShape beastHitbox = kitsune.getHitbox();
    return playerHitbox.getGlobalBounds().intersects(beastHitbox.getGlobalBounds());
}

bool CharDraw::checkCollisionKarasu(Karasu &karasu) const {
    sf::RectangleShape playerHitbox = getHitbox();
    sf::RectangleShape kirasuHitbox = karasu.getHitbox();
    return playerHitbox.getGlobalBounds().intersects(kirasuHitbox.getGlobalBounds());
}

void CharDraw::moveBack() {
    characterIdle.position = prevPosition;
    characterRun.position = prevPosition;
    characterAttack.position = prevPosition;
    characterEvade.position = prevPosition;
    characterTakeHit.position = prevPosition;
}

int CharDraw::getHp() {
    return hp;
}

void CharDraw::applyBuff(BufferItem &buffItem) {
    switch (buffItem.getType()) {
        case Item::HP:

            hp += 10;
            heart.setTexture(heartTexture);


            scaleNewHeart = 2.5f;
            heart.setScale(scaleNewHeart, scaleNewHeart);

            xPos = 0.f;
            yPos = 0.f;
            if (!hearts.empty()) {
                const sf::Sprite &lastHeart = hearts.back();
                xPos = lastHeart.getPosition().x + heartTexture.getSize().x * scaleNewHeart * 1.2f;
                yPos = lastHeart.getPosition().y;
            }
            heart.setPosition(xPos, yPos);

            hearts.push_back(heart);
            break;
        case Item::DAMAGE:
            damage += 15;

            break;

    }
}






