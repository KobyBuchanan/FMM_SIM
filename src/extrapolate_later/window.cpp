#include "window.hpp"

void Window::run() {
    //create window
    m_window.create(sf::VideoMode({800, 600}),"SFML window", sf::Style::Titlebar | sf::Style::Close);
    m_window.setFramerateLimit(60);

    //Main loop
    while (m_window.isOpen())
    {
        // Process events
        while (const std::optional event = m_window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                m_window.close();
        }
 
        // Clear screen
        m_window.clear();

        // set the shape 
        sf::CircleShape shape(50.f);
        shape.setFillColor(sf::Color(100, 250, 50));
        shape.setPosition(sf::Vector2f(400,300));
        m_window.draw(shape);
 
        // Update the window
        m_window.display();
};