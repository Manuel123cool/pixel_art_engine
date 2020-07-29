#include "map_grid.h"
#include <fstream>
#include <iostream>


MapGrid::MapGrid(sf::RenderWindow &window, int size, int width) : m_window(window)
{
    m_size = size; 
    m_width = width;
    m_grid_to_right = 0;

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
    m_view_grid.resize(arraySize);
    
    //define all grid 
    int allArraySize{ m_width * horizontalCount }; 
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
    setNewViewport();
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
            rect.setFillColor(sf::Color(m_view_grid[countRect].red, 
                m_view_grid[countRect].green, m_view_grid[countRect].blue));

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

            int index1{ yRow * m_width + xRow + m_grid_to_right}; 
            m_all_grid.at(index1) = m_activeColor;
            
            int index{ yRow * verticalCount + xRow };
            m_view_grid.at(index) = m_activeColor; 
        } 
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
    {
        int width{ static_cast<int>(m_window.getSize().x) };
        int verticalCount{ width / m_size }; 
        if (m_grid_to_right + 1 <= (m_width - verticalCount))
            m_grid_to_right++; 
        setNewViewport();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
    {
        if (m_grid_to_right - 1 >= 0)
            m_grid_to_right--;
        setNewViewport();
    }    
}

void MapGrid::setNewViewport()
{
    int width{ static_cast<int>(m_window.getSize().x) };
    int height{ static_cast<int>(m_window.getSize().y) };

    int verticalCount{ width / m_size }; 
    int horizontalCount{ (height - 50) / m_size }; 
    
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

void MapGrid::checkSave(sf::Event &event) 
{
    if (event.type == sf::Event::TextEntered)
    {
    if (event.text.unicode == 115)
            save();
    if (event.text.unicode == 110)
            reset();
    }
}

void MapGrid::save()
{
    std::ofstream outf("data/map");    
    if (!outf)
    {
        std::cerr << "Cant read file" << std::endl;
        return;
    }
    
    int width{ static_cast<int>(m_window.getSize().x) };
    int height{ static_cast<int>(m_window.getSize().y) };
    
    int verticalCount{ width / m_size }; 
    int horizontalCount{ (height - 50) / m_size }; 

    outf << "Array_size:" << m_all_grid.size() << '\n';
    outf << "Width:" << m_width << '\n';
    outf << "Heigth:" << horizontalCount << '\n';
    outf << "Size:" << m_size << '\n';
    outf << "m_view_width:" << verticalCount << '\n';
    
    int count{ 0 };
    for (const auto &element: m_all_grid)
    {
        outf << "RGB(" << count << "):"  << element.red << "," << element.green <<
            "," << element.blue << "\n";
        ++count;
    }
}

void MapGrid::reset()
{
    int count{ 0 };
    RGB rgb;
    rgb.blue;
    rgb.green;
    rgb.red;

    for (const auto &element: m_all_grid)
    {
        m_all_grid.at(count) = rgb;         
        ++count;
    }
    save();
    setNewViewport();
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
