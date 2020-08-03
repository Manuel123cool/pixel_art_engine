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
    RGB extractRGB_bird(std::string string);
    void draw_rects();
    void checkKeyPressed();
    void setNewViewport();
    void moveViewToRight();
    void drawBird();
    std::vector<int> reColorPos(int which);
    void load(); 
    int extractWhichArray(std::string string);
    std::vector<int> rePosLowerOrHigher(std::vector<int> pos);
    void collisionDetection();
    sf::RenderWindow &m_window;
    std::vector<RGB> m_view_grid;
    std::vector<RGB> m_all_grid;
    std::vector<std::vector<RGB> > m_bird_grid;
    int m_birdSize;
    int m_grid_to_right;
    bool m_gameStop;
    bool m_goingUp;
    int m_birdPosFactor;
};
