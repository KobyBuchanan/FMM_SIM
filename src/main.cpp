#include <SFML/Graphics.hpp>
#include <vector>

struct Object {
    sf::Vector2f position;
    sf::Color color;
    float radius = 15.f;

    Object() = default;
    Object(sf::Vector2f m_position, sf::Color m_color)
        : position{m_position}
        , color{m_color} 
        {}

    };

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}),"SFML window", sf::Style::Titlebar | sf::Style::Close);
    
    std::vector<Object> Objects;

    for (int i = 0; i < 5; i++) {
        sf::Vector2f pos = sf::Vector2f(400.f +(25*i), 300.f - (25*i));
        sf::Color color = sf::Color::Red;
        Object o{pos,color};
        Objects.emplace_back(o);
    }

    sf::CircleShape templateCircle;
 
    //Main loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }
 
        // Clear screen
        window.clear();

        for (const auto& obj : Objects) {
            templateCircle.setRadius(obj.radius);
            templateCircle.setFillColor(obj.color);
            templateCircle.setPosition(obj.position);
        
        window.draw(templateCircle);
    }
        // Update the window
        window.display();
    }
    return 0;
}
