#include "moon.h"
#include "textBox.h"

#define MOVEMENTSPEED 10
double mypow(double a, double b) {
    double c = 1;
    for (int i=0; i<b; i++)
        c *= a;
    return c;
}

double myfact(double x) {
    double ret = 1;
    for (int i=1; i<=x; i++)
        ret *= i;
    return ret;
}

double mysin(double x) {
    double y = x;
    double s = -1;
    for (int i=3; i<=100; i+=2) {
        y+=s*(mypow(x,i)/myfact(i));
        s *= -1;
    }
    return y;
}
double mycos(double x) {
    double y = 1;
    double s = -1;
    for (int i=2; i<=100; i+=2) {
        y+=s*(mypow(x,i)/myfact(i));
        s *= -1;
    }
    return y;
}
double mytan(double x) {
     return (mysin(x)/mycos(x));
}
Moon::Moon(std::string s, sf::Font& f)
    :textBox(s, f),
      speed(0,0)
{
    texture.loadFromFile(("res/glow.png"));
    tmoon.loadFromFile(("res/moon.png"));
    glowing.setTexture(texture);
    smoon.setTexture(tmoon);

    m_timer = 0;
    setRadius(50);
    setPointCount(200);
    setFillColor(sf::Color::White);

    glowing.setScale(getGlobalBounds().width*2/glowing.getGlobalBounds().width,
                     getGlobalBounds().height*2/glowing.getGlobalBounds().height );

    glowing.setPosition(getPosition());
    glowing.setOrigin(glowing.getLocalBounds().width/2, glowing.getLocalBounds().height/2);

    smoon.setScale(getGlobalBounds().width/smoon.getGlobalBounds().width,
                     getGlobalBounds().height/smoon.getGlobalBounds().height );

    smoon.setPosition(getPosition());
    smoon.setOrigin(smoon.getLocalBounds().width/2, smoon.getLocalBounds().height/2);

    //smoon.setColor(sf::Color(250,250,250,50));


}

void Moon::setSentence(std::string s)
{
    textBox.setSentence(s);
}

void Moon::update(float dt, sf::RenderTarget& window) {
    m_timer += dt;

    sf::Vector2f touchPos = getPosition();
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    if (sf::Touch::isDown(0)) {
               touchPos = sf::Vector2f(sf::Touch::getPosition(0).x,sf::Touch::getPosition(0).y);

               if(touchPos.x < window.getSize().x/3){
                   left = true;
               }
               if(touchPos.x > 2*window.getSize().x/3){
                   right = true;
               }
               if(touchPos.y < window.getSize().y/3){
                   up = true;
               }
               if(touchPos.y > 2*window.getSize().y/3){
                   down = true;
               }
/*               if(touchPos.x > getPosition().x+getGlobalBounds().width/2){
                   if(touchPos.y < getPosition().y+getGlobalBounds().height/2 && touchPos.y > getPosition().y-getGlobalBounds().height/2){
                        right = true;
                   }
               }

               if(touchPos.x < getPosition().x-getGlobalBounds().width/2){
                   if(touchPos.y < getPosition().y+getGlobalBounds().height/2 && touchPos.y > getPosition().y-getGlobalBounds().height/2){
                        left = true;
                   }
               }
  */

    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)  || sf::Keyboard::isKeyPressed(sf::Keyboard::A) || left) 	{
        speed.x += -MOVEMENTSPEED*dt;
//        move(sf::Vector2f(-MOVEMENTSPEED*dt, 0));
  //      glowing.setOrigin(glowing.getLocalBounds().width/2-MOVEMENTSPEED*dt, glowing.getLocalBounds().height/2);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) || right)	{
        speed.x += MOVEMENTSPEED*dt;
        //move(sf::Vector2f( MOVEMENTSPEED*dt, 0));
        //glowing.setOrigin(glowing.getLocalBounds().width/2+MOVEMENTSPEED*dt, glowing.getLocalBounds().height/2);
    }
    else {
        if(speed.x > 0) speed.x = std::max(0.f, speed.x - MOVEMENTSPEED*dt);
        if(speed.x < 0) speed.x = std::min(0.f, speed.x + MOVEMENTSPEED*dt);
    }


    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)    || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || up)	{
       speed.y += -MOVEMENTSPEED*dt;
        //move(sf::Vector2f(0, -MOVEMENTSPEED*dt));
        //glowing.setOrigin(glowing.getLocalBounds().width/2, glowing.getLocalBounds().height/2-MOVEMENTSPEED*dt);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)  || sf::Keyboard::isKeyPressed(sf::Keyboard::S) || down)	{
        speed.y += MOVEMENTSPEED*dt;
        //move(sf::Vector2f(0,  MOVEMENTSPEED*dt));
        //glowing.setOrigin(glowing.getLocalBounds().width/2, glowing.getLocalBounds().height/2+MOVEMENTSPEED*dt);
    }
    else {
        if(speed.y > 0) speed.y = std::max(0.f, speed.y - MOVEMENTSPEED*dt);
        if(speed.y < 0) speed.y = std::min(0.f, speed.y + MOVEMENTSPEED*dt);
    }

    const int MAX_SPEED = 5;
    if(speed.x > MAX_SPEED) speed.x = MAX_SPEED;
    if(speed.y > MAX_SPEED) speed.y = MAX_SPEED;
    if(speed.x < -MAX_SPEED) speed.x = -MAX_SPEED;
    if(speed.y < -MAX_SPEED) speed.y = -MAX_SPEED;
    move(speed);
    glowing.setOrigin(glowing.getLocalBounds().width/2 + speed.x, glowing.getLocalBounds().height/2+ speed.y);

    glowing.setPosition(getPosition());
    smoon.setPosition(getPosition());
    textBox.update(dt);

    glowing.setScale(getLocalBounds().width*2/glowing.getLocalBounds().width,
                     getLocalBounds().height*2/glowing.getLocalBounds().height );
    float scale = glowing.getScale().x + std::abs((std::sin(m_timer/3)))/10;
    glowing.setScale(scale,scale);

    static float angle = 0;
//    float speedFactor = (std::abs(speed.x)+std::abs(speed.y)/2);
    float speedFactor2 = std::max(std::abs(speed.x),std::abs(speed.y));
    smoon.setRotation(8*(speedFactor2/MAX_SPEED) *std::sin(angle) );
    angle += dt*speedFactor2;

    if(angle >= 2*3.1415) angle = 0;
}

void Moon::render(sf::RenderTarget &target)
{
    target.draw(glowing);
    //target.draw(*this);
    target.draw(smoon);
    textBox.setPosition(sf::Vector2f(getPosition().x - textBox.getGlobalBounds().width/2, getPosition().y -120));
    textBox.setFillColor(sf::Color::Black);
    target.draw(textBox);
}

