#pragma once
#include <SFML/Graphics.hpp>
#include <array>

class MapGrid
{
public:
    struct RGB
    {
        int red{ 0 };
        int green{ 0 };
        int blue{ 0 };
    };
    void update();    
    MapGrid(sf::RenderWindow &window);
private:
    int m_width;
    int m_height;
    int m_view_width;
    int m_size;
    int extractNum(std::string string);
    RGB extractRGB(std::string string);
    void draw_rects();
    void checkKeyPressed();
    void setNewViewport();
    void moveViewToRight();
    void drawBird();
    sf::RenderWindow &m_window;
    std::vector<RGB> m_view_grid;
    std::vector<RGB> m_all_grid;
    int m_grid_to_right;
    int m_birdPos;
    bool m_goingUp;
    bool m_gameStop;
};
