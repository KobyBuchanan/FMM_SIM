#include <SFML/Graphics.hpp>
#include "object.hpp"
#include "constants.hpp"
#include "quadtree.hpp"
#include "physics.hpp"
#include <vector>
#include <random>

//random distrabution for now, will be replaced with DM Halo distrabution later
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> distX(0.f, WINDOW_WIDTH);
std::uniform_real_distribution<float> distY(0.f, WINDOW_HEIGHT);
std::uniform_real_distribution<float> Rvel(-25, 25);

void drawQuadtree(sf::RenderWindow& window, const Quadtree& tree,
                   node_id id, sf::FloatRect bounds)
{
    if (id == null_node) return;

    sf::RectangleShape rect({bounds.size.x, bounds.size.y});
    rect.setPosition(bounds.position);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color(0, 255, 0, 120)); // translucent green
    rect.setOutlineThickness(1.f);
    window.draw(rect);

    if (tree.isLeaf(id)) return;

    float midX = bounds.position.x + bounds.size.x / 2.f;
    float midY = bounds.position.y + bounds.size.y / 2.f;
    float halfW = bounds.size.x / 2.f;
    float halfH = bounds.size.y / 2.f;

    drawQuadtree(window, tree, tree.nodes[id].children[0][0],
                 {bounds.position, {halfW, halfH}});
    drawQuadtree(window, tree, tree.nodes[id].children[0][1],
                 {{midX, bounds.position.y}, {halfW, halfH}});
    drawQuadtree(window, tree, tree.nodes[id].children[1][0],
                 {{bounds.position.x, midY}, {halfW, halfH}});
    drawQuadtree(window, tree, tree.nodes[id].children[1][1],
                 {{midX, midY}, {halfW, halfH}});
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}),"SFML window", sf::Style::Titlebar | sf::Style::Close);
    
    std::vector<Object> Objects;
    Objects.reserve(NMB_OBJECT);

    for (int i = 0; i < NMB_OBJECT; i++) {
        sf::Vector2f pos{distX(gen), distY(gen)};
        sf::Vector2f vel{Rvel(gen),Rvel(gen)};
        Objects.emplace_back(pos, vel);
    }

    sf::FloatRect worldBounds({0.f, 0.f}, {WINDOW_WIDTH, WINDOW_HEIGHT});

    sf::Clock clock;
    //Main loop
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }   

        float dt = clock.restart().asSeconds();
        
        //physics
        for (auto& obj : Objects) {
            obj.update(dt);
            if (ENABLE_WALL_COLLISION) {
            resolveBoundary(obj, worldBounds);
        }
        }
        
        //rebuild quadtree
        Quadtree tree = buildQuadtree(Objects, worldBounds);
        //draw
        window.clear();
        for (auto& obj : Objects) {
            obj.draw(window);
        }

        //quadtree overlay
        //drawQuadtree(window,tree,tree.root,worldBounds);

        window.display();
    }

    return 0;
}
