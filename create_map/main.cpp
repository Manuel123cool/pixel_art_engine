#include <SFML/Graphics.hpp>
#include "map_grid.h"
#include <iostream>

int main()
{
    int windowWidth = 800;
    int windowHeight = 650;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight),
        "Map_Creator", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
     
    MapGrid mapGrid(window, 25, 200);

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
            mapGrid.checkSave(event);
        }
        window.clear();
        mapGrid.update();    
        window.display();
    }
    return 0;
}
