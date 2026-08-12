#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <SFML/Graphics.hpp>
#include "object/object.hpp"
#include "quadtree/quadtree.hpp"

void resolveBoundary(Object& obj, const sf::FloatRect& bounds);

void resolveCollisions(std::vector<Object>& objects, const Quadtree& tree);


#endif