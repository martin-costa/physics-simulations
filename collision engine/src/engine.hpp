#pragma once

#include "collisionalgorithms.hpp"

class Engine {
private:
  int width;
  int height;

  int scale;

  std::vector<Particle> particles;

  sf::CircleShape circle;

  void updateMovement(int fps, int iterations);

  void particleCollisions(double dt);

public:

  bool gravity = false;

  Engine(int width, int height, int scale);

  Engine();

  //update the engine by one frame
  void update(int fps);

  //add a particle to the engine
  void addParticle(Particle p);

  //reset the engine scene
  void reset();

  //draw the scene of the engine
  void draw(sf::RenderWindow* window);
};