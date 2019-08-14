#pragma once

#include <cstdlib>

#include "particle.hpp"

//collision equation for a particle colliding with a plane
void planeCollisionEquation(Particle* p, double angle, double e);

//collision equation for two particles colliding
void particleCollisionEquation(Particle* p, Particle* q, double e);

//collision equation for particle p colliding into immovable particle q
void particleRigidCollisionEquation(Particle* p, Particle* q, double e);

//calculate the n-value of a collision
double calculateNValue(Particle* p, Particle* q, double dt);

//aux methods ==>

void particleTakeBack(Particle* p, double n, int direction);

int random(int lowerBound, int upperBound);