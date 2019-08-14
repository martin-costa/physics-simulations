#pragma once

#include "collisionalgorithms.hpp"

class Engine {
private:

  int width;
  int height;

  double scale;

  std::vector<Particle> particles;
  std::vector<Particle> rigidParticles;

  sf::CircleShape circle;

  void updateMovement(int fps, int iterations);

  void particleCollisions(double dt);

  void drawParticleVector(sf::RenderWindow* window, std::vector<Particle>* particles, sf::Color color);

public:

  bool gravity = false;

  Engine(int width, int height, int scale);

  Engine();

  //update the engine by one frame
  void update(int fps);

  //add a particle to the engine
  void addParticle(Particle p);

  //add an immovable particle to the engine
  void addRigidParticle(Particle p);

  //reset the engine scene
  void reset();

  //draw the scene of the engine
  void draw(sf::RenderWindow* window);
};