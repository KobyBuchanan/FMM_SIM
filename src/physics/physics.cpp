#include "physics.hpp"
#include <algorithm>
#include <cmath>

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


bool isIntersectingTreeBounds(const sf::Vector2f& center, float radius, const sf::FloatRect& bounds) {
    float closestX = std::clamp(center.x, bounds.position.x, bounds.position.x + bounds.size.x);
    float closestY = std::clamp(center.y, bounds.position.y, bounds.position.y + bounds.size.y);

    float dx = center.x - closestX;
    float dy = center.y - closestY;

    return (dx*dx + dy*dy) <= (radius * radius);
}

void resolvePair(Object& obj1, Object& obj2) {
    sf::Vector2f delta = obj2.position - obj1.position;
    float squared_distance = (delta.x * delta.x) + (delta.y * delta.y);
    float min_distance = obj1.radius + obj2.radius;

    if (squared_distance >= min_distance * min_distance || squared_distance == 0.f) return;

    float distance = std::sqrt(squared_distance);
    sf::Vector2f unitNormalVector = delta / distance;
    
    //resolve overlap
    float overlap = min_distance - distance;
    obj1.position -= unitNormalVector * (overlap / 2.f);
    obj2.position += unitNormalVector * (overlap / 2.f);

    //resolve velocity elastically kind of
    sf::Vector2f relative_velocity = obj2.velocity - obj1.velocity;
    float velocityAlongNormal = unitNormalVector.x * relative_velocity.x + unitNormalVector.y * relative_velocity.y;

    if (velocityAlongNormal > 0.f) return;

    //need to come back here when particles have mass
    sf::Vector2f impulse = unitNormalVector * velocityAlongNormal;
    obj1.velocity += impulse;
    obj2.velocity -= impulse;
}

void queryNode(std::vector<Object>& objects, const Quadtree& tree, node_id id, sf::FloatRect bounds, std::uint32_t objectIndex, const sf::Vector2f& pos, float search_radius) {
    if (id == null_node) return;
    if (!isIntersectingTreeBounds(pos,search_radius,bounds)) return;

    if (tree.isLeaf(id)) {
        for (auto i = tree.nodeObjectsBegin[id]; i < tree.nodeObjectsBegin[id + 1]; ++i) {
            std::uint32_t otherIndex = tree.indices[i];
            if (otherIndex <= objectIndex) continue;
            resolvePair(objects[objectIndex], objects[otherIndex]);
        }
        return;
    }

    float midX = bounds.position.x + bounds.size.x / 2.f;
    float midY = bounds.position.y + bounds.size.y / 2.f;
    float halfW = bounds.size.x / 2.f;
    float halfH = bounds.size.y / 2.f;

    queryNode(objects, tree, tree.nodes[id].children[0][0],{bounds.position, {halfW, halfH}}, objectIndex, pos, search_radius);

    queryNode(objects, tree, tree.nodes[id].children[0][1],{{midX, bounds.position.y}, {halfW, halfH}}, objectIndex, pos, search_radius);

    queryNode(objects, tree, tree.nodes[id].children[1][0],{{bounds.position.x, midY}, {halfW, halfH}}, objectIndex, pos, search_radius);

    queryNode(objects, tree, tree.nodes[id].children[1][1],{{midX, midY}, {halfW, halfH}}, objectIndex, pos, search_radius);

}


void resolveCollisions(std::vector<Object>& objects, const Quadtree& tree) {

    for (std::uint32_t i = 0; i < tree.indices.size(); ++i) {
        std::uint32_t objectIndex = tree.indices[i];
        const Object& obj = objects[objectIndex];
        float search_radius = 2.f * obj.radius;
    
        queryNode(objects,tree,tree.root,tree.bounds,objectIndex,obj.position,search_radius);
    }
}

void queryForce(std::vector<Object>& objects, const Quadtree& tree, node_id id, sf::FloatRect bounds, std::uint32_t objectIndex) {
    Object& obj = objects[objectIndex];
    if(id == null_node) return;

    if(tree.isLeaf(id)) return;
    //construct child bounds
    float midX = bounds.position.x + bounds.size.x / 2.f;
    float midY = bounds.position.y + bounds.size.y / 2.f;
    float halfW = bounds.size.x / 2.f;
    float halfH = bounds.size.y / 2.f;

    //near field check
    float theta = 1.f; //needs to be aquared
    sf::Vector2f delta = tree.NodeCenterOfMass[id] - obj.position;
    float squared_distance = (delta.x * delta.x) + (delta.y * delta.y);

    if((bounds.size.x * bounds.size.x) / squared_distance < theta *theta) {
        
    }


    

}

void computeForces(std::vector<Object>& objects, const Quadtree& tree) {

    for(std::uint32_t i = 0; i < tree.indices.size(); ++i) {
        std::uint32_t objectIndex = tree.indices[i];
        Object& obj = objects[objectIndex];
        obj.acceleration = {0.f,0.f};

        queryForce(objects, tree, tree.root, tree.bounds, objectIndex);
    }
}