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
    float particle_mass = 1.f / NMB_OBJECT;
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
            x[i] = r * std::sin(theta) * std::cos(phi) * SCALE;
            y[i] = r * std::sin(theta) * std::sin(phi) * SCALE;
            z[i] = r * std::cos(theta);

            float radius_px = std::sqrt(x[i] * x[i] + y[i] * y[i]);
            float tangent_x = -y[i];
            float tangent_y = x[i];

            if (radius_px > 1e-5f) {
                tangent_x /= radius_px;
                tangent_y /= radius_px;
            }
            else {
                tangent_x = tangent_y = 0.f;
            }

            float v_circ = std::sqrt(v_circ_squared);

            X_velocity[i] = tangent_x * v_circ + distVel(rng);
            Y_velocity[i] = tangent_y * v_circ + distVel(rng);
        }

            
        };
        void init_model(int NMB_PARTICLE) {
            populate_vectors(NMB_PARTICLE);
        };

};


struct SOLAR {
    //position vectors
    std::vector<float> x_sun, y_sun;
    //veloctiy vectors
    std::vector<float> X_velocity, Y_velocity;
    //assign particle masses
    float solarMass = 1000.f;
    float earthMass = 10.f;
    

};



#endif
