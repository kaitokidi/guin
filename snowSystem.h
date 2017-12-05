#ifndef SNOWSYSTEM_H
#define SNOWSYSTEM_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <stdio.h>

class SnowSystem
{

public:
    SnowSystem();

    void populate(int ammount, int winW, int winH);
    void update(float dt);
    void draw(sf::RenderTarget &window);

private:

    sf::Texture m_flocTexture;
    std::vector < sf::Sprite > m_flocs;

    std::vector < sf::RectangleShape > m_rectangles;

    sf::Vector2i m_screenSize;
    sf::Vector2f m_movementDirection;


};

#endif // SNOWSYSTEM_H
