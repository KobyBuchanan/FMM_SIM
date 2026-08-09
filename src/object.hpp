#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <SFML/Graphics.hpp>

struct Object {
    //Intial values
    sf::Vector2f position = sf::Vector2f{0.0f,0.0f};
    sf::Color color = sf::Color::Red;
    float radius = 15.f;

    sf::CircleShape circle;
    
    //constructor
    Object() {
        initShape();
    };
    explicit
    Object(sf::Vector2f m_position) 
        : position{m_position}
        {
            initShape();
        }

    void update(double dt) {

    };

    void draw(sf::RenderWindow& window) {
        circle.setPosition(position);
        window.draw(circle);
    };

private:
    void initShape() {
        circle.setRadius(radius);
        circle.setFillColor(color);
        circle.setOrigin(sf::Vector2f(radius,radius));
        circle.setPosition(position);
    };

};


#endif