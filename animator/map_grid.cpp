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
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        std::cout << "Name file name: ";
        std::string name;
        std::cin >> name;
        save(name);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
    {
        std::cout << "Name file name: ";
        std::string name;
        std::cin >> name;
        load(name);
    }
}

void MapGrid::save(std::string name)
{
    std::string fileDirName;
    fileDirName += "data/";
    fileDirName += name;
    std::ofstream outf(fileDirName);    
    if (!outf)
    {
        std::cerr << "Cant read file" << std::endl;
        return;
    }
    
    int width{ static_cast<int>(m_window.getSize().x) };
    int height{ static_cast<int>(m_window.getSize().y) };
    
    int verticalCount{ width / m_size }; 
    int horizontalCount{ (height - 50) / m_size }; 

    outf << "Number_of_frames:" << m_view_grids.size() << '\n'; 
    outf << "Array_size:" << m_view_grids[0].size() << '\n';
    outf << "Size:" << m_size << '\n';

    for (int i{ 0 }; i < m_view_grids.size(); ++i)
    {
        for (int j{ 0 }; j < m_view_grids[0].size(); ++j)
        {
            outf << i;
            outf << "(";  
            outf << m_view_grids[i][j].red; 
            outf << ","; 
            outf << m_view_grids[i][j].green; 
            outf << ","; 
            outf << m_view_grids[i][j].blue;
            outf << ") ";
            outf << '\n';
        }
    }
}

void MapGrid::load(std::string name)
{   
    std::string fileDirName;
    fileDirName += "data/";
    fileDirName += name;
 
    std::ifstream inf{fileDirName};  
     
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
            m_view_grids.resize(extractNum(strInput)); 
        }
        else if (count == 1)
        {
            for (auto &elem : m_view_grids)
            {
                elem.resize(extractNum(strInput)); 
            }
        }
        else if (count == 2)
        {
            m_size = extractNum(strInput); 
        }
        else if (count < m_view_grids[0].size() * m_view_grids.size() + 3)
        {
            int tmpCount{(count - 3) - extractWhichArray(strInput) * static_cast<int>(m_view_grids[0].size())};
            std::cout << tmpCount << std::endl;
            m_view_grids.at(extractWhichArray(strInput)).at(tmpCount) = extractRGB(strInput);
        }
        
        count++;
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

