#pragma once
#include <SFML/Graphics.hpp>
#include "quadtree/quadtree.hpp"

namespace MyGui {
    void RenderGui(sf::RenderWindow& window, const Quadtree& tree, sf::FloatRect worldBounds);
}