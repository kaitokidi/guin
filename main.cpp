#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <stdio.h>
#include "moon.h"
#include "star.h"

bool isWhite(sf::Image& image, float px, float py){
	return image.getPixel(px, py) == sf::Color::White;
}

float getAngle(sf::Vector2f &orig, sf::Vector2i &des) {
    return std::atan2(des.y - orig.y, des.x - orig.x)*180/(M_PI);
}

float getModule(const sf::Vector2f &orig, const sf::Vector2f &des) {
    return std::sqrt(std::pow(std::abs(des.x-orig.x), 2) + std::pow(std::abs(des.y-orig.y), 2));
}

int randomSignOne(){
    return 1 - 2*std::rand()%2;
}

namespace State{
enum state {
  Idle, Speaking, Searching, GroupChat, fadeOut
};
}

float distance(const sf::Vector2f &orig, const sf::Vector2f &des) {
    return std::sqrt(std::pow(std::abs(des.x-orig.x), 2) + std::pow(std::abs(des.y-orig.y), 2));
}

float getAngle(const sf::Vector2f &orig,const sf::Vector2f &des) {
    return std::atan2(des.y - orig.y, des.x - orig.x)*180/(M_PI);
}

int main(){

    /* initialize random seed: */
    std::srand (time(NULL));

	//SFML OBJECTS
	sf::View view;
	sf::Event event;
	sf::Clock deltaClock;

    sf::CircleShape guia;
    guia.setRadius(10);
    guia.setFillColor(sf::Color(200,200,200,50));
    guia.setOrigin(guia.getLocalBounds().width/2, guia.getLocalBounds().height/2);

    float alpha = 0.0;
    float deltatime = 0.0;

    sf::Font font;
    font.loadFromFile("res/font.otf");

    int moonSentenceIndex = 0;
    std::vector <std::string> moonSentences;

    int groupIndex = 0;
    std::vector< std::vector <std::pair <float, std::string > > > groups =
    {
        {
            std::pair <float, std::string >(0, "Hi, been a long time!"),
            std::pair <float, std::string >(5, "I've missed you so much!")
        }
        ,
    /*    {
            std::pair <float, std::string >(0, "Glad you are here!"),
            std::pair <float, std::string >(6, "Oh, I love you! ^^ ")
        }
        ,
        {
        }
        ,
        {
            std::pair <float, std::string >(0, "Hey boys!"),
            std::pair <float, std::string >(6, "You are awesome!"),
            std::pair <float, std::string >(10, "So good to see you all!!!")
        }
        ,
        {
            std::pair <float, std::string >(0, "Loneliness fades away with you"),
            std::pair <float, std::string >(6, "We are all good together!")
        }
        ,
        {}
        ,*/
        {}
    };

    moonSentences.push_back("Sols veig la mort si no trobo \n un refugi de la tempesta...");
    moonSentences.push_back("Segueix-me! no tenim gaire temps!");
    moonSentences.push_back("Has to be so nice to be loved...");
    moonSentences.push_back("Veniu amb nosaltres! \n (Tot i que no estic segur de si ho aconseguirem");
    moonSentences.push_back("Seguim buscant tots junts \n (El grup confia en mi...)");
    moonSentences.push_back("No et rendeixis! Trobarem algun lloc! \n (O morirem congelats...)");
    moonSentences.push_back("...");
    moonSentences.push_back("...");
    moonSentences.push_back("...");
    moonSentences.push_back("Gel fred i cruel...");
    moonSentences.push_back("Fa tant de fred quee no em puc ni moure...");
    moonSentences.push_back("Perdoneu companys que us he fallat...");
    moonSentences.push_back("Deixeu al menys que us abraçi i morirem junts...");
//    moonSentences.push_back("Game made with love  \n by Genis Bayona");
  //  moonSentences.push_back("Music: Sad Day from \n Royalty Free Music \n Bensound. ");

    Moon moon("Ice Storm in Anctartica \n                          ", font);
    moon.setOrigin(moon.getLocalBounds().width/2, moon.getLocalBounds().height/2);

    std::vector < Star > stars;
    std::vector < Star > penguinsToFollow;

    State::state currentState = State::Idle;

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), L"The Moon", sf::Style::Resize|sf::Style::Close);

    view.reset(sf::FloatRect(0,0,
                             window.getSize().x, window.getSize().y));

    window.setFramerateLimit(30);


    sf::Texture endT;
    endT.loadFromFile("res/end.png");
    sf::Sprite endS;
    endS.setTexture(endT);
    float endScaleFactor = window.getSize().y/endS.getGlobalBounds().height;
    if(window.getSize().x < window.getSize().y) endScaleFactor = window.getSize().x/endS.getGlobalBounds().width;
    endS.setScale(endScaleFactor, endScaleFactor);
    endS.setOrigin(endS.getLocalBounds().width/2, endS.getLocalBounds().height/2);
    endS.setPosition(window.getSize().x/2, window.getSize().y/2);

    std::vector < sf::CircleShape > ministars;

    float fadeOutValue = 0;

    sf::Music m;
    m.openFromFile("res/music.ogg");
    m.setLoop(true);
    //m.play();

	//GAME LOOP
	while(window.isOpen()){

        //Deltatime
        deltatime = deltaClock.restart().asSeconds();

		//Loop for handling events
		while(window.pollEvent(event)){
			switch (event.type){
				//Close event
				case sf::Event::Closed:
					window.close();
					break;
				//KeyPressed event
				case  sf::Event::KeyPressed:
					//Close key
					if (event.key.code == sf::Keyboard::Escape) {
						window.close();
					}
                //Default
				default:
					//Do nothing
					break;
			}
		}	

sf::Vector2f groupPosition;
        switch(currentState){
        case State::Idle:
            //std::cout << "idle" << std::endl;
            if(moon.textBox.finished()){
                //std::cout << "boobs" << std::endl;
                if(moonSentenceIndex >= moonSentences.size()) moon.setSentence(" ");
                else moon.setSentence(moonSentences[moonSentenceIndex]);
                ++moonSentenceIndex;
                currentState = State::Speaking;
            }
            break;
        case State::Speaking:
            //std::cout << "speaking" << std::endl;
            if(moon.textBox.finished()){
                currentState = State::Searching;

                if(groupIndex < groups.size()){
                    auto group = groups[groupIndex];
                    ++groupIndex;

                    int sign1 = -1;
                    if( std::rand()%2 == 0) sign1 = 1;
                    int sign2 = -1;
                    if( std::rand()%2 == 0) sign2 = 1;
                    int sign3 = -1;
                    if( std::rand()%2 == 0) sign3 = 1;
                    int sign4 = -1;
                    if( std::rand()%2 == 0) sign4 = 1;

                    int w_size = static_cast<int>(window.getSize().y);
                    groupPosition = moon.getPosition() + sf::Vector2f((w_size/5 + std::rand()%w_size) * sign1,
                                                                    (w_size/5 + std::rand()%w_size) * sign2);


                    for(auto &pair: group){
                        stars.emplace_back(font, pair.first);
                        stars[stars.size()-1].setPosition(groupPosition+sf::Vector2f((20 + std::rand()%150) * sign3,
                                                                                     (20 + std::rand()%150) * sign4));
                        for(int i = 0; i < stars.size()-1; ++i){
                            Star* currentStarAdded = &stars.back();

                            if(currentStarAdded->getGlobalBounds().intersects(stars[i].getGlobalBounds())){
                                currentStarAdded->move(sf::Vector2f(stars[i].getGlobalBounds().width*randomSignOne(), stars[i].getGlobalBounds().height*randomSignOne()));
                            }
                        }

                        stars[stars.size()-1].setSentence(pair.second);
                    }
                }
                else {
                    int size = 20;
                    stars.emplace_back(font, 2);
                    stars[stars.size()-1].setPosition(sf::Vector2f(0.0,0.0));
                    stars[stars.size()-1].setSentence(": )");

                   /* stars.emplace_back(Star(font, 3));
                    stars[stars.size()-1].setPosition(sf::Vector2f(1*size,-1.42*size));
                    stars[stars.size()-1].setSentence("<3");

                    stars.emplace_back(Star(font, 3));
                    stars[stars.size()-1].setPosition(sf::Vector2f(-1*size,-1.42*size));
                    stars[stars.size()-1].setSentence("<3");

                    stars.emplace_back(Star(font, 4));
                    stars[stars.size()-1].setPosition(sf::Vector2f(2*size,-2.84*size));
                    stars[stars.size()-1].setSentence(": )");

                    stars.emplace_back(Star(font, 4));
                    stars[stars.size()-1].setPosition(sf::Vector2f(-2*size,-2.84*size));
                    stars[stars.size()-1].setSentence("<3");

                    stars.emplace_back(Star(font, 5));
                    stars[stars.size()-1].setPosition(sf::Vector2f(1.6*size,-3.9*size));
                    stars[stars.size()-1].setSentence(": )");

                    stars.emplace_back(Star(font, 3));
                    stars[stars.size()-1].setPosition(sf::Vector2f(-1.4*size,-3.9*size));
                    stars[stars.size()-1].setSentence("( :");

                    stars.emplace_back(Star(font, 6));
                    stars[stars.size()-1].setPosition(sf::Vector2f(0*size,-3.1*size));
                    stars[stars.size()-1].setSentence("<3");*/
                }
            }
            break;
        case State::Searching:
        {
            ////std::cout << "seraching the meaning of life" << moon.getPosition().x << "," <<moon.getPosition().y << std::endl;
            bool goToNextState = true;
            for(Star &s : stars){
               //// std::cout << "star -> " << s.getPosition().x << ";" << s.getPosition().y << std::endl;
                if(distance(moon.getPosition(), s.getPosition()) > window.getSize().y/2) goToNextState = false;
            }
            for(Star &s : stars){
                s.update(deltatime);
            }
            if(goToNextState){
                for(Star &s : stars){
                    s.setCanSpeak(true);
                }
                currentState = State::GroupChat;
            }

            break;
        }
        case State::GroupChat:
        {
            //std::cout << "groupchating (. Y .)" << std::endl;
            for(Star &s : stars){
                s.update(deltatime);
            }

            bool finished = true;
            for(Star &s : stars){
                if(!s.textBox.finished()) finished = false;
            }

            if(finished) {
                fadeOutValue = 0.0;
                currentState = State::fadeOut;
            }
            break;
        }
        case State::fadeOut:
            if(fadeOutValue <= 1){
                fadeOutValue += deltatime* 0.5*2;
                /*for(Star &s : stars){
                    s.setColor(sf::Color(255,255,255, 255-255*fadeOutValue));
                    s.sglow.setColor(sf::Color(255,255,255, 255-255*fadeOutValue));
                }*/
            }
            else {
                for(Star &s : stars){
                    penguinsToFollow.emplace_back(font,1000);
                    penguinsToFollow[penguinsToFollow.size()-1].setPosition(s.getPosition());
                    penguinsToFollow[penguinsToFollow.size()-1].setRotation(s.getRotation());
                    penguinsToFollow[penguinsToFollow.size()-1].setScale(s.getScale());
                }
                stars.clear();
                currentState = State::Idle;
            }
            break;
        default:
            break;
        }

        moon.update(deltatime, window);


        //followers
        sf::Vector2f destinationPoint = moon.getPosition();
        float m_speed = 40;
        for(Star& s : penguinsToFollow) {

            sf::Vector2f originalPosition = s.getPosition();
            float module = getModule(originalPosition,destinationPoint);
            float directionX = destinationPoint.x - originalPosition.x;
            float directionY = destinationPoint.y - originalPosition.y;
            sf::Vector2f movement = sf::Vector2f(directionX/module*m_speed*deltatime,directionY/module*m_speed*deltatime);
            s.move(movement);
            bool tooCloseToPlayer = false;
            if(getModule(s.getPosition(),destinationPoint) <
                    getModule(s.getPosition(), originalPosition)){
                s.move(sf::Vector2f(movement.x*-1,movement.y*-1));
                tooCloseToPlayer = true;
            }

            sf::FloatRect intersectionOfPenguins;
            if(!tooCloseToPlayer){
                for(Star& ss : penguinsToFollow) {
                    if(&s != &ss && s.getGlobalBounds().intersects(ss.getGlobalBounds(),intersectionOfPenguins)){
                        s.move(sf::Vector2f(
                                   movement.x-(intersectionOfPenguins.width < intersectionOfPenguins.height? intersectionOfPenguins.width:0),
                                   movement.y-(intersectionOfPenguins.width < intersectionOfPenguins.height? 0: intersectionOfPenguins.height)));
                    }
                }
            }

            float speedFactor2 = m_speed;
            s.setRotation(8*(speedFactor2/m_speed) *std::sin(s.movementAngle) );
            s.movementAngle += deltatime*speedFactor2/6;

            if(s.movementAngle >= 2*3.1415) s.movementAngle = 0;
        }


        //Set view values
        sf::Vector2f viewPosition = view.getCenter();

        view.reset(sf::FloatRect(viewPosition.x, viewPosition.y,
                                 window.getSize().x, window.getSize().y));

        sf::Vector2f step;
        step.x = (viewPosition.x - moon.getPosition().x+10) *-0.1* deltatime;
        step.y = (viewPosition.y - moon.getPosition().y+10) *-0.1* deltatime;

        float maxDist = 20;
        float dist = distance(viewPosition, moon.getPosition());
        sf::Vector2f extra(0.f,0.f);
        if( dist > maxDist){
            extra.x = (viewPosition.x - moon.getPosition().x+10) *-1* deltatime;
            extra.y = (viewPosition.y - moon.getPosition().y+10) *-1* deltatime;
        }


        view.setCenter(viewPosition + step + extra);
        view.setViewport(sf::FloatRect(0,0,1.0f,1.0f));

/*
        view.reset(sf::FloatRect(moon.getPosition().x, moon.getPosition().y,
                                 window.getSize().x, window.getSize().y));

        viewPosition.y = moon.getPosition().y+10;
        viewPosition.x = moon.getPosition().x+10;
        view.setCenter(viewPosition);
        view.setViewport(sf::FloatRect(0,0,1.0f,1.0f));
*/

		//Set window view, draw and display
        window.setView(view);

        sf::Color c = sf::Color(200,200,200,255);

        if(moonSentenceIndex >= moonSentences.size()) {
            if(moonSentenceIndex == moonSentences.size()) {
                ++moonSentenceIndex;
                alpha = 55;
            }
            alpha+=deltatime*10;
            c = sf::Color(std::max(0,255-int(alpha)),std::max(0,255-int(alpha)),std::max(0,255-int(alpha)),std::max(0,255-int(alpha)));
        }
        window.clear(c);


        for(auto it = ministars.begin(); it != ministars.end();){
            if(std::abs(it->getPosition().x - moon.getPosition().x) > window.getSize().x
                    || std::abs(it->getPosition().y - moon.getPosition().y) > window.getSize().y
                    ) it = ministars.erase(it);
            else ++it;
        }

        const int maxStars = 50;
        if(ministars.size() < maxStars){
            for(int i = ministars.size(); i < maxStars; ++i){
                sf::Vector2f pos;
                pos.x = rand()%window.getSize().x;
                pos.x -= window.getSize().x/2;
                pos.y = rand()%window.getSize().y;
                pos.y -= window.getSize().y/2;
                sf::CircleShape c(2,4);
                c.setPosition(moon.getPosition() + pos);
                c.setFillColor(sf::Color(0,0,0,250));
                ministars.push_back(c);
            }
        }

        for(int i = 0; i < maxStars; ++i){
            window.draw(ministars[i]);
        }



        if(stars.size() > 0){
            float angle = getAngle(moon.getPosition(), stars[0].getPosition());
            //std::cout << "the angle is "<< angle << std::endl;
            sf::Vector2f guiaPos;
            guia.setFillColor(sf::Color(0,0,0,255));
            guiaPos.x = moon.getPosition().x + std::cos(angle*M_PI/180) * (moon.getRadius()*3 + guia.getRadius());
            guiaPos.y = moon.getPosition().y + std::sin(angle*M_PI/180) * (moon.getRadius()*3 + guia.getRadius());
            guia.setPosition(guiaPos);
            window.draw(guia);

            guia.setFillColor(sf::Color(200,200,200,250));
            guiaPos.x = moon.getPosition().x + std::cos(angle*M_PI/180) * (moon.getRadius()*3 );//+ guia.getRadius());
            guiaPos.y = moon.getPosition().y + std::sin(angle*M_PI/180) * (moon.getRadius()*3 );//+ guia.getRadius());
            guia.setPosition(guiaPos);
            window.draw(guia);
        }

        for(Star &s : stars){
            /*
            float dist = std::abs(distance(moon.getPosition(), s.getPosition()));
            if(dist < 2*moon.getGlobalBounds().width){
                float factor = dist/2.f*moon.getGlobalBounds().width;
                s.setColor(sf::Color(255,255,255,int(255*factor)));
            }else s.setColor(sf::Color(255,255,255,255));
            */
            s.render(window);
            //window.draw(s);
        }

        for(Star& s : penguinsToFollow) {
            s.render(window);
        }

        moon.render(window);

        if(alpha >= 255){
            window.setView(window.getDefaultView());
            window.clear(c);
            window.draw(endS);
        }

        window.display();

	}
}
