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

    const int frame_rate = 60;
    window.setFramerateLimit(frame_rate);

    const double dt = 1.f / 60.f;
    const float MAX_FRAME_TIME = 0.25f;
    sf::Clock clock;
    float accumulator = 0.f;

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

        float frameTime = clock.restart().asSeconds();
        accumulator += std::min(frameTime, MAX_FRAME_TIME);

        Quadtree tree;

        while (accumulator >= dt) {
            //physics
            tree = buildQuadtree(Objects, worldBounds);
            computeMassDistabution(tree, Objects);
            computeForces(Objects,tree);
            
            if(ENABLE_PARTICLE_COLLISION) {
                resolveCollisions(Objects, tree);
            }

            for (auto& obj : Objects) {
                obj.update(dt);  
                if (ENABLE_WALL_COLLISION) {
                    resolveBoundary(obj, worldBounds);
                }
            }

            accumulator -= dt;
        }

         
        ImGui::SFML::Update(window, sf::seconds(frameTime));
        //draw
        window.setView(cameraView);
        window.clear();
        for (auto& obj : Objects) {
            obj.draw(window);
        }

        MyGui::RenderGui(window,Objects,tree,worldBounds);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    window.close();
    return 0;
}
