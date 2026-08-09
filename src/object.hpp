#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <SFML/Graphics.hpp>

struct Object {
    //Intial values
    sf::Vector2f position = {0.0f,0.0f};
    sf::Color color = sf::Color::Red;
    float radius = 5.f;
    //Intial physics
    sf::Vector2f velocity {0.0f,0.0f};
    sf::Vector2f acceleration {0.0f,0.0f};

    sf::CircleShape circle;

    //constructor
    Object() {
        initShape();
    };
    explicit
    Object(sf::Vector2f m_position, sf::Vector2f m_velocity) 
        : position{m_position}
        , velocity{m_velocity}
        {
            initShape();
        }

    void update(float dt) {
        velocity += acceleration * dt;
        position += velocity * dt;
       
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