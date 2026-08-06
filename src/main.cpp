#include <SFML/Graphics.hpp>


int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}),"SFML window", sf::Style::Titlebar | sf::Style::Close);
 
 
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

        // set the shape
        for (int i = 0; i < 5; i++) {

        }
        sf::CircleShape shape(50.f);
        shape.setFillColor(sf::Color(100, 250, 50));
        shape.setPosition(sf::Vector2f(400,300));
        window.draw(shape);
 
        // Update the window
        window.display();
    }
}
