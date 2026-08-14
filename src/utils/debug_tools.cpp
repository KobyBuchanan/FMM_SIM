#include "debug_tools.hpp"


void drawQuadtree(sf::RenderWindow& window, const Quadtree& tree,
                   node_id id, sf::FloatRect bounds)
{
    if (id == null_node) return;

    sf::RectangleShape rect({bounds.size.x, bounds.size.y});
    rect.setPosition(bounds.position);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color(0,0,255, 120)); // translucent blue
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

void wall_collision_toggle() {
    ENABLE_WALL_COLLISION = !ENABLE_WALL_COLLISION;
};

void clearObjects(std::vector<Object>& objects) {
    objects.clear();
};