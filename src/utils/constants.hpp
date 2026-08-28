#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 900
//will change this to be different default values per model
//app should start with nothing generated and on button press load model with N particles; N will be a default value with option
//that value before generation
#define NMB_OBJECT 1000
#define SCALE 300.f
//non constant values
inline bool ENABLE_WALL_COLLISION = true;
inline bool ENABLE_PARTICLE_COLLISION = true;

inline float theta_BH = 0.8f;
inline float G = 300.f; 
inline float softening = 3.f;

#endif