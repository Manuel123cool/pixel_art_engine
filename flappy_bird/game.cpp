#include "game.h"
#include <fstream>
#include <iostream>
#include <string>
MapGrid::MapGrid(sf::RenderWindow &window) : m_window(window)
{
    m_gameStop = false;
    m_goingUp = false;
    std::ifstream inf{ "maps/map" };  
     
    if (!inf)
    {
        // Print an error and exit
        std::cerr << "Uh oh, Sample.dat could not be opened for reading!\n";
        return;
    }
    
    int count{ 0 };

    while (inf)
    {
        std::string strInput;
        std::getline(inf, strInput);
        
        if (count == 0)
        {
            m_all_grid.resize(extractNum(strInput));
        } 
        else if (count == 1)
        {
            m_width = extractNum(strInput); 
        }
        else if (count == 2)
        {
            m_height = extractNum(strInput); 
        }
        else if (count == 3)
        {
            m_size = extractNum(strInput);
        }
        else if (count == 4)
        {
           m_view_width = extractNum(strInput);
        } 
        else if  (count < m_all_grid.size()) 
        {
            int tmpCount{ count - 5 };
            m_all_grid.at(tmpCount) = extractRGB(strInput);  
        }
        
        count++;
    }
    m_view_grid.resize( m_view_width * m_height );
    setNewViewport();

    m_birdPos = (m_view_width / 2) + m_view_width * (m_height / 2);
}

int MapGrid::extractNum(std::string string)
{
    int reNum{ 0 };
    int getLength{ 1 };
    for(const auto &c : string) {
        if (static_cast<int>(c) <= 57 && static_cast<int>(c) >= 48)
        {
            getLength *= 10; 
        }
    } 
    getLength /= 10;
    for(const auto &c : string) {
        if (static_cast<int>(c) <= 57 && static_cast<int>(c) >= 48)
        {
            reNum += getLength * (static_cast<int>(c) - 48);
            getLength /= 10;
        }
    } 

    return reNum; 
}


MapGrid::RGB MapGrid::extractRGB(std::string string)
{
    int whichColorCount{ 0 };
    RGB rgb;
    std::string tmpString;

    for(const auto &c : string) {
         if (c == ',' && whichColorCount == 2)
        {
            ++whichColorCount; 
            rgb.green = extractNum(tmpString);
            tmpString = "";
        }

        if (c == ',' && whichColorCount == 1)
        {
            ++whichColorCount; 
            rgb.red = extractNum(tmpString);
            tmpString = "";
        }

        if (whichColorCount > 0 && 
            static_cast<int>(c) <= 57 && static_cast<int>(c) >= 48)
            tmpString += c;

        if (c == ':')
        {
            whichColorCount = 1; 
        }
    }

    rgb.blue = extractNum(tmpString);
    tmpString = "";

    return rgb;
}

void MapGrid::update() 
{
    if (!m_gameStop)
    {
        checkKeyPressed();
        moveViewToRight(); 
        drawBird();
        draw_rects();
    }
}

void MapGrid::draw_rects() 
{
    int width{ static_cast<int>(m_window.getSize().x) };
    int height{ static_cast<int>(m_window.getSize().y) };

    int verticalCount{ width / m_size }; 
    int horizontalCount{ height / m_size }; 
 
    int countRect{ 0 };
    float sizeF{ static_cast<float>(m_size) };
    for (int i{ 0 }; i < horizontalCount; ++i)
    {
        for (int j{ 0 }; j < verticalCount; ++j)
        {
            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(sizeF, sizeF)); 
            rect.setPosition(sizeF * j, sizeF * i);
            rect.setFillColor(sf::Color(m_view_grid[countRect].red, 
                m_view_grid[countRect].green, m_view_grid[countRect].blue));

            m_window.draw(rect);
            
            ++countRect; 
        }
    }
}

void MapGrid::setNewViewport()
{
    int width{ static_cast<int>(m_window.getSize().x) };
    int height{ static_cast<int>(m_window.getSize().y) };

    int verticalCount{ width / m_size }; 
    int horizontalCount{ height / m_size }; 
    
    int count{ 0 };
    for (int i{ 0 }; i < horizontalCount; ++i)
    { 
        for (int j{ m_grid_to_right + i * m_width }; 
                j < m_grid_to_right + verticalCount + i * m_width; ++j)
        {
            m_view_grid[count] = m_all_grid.at(j);      
            ++count;  
        }
    }
}  

void MapGrid::checkKeyPressed()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        m_goingUp = true;
    } 
}

void MapGrid::moveViewToRight()
{
    static int s_countDelay{ 0 };
    if (s_countDelay == 5)
    {
        ++m_grid_to_right; 
        setNewViewport();
        s_countDelay = 0;
    }
    ++s_countDelay;
}

void MapGrid::drawBird()
{   
    RGB rgb; 
    rgb.red = 255; 
    m_view_grid.at(m_birdPos) = rgb;
    static int s_countDelay{ 0 };
    ++s_countDelay;    
    if (m_view_grid[m_birdPos + 1].red > 0 || 
            m_view_grid[m_birdPos + 1].green > 0 || m_view_grid[m_birdPos + 1].blue > 0)
    {
        std::cout << "Game over" << std::endl;
        m_gameStop = true;
    } 
    
    if (m_goingUp && s_countDelay == 20) 
    {
        m_birdPos -= m_view_width;
        if (m_view_grid[m_birdPos].red > 0 || 
            m_view_grid[m_birdPos].green > 0 || m_view_grid[m_birdPos].blue > 0)
        {
            std::cout << "Game over" << std::endl;
            m_gameStop = true;
        }

        static int s_howOftenUp{ 0 };
        if (s_howOftenUp == 2) 
        {
            m_goingUp = false;
            s_howOftenUp = 0;
        }
        ++s_howOftenUp;
        s_countDelay = 0;
    }
    else if (s_countDelay == 20)
    {
        m_birdPos += m_view_width;
        if (m_view_grid[m_birdPos].red > 0 || 
            m_view_grid[m_birdPos].green > 0 || m_view_grid[m_birdPos].blue > 0)
        {
            std::cout << "Game over" << std::endl;
            m_gameStop = true;
        }


        s_countDelay = 0;
    }
}
