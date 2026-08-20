#include <SFML/Graphics.hpp>
#include "hud/hud.hpp"
#include "utils/constants.hpp"
#include "quadtree/quadtree.hpp"
#include "physics/physics.hpp"
#include "hud/hud.hpp"
#include <vector>
#include <random>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include "generation_models/generator.hpp"

int main()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    
    int centerX = (desktop.size.x - WINDOW_WIDTH) / 2;
    int centerY = (desktop.size.y - WINDOW_HEIGHT) / 2;

    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),"SFML window", sf::Style::Titlebar | sf::Style::Close);
    sf::View cameraView(sf::FloatRect({0.f,0.f},{WINDOW_WIDTH,WINDOW_HEIGHT}));
    window.setPosition(sf::Vector2i(centerX, centerY));

    if (!ImGui::SFML::Init(window)) {
    std::cerr << "Failed to initialize ImGui-SFML!" << std::endl;
    return -1;
    }
    auto& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
        
    sf::FloatRect worldBounds({0.f, 0.f}, {WINDOW_WIDTH, WINDOW_HEIGHT});
    auto Objects = generate<ISOSPHERE>(worldBounds);

    sf::Clock clock;
    bool Running = true;
    //Main loop
    while (Running)
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event); 
            if (event->is<sf::Event::Closed>())
                Running = false;

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                //close window
                //if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                //window.close();
                //zoom in 
                if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
                cameraView.zoom(0.9f);
                //zoom out
                if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
                cameraView.zoom(1.1f);
                //reset zoom
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
                cameraView.setSize({WINDOW_WIDTH,WINDOW_HEIGHT});

            }   
        }   

        if (!Running) 
            break;

        float dt = clock.restart().asSeconds();

        ImGui::SFML::Update(window, sf::seconds(dt)); 


        //physics
        Quadtree tree = buildQuadtree(Objects, worldBounds);
        computeMassDistabution(tree, Objects);
        computeForces(Objects,tree);
        //std::cout << "accel[0] = " << Objects[0].acceleration.x << ", " << Objects[0].acceleration.y << "\n";
        resolveCollisions(Objects, tree);

        //test cicle
        sf::CircleShape circle(300.f);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(sf::Color::White);
        circle.setOutlineThickness(2.f);
        circle.setOrigin({300.f, 300.f}); // Center the origin if needed
        circle.setPosition({400.f, 400.f});

        for (auto& obj : Objects) {
            obj.update(dt);  
            if (ENABLE_WALL_COLLISION) {
                resolveBoundary(obj, worldBounds);
            }
        }
        

        //draw
        window.setView(cameraView);
        window.clear();
        for (auto& obj : Objects) {
            obj.draw(window);
        }

        window.draw(circle);

        MyGui::RenderGui(window,Objects,tree,worldBounds);
        
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    window.close();
    return 0;
}
