#include "map_grid.h"
#include <fstream>
#include <iostream>


MapGrid::MapGrid(sf::RenderWindow &window, int size) : m_window(window)
{
    m_size = size; 
    m_activeFrame = 0;

    //define toolbar colors
    m_rgb_toolbar[0].red = 255; 
    
    m_rgb_toolbar[2].blue = 255; 

    m_rgb_toolbar[14].blue = 255; 

    m_rgb_toolbar[1].green = 255;

    //define view grid
    //get vertical and horizontal count
    const int width1{ static_cast<int>(window.getSize().x) };
    const int height1{ static_cast<int>(window.getSize().y) };

    int verticalCount{ width1 / size }; 
    int horizontalCount{ (height1 - 50) / size }; 
    int arraySize{ verticalCount * horizontalCount };
    
    std::cout << "Enter number of frames: " << std::endl;
    int frames;
    std::cin >> frames;
    m_view_grids.resize(frames);
    for (auto &elem : m_view_grids)
    {
       elem.resize(arraySize); 
    }
}



void MapGrid::update() 
{
    checkKeyPressed();
    draw_toolbar(); 
    draw_rects();
}

void MapGrid::draw_rects() 
{
    int width{ static_cast<int>(m_window.getSize().x) };
    int height{ static_cast<int>(m_window.getSize().y) };

    int verticalCount{ width / m_size }; 
    int horizontalCount{ (height - 50) / m_size }; 
 
    int countRect{ 0 };
    float sizeF{ static_cast<float>(m_size) };
    for (int i{ 0 }; i < horizontalCount; ++i)
    {
        for (int j{ 0 }; j < verticalCount; ++j)
        {
            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(sizeF, sizeF)); 
            rect.setPosition(sizeF * j, sizeF * i + 50.0f);
            rect.setFillColor(sf::Color(m_view_grids[m_activeFrame][countRect].red, 
                m_view_grids[m_activeFrame][countRect].green, 
                    m_view_grids[m_activeFrame][countRect].blue));

            m_window.draw(rect);
            
            ++countRect; 
        }
    }
}
void MapGrid::draw_toolbar()
{   
    int rectLength{ 16 };
    
    for (int i{ 0 }; i < rectLength; ++i)
    {
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(50.0f, 50.0f)); 
        rect.setPosition(50.0f * i, 0.0f);
        rect.setFillColor(sf::Color(m_rgb_toolbar[i].red, 
            m_rgb_toolbar[i].green, m_rgb_toolbar[i].blue));

        m_window.draw(rect);
    } 
}


void MapGrid::checkKeyPressed()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        //check key pressed toolbar
        if (sf::Mouse::getPosition(m_window).y < 50 && 
                sf::Mouse::getPosition(m_window).y > 0)
        { 
            int xRow{ 0 };
            for (int i{ 0 }; i < 16; ++i) 
            {
                if (i * 50 - sf::Mouse::getPosition(m_window).x < 0) 
                {
                   xRow++; 
                }
            }
            m_activeColor = m_rgb_toolbar[xRow - 1]; 
            return;
        }
        //check the map rect 
        if (sf::Mouse::getPosition(m_window).y > 50 &&
                sf::Mouse::getPosition(m_window).y < m_window.getSize().y
                  && sf::Mouse::getPosition(m_window).x > 0 &&
                      sf::Mouse::getPosition(m_window).x < m_window.getSize().x)
        {
            int xRow{ 0 };                   
            int yRow{ 0 };                   

            int width{ static_cast<int>(m_window.getSize().x) };
            int height{ static_cast<int>(m_window.getSize().y) };

            int verticalCount{ width / m_size }; 
            int horizontalCount{ (height - 50) / m_size }; 
         
            for (int i{ 0 }; i < verticalCount; ++i) 
            {
                if (i * m_size - sf::Mouse::getPosition(m_window).x < 0)
                {
                    ++xRow;       
                } 
            } 

            for (int i{ 0 }; i < horizontalCount; ++i) 
            {
                if (i * m_size - (sf::Mouse::getPosition(m_window).y - 50) < 0)
                {
                    ++yRow;       
                } 
            } 

            xRow--;
            yRow--;

            int index{ yRow * verticalCount + xRow };
            m_view_grids[m_activeFrame].at(index) = m_activeColor; 
        } 
    } 
    
    {
        static bool s_isClicket = false;
        bool wasClicket = s_isClicket;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            s_isClicket = true;
        else
            s_isClicket = false;

        if (s_isClicket && !wasClicket)
        {
            if (m_activeFrame + 1 < m_view_grids.size())
                ++m_activeFrame; 
            std::cout << "Frame: " << m_activeFrame << std::endl;
        }
    }

    {
        static bool s_isClicket = false;
        bool wasClicket = s_isClicket;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            s_isClicket = true;
        else
            s_isClicket = false;

        if (s_isClicket && !wasClicket)
        {
            if (m_activeFrame - 1 >= 0)
                --m_activeFrame; 
            std::cout << "Frame: " << m_activeFrame << std::endl;
        }
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
    {
        std::cout << "Enter which one to clone:" << std::endl;
        int toClone;
        std::cin >> toClone;

        std::cout << "Enter which one to clone into:" << std::endl;
        int cloneInto;
        std::cin >> cloneInto;
    
        if (toClone > m_view_grids.size() || toClone < 0)
        {
            std::cerr << "Frame number out of range";
            return;
        }
        
        if (cloneInto > m_view_grids.size() || cloneInto < 0)
        {
            std::cerr << "Frame number out of range";
            return;
        }
 
        m_view_grids[cloneInto] = m_view_grids[toClone]; 
        
    }
}

