#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define NMB_OBJECT 1000
#define SCALE 300.f
//non constant values
inline bool ENABLE_WALL_COLLISION = true;
inline sf::Vector2f gravity = {0.f,0.f};
inline float theta_BH = 0.7f;
inline float G = 1.f; 
inline float softening_squared = 9.f;

#endif