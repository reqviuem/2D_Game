#include <SFML/Graphics.hpp>
#include <iostream>
#include "Button.h"
#include "MainMenu.h"
#include "InstructionWindow.h"
#include "LevelManager.h"

int main() {
    try {
        MainMenu menu;
        menu.backgroundSetUp();
        menu.logoOfMenu();

        Button mainMenuButton;
        sf::FloatRect buttonBounds = mainMenuButton.getButton3().getGlobalBounds();
        sf::IntRect buttonRect(buttonBounds.left, buttonBounds.top, buttonBounds.width, buttonBounds.height);

        sf::FloatRect newGameButtonBounds = mainMenuButton.getButton1().getGlobalBounds();
        sf::IntRect newGameButtonRect(newGameButtonBounds.left, newGameButtonBounds.top, newGameButtonBounds.width,
                                      newGameButtonBounds.height);

        while (menu.window.isOpen()) {

            while (menu.window.isOpen()) {
                while (menu.window.pollEvent(menu.ev)) {
                    switch (menu.ev.type) {
                        case sf::Event::Closed:
                            menu.window.close();
                            break;

                        case sf::Event::KeyPressed:
                            if (menu.ev.key.code == sf::Keyboard::Escape) {
                                menu.window.close();
                            }
                            break;

                        case sf::Event::MouseButtonPressed:

                            if (buttonRect.contains(menu.ev.mouseButton.x, menu.ev.mouseButton.y)) {

                                InstructionWindow instruction;
                                instruction.Open();
                            }
                            if (newGameButtonRect.contains(menu.ev.mouseButton.x, menu.ev.mouseButton.y)) {
                                LevelManager game;
                                game.loadLevel(0);
                                game.Open(menu.window);


                            }
                            break;
                    }
                }
                menu.window.clear();
                menu.window.draw(menu.background);
                menu.window.draw(mainMenuButton.getButton1());
                menu.window.draw(mainMenuButton.getButton2());
                menu.window.draw(mainMenuButton.getButton3());
                menu.window.draw(menu.textSprite);
                menu.window.display();
            }

        }
    } catch (const std::runtime_error &e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}