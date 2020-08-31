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
    void checkSave(sf::Event &event);
    MapGrid(sf::RenderWindow &window, int size, int width, int height);
private:
    void save();
    void draw_rects();
    void checkKeyPressed();
    void setNewViewport();
    int extractNum(std::string string);
    RGB extractRGB(std::string string);
    void drawBall();    
    sf::RenderWindow &m_window;
    std::vector<RGB> m_view_grid;
    std::vector<RGB> m_all_grid;
    int m_grid_to_right;
    int m_grid_to_up;
    int m_size;
    int m_height;
    int m_width;
    int m_ballPos;
    bool m_goingLeft;
    int m_goingUp;
    int m_countMoves;
};
