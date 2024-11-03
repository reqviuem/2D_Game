#include <SFML/Graphics.hpp>
#pragma once
class InstructionWindow {
    sf::RenderWindow instructionWindow;
    sf::Font font;
    sf::Text instructionText;
public:
    InstructionWindow() ;
    void Open() ;
};

