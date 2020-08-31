#include "game.h"
#include <fstream>
#include <iostream>
#include <string>
MapGrid::MapGrid(sf::RenderWindow &window) : m_window(window)
{
    m_gameStop = false;
    m_birdPosFactor = 0;
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

    load();
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
        collisionDetection(); 
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
    static int s_howOften{ 0 };
    ++s_howOften;
    if (m_goingUp && s_howOften == 10)
    {
        --m_birdPosFactor;
        s_howOften = 0; 
        static int s_howOftenUp{ 0 };
        if (s_howOftenUp == 2)
        {
            m_goingUp = false;
            s_howOftenUp = 0; 
        }
        ++s_howOftenUp;
    } 
    else if (s_howOften == 10)
    {
        ++m_birdPosFactor;
        s_howOften = 0; 
    } 
    int whichFrame{ 0 };
    if (m_goingUp)
        whichFrame = 2;
    else
        whichFrame = 1;
    std::vector<int> birdPos{ rePosLowerOrHigher(reColorPos(whichFrame)) }; 
    std::vector<int> birdGridPos{ reColorPos(whichFrame) };
    
    for (int i{ 0 }; i < birdPos.size(); ++i)
    { 
        m_view_grid.at(birdPos.at(i)) = m_bird_grid.at(whichFrame).at(birdGridPos.at(i)); 
    } 
}

std::vector<int> MapGrid::reColorPos(int which)
{
    std::vector<int> birdPos; 
    for (int i{ 0 }; i < m_bird_grid.at(which).size(); ++i)
    {
        if (m_bird_grid.at(which).at(i).blue != 0 || 
                m_bird_grid.at(which).at(i).green != 0 || m_bird_grid.at(which).at(i).red != 0)
        { 
            birdPos.push_back(i);      
        }
    } 
    return birdPos;
}

void MapGrid::load()
{
    std::ifstream inf{"animation/bird"};  
     
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
            m_bird_grid.resize(extractNum(strInput)); 
        }
        else if (count == 1)
        {
            for (auto &elem : m_bird_grid)
            {
                elem.resize(extractNum(strInput)); 
            }
        }
        else if (count == 2)
        {
            m_size = extractNum(strInput); 
        }
        else if (count < m_bird_grid[0].size() * m_bird_grid.size() + 3)
        {
            int tmpCount{(count - 3) - 
                    extractWhichArray(strInput) * static_cast<int>(m_bird_grid[0].size())};
            m_bird_grid.at(extractWhichArray(strInput)).at(tmpCount) = extractRGB_bird(strInput);
        }
        
        count++;
    }
}

int MapGrid::extractWhichArray(std::string string)
{
    std::string tmpString;

    for(const auto &c : string) {
        if (c == '(')
            break;
        tmpString += c;
    }
    return extractNum(tmpString);
}

MapGrid::RGB MapGrid::extractRGB_bird(std::string string)
{
    int whichColorCount{ 0 };
    RGB rgb;
    std::string tmpString;

    for(const auto &c : string) {
        if (c == ')' && whichColorCount == 3)
        {
            rgb.blue = extractNum(tmpString);
        }
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

        if (c == '(')
        {
            whichColorCount = 1; 
        }
    }

    return rgb;
}


std::vector<int> MapGrid::rePosLowerOrHigher(std::vector<int> pos)
{
    std::vector<int> newPos;
    for (const auto &elem : pos)
    {
        newPos.push_back(elem + m_view_width * m_birdPosFactor);
    }        
    return newPos;
}

void MapGrid::collisionDetection()
{
    setNewViewport(); 
    int whichFrame{ 0 };
    if (m_goingUp)
        whichFrame = 2;
    else
        whichFrame = 1;

    std::vector<int> pos{ rePosLowerOrHigher(reColorPos(whichFrame)) }; 
    for (const auto &elem : pos)
    {
        if (m_view_grid[elem].red > 0 || 
                m_view_grid[elem].green > 0 || 
                    m_view_grid[elem].blue > 0)
        {
            m_gameStop = true;
            std::cout << "Game over" << elem << std::endl;
            return;
        }
    }
}
