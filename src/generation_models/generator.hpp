#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <vector>
#include "object/object.hpp"
#include "models.hpp"
#include "utils/constants.hpp"
#include <SFML/Graphics.hpp>

template <typename Model>
std::vector<Object> generate(const sf::FloatRect& bounds) {
    std::vector<Object> objects;
    objects.reserve(NMB_OBJECT);

    Model model(NMB_OBJECT);

    //center coortinates
    float halfW = bounds.size.x / 2.f;
    float halfH = bounds.size.y / 2.f;

    for (size_t i = 0; i < model.size(); ++i) {
        Object obj;
        obj.mass = model.particle_mass;
        obj.position = { halfW + model.x[i] * SCALE, halfH - model.y[i] * SCALE };
        obj.velocity = { model.X_velocity[i] , -model.Y_velocity[i] };
        objects.push_back(obj);
    }
    return objects;
};







#endif