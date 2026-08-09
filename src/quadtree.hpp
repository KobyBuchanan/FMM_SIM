#ifndef QUADTREE_HPP
#define QUADTREE_HPP
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>
#include "object.hpp"


using node_id = std::uint32_t;
static constexpr node_id null_node = node_id(-1);

static constexpr std::size_t MAX_OBJECTS_PER_LEAF = 4;
static constexpr std::size_t MAX_QUADTREE_DEPTH = 12;

struct QuadtreeNode {
    node_id children[2][2] = {{null_node, null_node}, {null_node, null_node}};
};

struct Quadtree {
    sf::FloatRect bounds;
    node_id root = null_node;
    std::vector<QuadtreeNode> nodes;

    std::vector<std::uint32_t> indices;

    std::vector<std::uint32_t> nodeObjectsBegin;

    bool isLeaf(node_id id) const {
        return nodes[id].children[0][0] == null_node;
    }
};

// bounds is fixed; any particle outside it is skipped and not inserted into the tree
Quadtree buildQuadtree(const std::vector<Object>& objects, const sf::FloatRect& bounds);


#endif