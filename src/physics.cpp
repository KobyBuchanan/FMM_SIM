#include "physics.hpp"

void resolveBoundary(Object& obj, const sf::FloatRect& bounds) {
    float minX = bounds.position.x + obj.radius;
    float maxX = bounds.position.x + bounds.size.x - obj.radius;
    float minY = bounds.position.y + obj.radius;
    float maxY = bounds.position.y + bounds.size.y - obj.radius;

    if (obj.position.x < minX) {
        obj.position.x = minX;
        obj.velocity.x = -obj.velocity.x;
    } else if (obj.position.x > maxX) {
        obj.position.x = maxX;
        obj.velocity.x = -obj.velocity.x;
    }

    if (obj.position.y < minY) {
        obj.position.y = minY;
        obj.velocity.y = -obj.velocity.y;
    } else if (obj.position.y > maxY) {
        obj.position.y = maxY;
        obj.velocity.y = -obj.velocity.y;
    }
}