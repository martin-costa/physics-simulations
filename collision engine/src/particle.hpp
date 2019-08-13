#pragma once

#include "vector.hpp"

class Particle {

public:
  double radius;
  double mass;

  bool immovable = false;

  Vector2 pos;
  Vector2 vel;
  Vector2 acc;

  Particle(double radius);

  Particle();

  bool collided(Particle p);

  void advance(double dt, double scale);

  void reset();
};