#ifndef DEBUG_TOOLS_HPP
#define DEBUG_TOOLS_HPP

#include <SFML/Graphics.hpp>
#include "quadtree/quadtree.hpp"
#include "utils/constants.hpp"

void drawQuadtree(sf::RenderWindow& window, const Quadtree& tree,
                   node_id id, sf::FloatRect bounds);

void wall_collision_toggle();

#endif
