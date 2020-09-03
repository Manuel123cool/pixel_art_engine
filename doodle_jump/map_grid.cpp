#include "map_grid.h"
#include <fstream>
#include <iostream>


MapGrid::MapGrid(sf::RenderWindow &window, int size, int width, int height) : m_window(window)
{
    m_size = size; 
    m_width = width;
    m_height = height;
    m_goingUp = true;
    m_goingLeft = true;
    m_countMoves = 0;
    m_grid_to_right = 0;

    //define view grid
    //get vertical and horizontal count
    const int width1{ static_cast<int>(window.getSize().x) };
    const int height1{ static_cast<int>(window.getSize().y) };

    int verticalCount{ width1 / size }; 
    int horizontalCount{ (height1) / size }; 

    m_grid_to_up = m_height - horizontalCount;
    m_ballPos = m_height * m_width - (verticalCount / 2);  
    int arraySize{ verticalCount * horizontalCount };
    m_view_grid.resize(arraySize);
    
    //define all grid 
    int allArraySize{ m_width * m_height }; 
    m_all_grid.resize(allArraySize);
    
    std::ifstream inf{ "data/map" };  
     
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
        
        if  (count < m_all_grid.size() && count >= 5) 
        {
            int tmpCount{ count - 5 };
            m_all_grid.at(tmpCount) = extractRGB(strInput);  
        }
        
        count++;
    }
    m_ball_all_grid = m_all_grid;
    setNewViewport();
}



void MapGrid::update() 
{
    checkKeyPressed();
    drawBall();
    draw_rects();
}

void MapGrid::draw_rects() 
{
    int width{ static_cast<int>(m_window.getSize().x) };
    int height{ static_cast<int>(m_window.getSize().y) };

    int verticalCount{ width / m_size }; 
    int horizontalCount{ (height) / m_size }; 
 
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

void MapGrid::checkKeyPressed()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        m_goingLeft = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        m_goingLeft = false;
    }
}

void MapGrid::setNewViewport()
{
    int width{ static_cast<int>(m_window.getSize().x) };
    int height{ static_cast<int>(m_window.getSize().y) };

    int verticalCount{ width / m_size }; 
    int horizontalCount{ (height) / m_size }; 
    
    int count{ 0 };
    for (int i{ 0 }; i < horizontalCount; ++i)
    { 
        for (int j{ m_grid_to_right + i * m_width }; 
                j < m_grid_to_right + verticalCount + i * m_width; ++j)
        {
            int plusUp{ j + m_grid_to_up * m_width };
            m_view_grid[count] = m_ball_all_grid.at(plusUp);      
            ++count;  
        }
    }
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

void MapGrid::drawBall()
{
    int width{ static_cast<int>(m_window.getSize().x) };
    int verticalCount{ width / m_size }; 

    const int height1{ static_cast<int>(m_window.getSize().y) };
    int horizontalCount{ (height1) / m_size }; 

    m_ball_all_grid = m_all_grid;

    if (m_goingUp == false) 
    {
        if (m_all_grid[m_ballPos].red > 0 ||
                m_all_grid[m_ballPos].green > 0 ||
                     m_all_grid[m_ballPos].blue > 0)
        {
            static int countBla{ 0 };
            ++countBla;
            std::cout << "hallo" << countBla << std::endl;
            m_goingUp = true; 
            m_countMoves = 0;
        }
    }

    static int s_countFrames{ 0 };
    ++s_countFrames;
    if (s_countFrames == 10)
    {
        const int minimum{ m_width * m_height - ((horizontalCount / 2) * verticalCount) };
        if (m_ballPos < minimum) 
        {         
            if (m_goingUp && (m_grid_to_up - 1) >= 0)
                --m_grid_to_up;
            else if ((m_grid_to_up + 1) + horizontalCount <= m_height)
                ++m_grid_to_up;
        }

        if (m_goingLeft)
            m_ballPos -= 1;
        else
            m_ballPos += 1;

        if (m_goingUp)
            m_ballPos -= verticalCount;
        else 
            m_ballPos += verticalCount;

        s_countFrames = 0;
        ++m_countMoves;
        if (m_countMoves == 12)
        {
            m_goingUp = false;
            m_countMoves = 0;
        }
    }

    RGB rgb;
    rgb.red = 255;
    m_ball_all_grid.at(m_ballPos) = rgb;

    setNewViewport();
}
