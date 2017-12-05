#include "snowSystem.h"

SnowSystem::SnowSystem()
{
    std::srand (time(NULL));
    m_flocTexture.loadFromFile("res/floc.png");
    m_movementDirection.x = 0;
    m_movementDirection.y = 100;

}

void SnowSystem::populate(int ammount, int winW, int winH)
{
    m_screenSize.x = winW;
    m_screenSize.y = winH;

    for(int i = 0; i < ammount; ++i) {
        m_flocs.emplace_back(sf::Sprite());
        sf::Sprite* currentFloc = &m_flocs.back();
        currentFloc->setTexture(m_flocTexture);
        currentFloc->setPosition((std::rand()%(2*m_screenSize.x))-m_screenSize.x,(std::rand()%(m_screenSize.y))-m_screenSize.y);
    }

    int left, top, width, height;
    for(int k = 0; k < 10; ++k){
        if(std::rand()%2 == 0){
            left = std::rand()%m_screenSize.x - m_screenSize.x;
            top = std::rand()%(m_screenSize.y);
            width = std::rand()%m_screenSize.x/4;
            height = std::rand()%m_screenSize.y/4;
            m_rectangles.emplace_back(sf::RectangleShape(sf::Vector2f(width, height)));
            m_rectangles.back().setPosition(left, top);
        }
    }
}

void SnowSystem::update(float dt)
{
    static float sinVar = 0;
    sinVar += dt;

    for(sf::RectangleShape& rect : m_rectangles){
        rect.move(300*dt,std::sin(sinVar)*100*dt);

        if(rect.getGlobalBounds().left > m_screenSize.x){
            rect.setPosition(sf::Vector2f(std::rand()%m_screenSize.x - m_screenSize.x, std::rand()%m_screenSize.y));
        }
    }

    for(sf::Sprite& floc: m_flocs){
        floc.move(m_movementDirection.x*dt, m_movementDirection.y*dt);

        for(sf::Shape& rect : m_rectangles){
            if(rect.getGlobalBounds().contains(floc.getPosition())){
                bool isWither = rect.getGlobalBounds().width > rect.getGlobalBounds().height;
                floc.move(rect.getGlobalBounds().width*isWither*dt, rect.getGlobalBounds().height*!isWither*dt);
            }
        }

        if(floc.getPosition().y > m_screenSize.y + 100){
            floc.setPosition((std::rand()%(2*m_screenSize.x))-m_screenSize.x,(std::rand()%(m_screenSize.y))-m_screenSize.y);
        }
    }


}

void SnowSystem::draw(sf::RenderTarget& window)
{
    for(sf::Sprite& floc: m_flocs){
        window.draw(floc);
    }

    for(sf::RectangleShape& rect : m_rectangles){
        rect.setOutlineColor(sf::Color::Green);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineThickness(2);
        window.draw(rect);
    }
}

