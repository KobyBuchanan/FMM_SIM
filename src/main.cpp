#include <SFML/Graphics.hpp>
#include "object.hpp"
#include <vector>


int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}),"SFML window", sf::Style::Titlebar | sf::Style::Close);
    
    std::vector<Object> Objects;

    for (int i = 0; i < 5; i++) {
        sf::Vector2f pos = sf::Vector2f(400.f +(25*i), 300.f - (25*i));
        Objects.emplace_back(pos);
    }
 
    //Main loop
    sf::Clock clock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }   

        float dt = clock.restart().asSeconds();

        window.clear();

        for (auto& obj : Objects) {
            obj.update(dt);
            obj.draw(window);
        }

        window.display();
    }

    return 0;
}
