#ifndef MODELS_HPP
#define MODELS_HPP

#include "utils/constants.hpp"
#include <random>
#include <cmath>

std::random_device rd;
std::mt19937 gen(rd());

//generates a distrabution of particles matching an Isothermal Sphere Galaxy
struct ISOSPHERE {
    //position vectors
    std::vector<float> x, y, z;
    //veloctiy vectors
    std::vector<float> X_velocity, Y_velocity;
    //assign particle masses
    float particle_mass = 1.f;
    //calculate rho_0
    //float rho_0 = (particle_mass * NMB_OBJECT) / (4 * static_cast<float>(M_PI) * SCALE);
    //v_circ
    float v_circ_squared = G * particle_mass * NMB_OBJECT / SCALE;
    //velocity dispersion
    const float disp = std::sqrt(v_circ_squared) /std::sqrt(3); //values from other project (see simple dark matter halos)
    

    size_t size() const {return x.size();}

    ISOSPHERE(int NMB_PARTICLE) {
        init_model(NMB_PARTICLE);
    };

    private:
        void populate_vectors(int NMB_PARTICLE, unsigned seed = std::random_device{}()) {
            x.resize(NMB_PARTICLE);
            y.resize(NMB_PARTICLE); 
            z.resize(NMB_PARTICLE);
            X_velocity.resize(NMB_PARTICLE); 
            Y_velocity.resize(NMB_PARTICLE);

            std::mt19937 rng(seed);
            std::uniform_real_distribution<float> distPhi(0.0f, 2.0f * static_cast<float>(M_PI));
            std::uniform_real_distribution<float> distCosTheta(-1.0f, 1.0f);
            std::uniform_real_distribution<float> distR(0.0f, 1.0f);
            std::normal_distribution<float> distVel(0.0f, disp);
            
            for (size_t i = 0; i < NMB_PARTICLE; ++i) {
            float r     = distR(rng);
            float phi   = distPhi(rng);
            float theta = std::acos(distCosTheta(rng));
            
            //convert to cartesien
            x[i] = r * std::sin(theta) * std::cos(phi);
            y[i] = r * std::sin(theta) * std::sin(phi);
            z[i] = r * std::cos(theta);

            X_velocity[i] = distVel(rng);
            Y_velocity[i] = distVel(rng);
        }

            
        };
        void init_model(int NMB_PARTICLE) {
            populate_vectors(NMB_PARTICLE);
        };

};




#endif
