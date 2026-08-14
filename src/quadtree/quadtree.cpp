#include "quadtree.hpp"
#include <algorithm>

namespace {

using Iter = std::vector<std::uint32_t>::iterator;

node_id build_impl(Quadtree& tree, const std::vector<Object>& objects,
                    const sf::FloatRect& bounds,
                    Iter begin, Iter end, std::size_t depth)
{
    node_id result = static_cast<node_id>(tree.nodes.size());
    tree.nodes.emplace_back();
    tree.nodeObjectsBegin.push_back(
        static_cast<std::uint32_t>(begin - tree.indices.begin()));

    std::size_t count = static_cast<std::size_t>(end - begin);

    
    if (count <= MAX_OBJECTS_PER_LEAF || depth >= MAX_QUADTREE_DEPTH) {
        return result;
    }

    float midX = bounds.position.x + bounds.size.x / 2.f;
    float midY = bounds.position.y + bounds.size.y / 2.f;
    float halfW = bounds.size.x / 2.f;
    float halfH = bounds.size.y / 2.f;

    auto belowMid = [&](std::uint32_t i) { return objects[i].position.y < midY; };
    auto leftMid  = [&](std::uint32_t i) { return objects[i].position.x < midX; };

    Iter splitY       = std::partition(begin, end, belowMid);
    Iter splitXLower  = std::partition(begin, splitY, leftMid);
    Iter splitXUpper  = std::partition(splitY, end, leftMid);

    tree.nodes[result].children[0][0] = build_impl(tree, objects,
        {bounds.position, {halfW, halfH}}, begin, splitXLower, depth + 1);

    tree.nodes[result].children[0][1] = build_impl(tree, objects,
        {{midX, bounds.position.y}, {halfW, halfH}}, splitXLower, splitY, depth + 1);

    tree.nodes[result].children[1][0] = build_impl(tree, objects,
        {{bounds.position.x, midY}, {halfW, halfH}}, splitY, splitXUpper, depth + 1);

    tree.nodes[result].children[1][1] = build_impl(tree, objects,
        {{midX, midY}, {halfW, halfH}}, splitXUpper, end, depth + 1);

    return result;
}

}

Quadtree buildQuadtree(const std::vector<Object>& objects, const sf::FloatRect& bounds)
{
    Quadtree tree;
    tree.bounds = bounds;

    tree.indices.reserve(objects.size());
    for (std::uint32_t i = 0; i < objects.size(); ++i) {
        if (bounds.contains(objects[i].position)) {
            tree.indices.push_back(i);
        }
    }

    tree.root = build_impl(tree, objects, bounds,
                            tree.indices.begin(), tree.indices.end(), 0);

    tree.nodeObjectsBegin.push_back(static_cast<std::uint32_t>(tree.indices.size()));

    return tree;
}

float computeMassRecurse(Quadtree& tree, std::vector<Object>& objects, node_id id) {
    if(tree.isLeaf(id)) {
        float total_mass = 0.f;
        sf::Vector2f weighted_position{0.f,0.f};

        for(int i = tree.nodeObjectsBegin[id]; i < tree.nodeObjectsBegin[id + 1]; ++i) {
            const Object obj = objects[tree.indices[id]];
            total_mass += obj.mass;
            weighted_position += obj.mass * obj.position;
        }
   
        tree.nodeMass[id] = total_mass;
        tree.NodeCenterOfMass[id] = (total_mass > 0.f) ? weighted_position / total_mass : sf::Vector2f{0.f, 0.f};
        return total_mass;
     }

     float total_mass = 0.f;
     sf::Vector2f weighted_position{0.f,0.f};

    for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < 2; ++x) {
            node_id child = tree.nodes[id].children[y][x];
            if (child == null_node) continue;
                
            float childMass = computeMassRecurse(tree, objects, child);
            total_mass += childMass;
            weighted_position += childMass * tree.NodeCenterOfMass[child];
        }
    }

    tree.nodeMass[id] = total_mass;
    tree.NodeCenterOfMass[id] = (total_mass > 0.f) ? weighted_position / total_mass : sf::Vector2f{0.f, 0.f};
    return total_mass;
}

void computeMassDistabution(Quadtree& tree, std::vector<Object>& objects) {
    tree.nodeMass.assign(tree.nodes.size(),0.f);
    tree.NodeCenterOfMass.assign(tree.nodes.size(),sf::Vector2f{0.f,0.f});

    if(tree.root != null_node) {
        computeMassRecurse(tree,objects,tree.root);
    }

    
}