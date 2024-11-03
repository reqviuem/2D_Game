
#include "InstructionWindow.h"


#include <SFML/Graphics.hpp>


InstructionWindow::InstructionWindow() {
    instructionWindow.create(sf::VideoMode(700, 300), "Instructions");

    font.loadFromFile("../Text/Blockstepped.ttf");

    instructionText.setFont(font);
    instructionText.setString("Use W A S D in order to move\n"
                              "Use (W+A or W+D) or (S+A or S+D) to move diagonally.\n"
                              "Use LEFT button of the mouse in order to attack.\n"
                              "Use RIGHT button of the mouse in order to evade.");
    instructionText.setCharacterSize(24);  // in pixels
    instructionText.setFillColor(sf::Color::Red);
    instructionText.setStyle(sf::Text::Regular);
}

void InstructionWindow::Open() {
    while (instructionWindow.isOpen()) {
        sf::Event instructionEvent;
        while (instructionWindow.pollEvent(instructionEvent)) {
            if (instructionEvent.type == sf::Event::Closed) {
                instructionWindow.close();
            }
        }
        instructionWindow.clear(sf::Color::Black);


        instructionWindow.draw(instructionText);

        instructionWindow.display();
    }
}

