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

//random distrabution for now, will be replaced with DM Halo distrabution later
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> distX(0.f, WINDOW_WIDTH);
std::uniform_real_distribution<float> distY(0.f, WINDOW_HEIGHT);
std::uniform_real_distribution<float> Rvel(-25, 25);


int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}),"SFML window", sf::Style::Titlebar | sf::Style::Close);

    if (!ImGui::SFML::Init(window)) {
    std::cerr << "Failed to initialize ImGui-SFML!" << std::endl;
    return -1;
    }
    auto& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
        
    std::vector<Object> Objects;
    Objects.reserve(NMB_OBJECT);

    for (int i = 0; i < NMB_OBJECT; i++) {
        sf::Vector2f pos{distX(gen), distY(gen)};
        sf::Vector2f vel{Rvel(gen),Rvel(gen)};
        Objects.emplace_back(pos, vel);
    }

    sf::FloatRect worldBounds({0.f, 0.f}, {WINDOW_WIDTH, WINDOW_HEIGHT});

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
        }   

        if (!Running) 
            break;

        float dt = clock.restart().asSeconds();

        ImGui::SFML::Update(window, sf::seconds(dt)); 

        //physics
        for (auto& obj : Objects) {
            obj.update(dt);  
        }

        Quadtree tree = buildQuadtree(Objects, worldBounds);
        resolveCollisions(Objects, tree);

        for (auto& obj : Objects) {
            if (ENABLE_WALL_COLLISION) {
                resolveBoundary(obj, worldBounds);
            }
        }
        

        //draw
        window.clear();
        for (auto& obj : Objects) {
            obj.draw(window);
        }

        MyGui::RenderGui(window,tree,worldBounds);
        
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    window.close();
    return 0;
}
