#pragma once
#include <SFML/Graphics.hpp>
#include "quadtree/quadtree.hpp"

namespace MyGui {
    void RenderGui(sf::RenderWindow& window, std::vector<Object>& objects, const Quadtree& tree, sf::FloatRect worldBounds, float frameTime);
}