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
    MapGrid(sf::RenderWindow &window, int size);
private:
    void draw_toolbar();
    void draw_rects();
    void checkKeyPressed();
    void save(std::string name);
    void load(std::string name); 
    int extractNum(std::string string);
    RGB extractRGB(std::string string);
    int extractWhichArray(std::string string);
    sf::RenderWindow &m_window;
    RGB m_rgb_toolbar[16] = { 0 };
    std::vector<std::vector<RGB> > m_view_grids;
    int m_size;
    RGB m_activeColor;
    int m_activeFrame;
};
