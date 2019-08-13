#include "particle.hpp"

Particle::Particle(double radius) {
  this->radius = radius;
  this->mass = radius * radius;
}

Particle::Particle() : Particle(1) {}

bool Particle::collided(Particle p) {
  return (pos - p.pos).magnitude() < radius + p.radius;
}

void Particle::advance(double dt, double scale) {
  vel += acc * dt * scale;
  pos += vel * dt;
}

void Particle::reset() {
  pos = Vector2(radius, radius);
  vel = Vector2();
  acc = Vector2();
}