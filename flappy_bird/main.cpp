#include <SFML/Graphics.hpp>
#include "game.h"
#include <iostream>

int main()
{
    int windowWidth = 800;
    int windowHeight = 600;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight),
        "Flappy_Bird", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    MapGrid mapGrid(window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
               if (event.key.code == sf::Keyboard::Escape)
                    window.close();
        }
        window.clear();
        mapGrid.update();
        window.display();
    }
    return 0;
}

