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

    // stop: few enough particles for a leaf, or hit max depth (guards against
    // near-duplicate positions causing endless subdivision)
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

    tree.nodeObjectsBegin.push_back(static_cast<std::uint32_t>(tree.indices.size())); // sentinel

    return tree;
}