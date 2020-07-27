#pragma once
#include <SFML/Graphics.hpp>
#include <array>

class MapGrid
{
public:
    void update();    
    void checkSave(sf::Event &event);
    MapGrid(sf::RenderWindow &window, int size, int width);
private:
    void save();
    void draw_toolbar();
    void draw_rects();
    void checkKeyPressed();
    void setNewViewport();
    sf::RenderWindow &m_window;
    struct RGB
    {
        int red{ 0 };
        int green{ 0 };
        int blue{ 0 };
    };
    RGB m_rgb_toolbar[16] = { 0 };
    std::vector<RGB> m_view_grid;
    std::vector<RGB> m_all_grid;
    int m_grid_to_right;
    int m_size;
    int m_width;
    RGB m_activeColor;
};
