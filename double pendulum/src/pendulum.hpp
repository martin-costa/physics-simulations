#pragma once

#include <SFML/Graphics.hpp>

#define PI 3.14159265359

class DoublePendulum {
public:

  //pendulum properties
  float L1;
  float L2;

  float r1 = 15;
  float r2 = 15;

  float m1;
  float m2;

  float damping = 0.999*0 + 1;

  float theta1 = 0;
  float theta2 = 0;

  float omega1 = 0.01;
  float omega2 = 0;

  float omega1prime = 0;
  float omega2prime = 0;

  float scale;

  //sfml objects
  sf::Vector2f pendulumPos;

  sf::Vector2f ball1Pos;
  sf::Vector2f ball2Pos;

  sf::CircleShape ball1 = sf::CircleShape(r1);
  sf::CircleShape ball2 = sf::CircleShape(r2);

  sf::VertexArray rods = sf::VertexArray(sf::LinesStrip, 4);

  //constructors
  DoublePendulum(sf::Vector2f pos, float scale);

  DoublePendulum();

  //update the pendulum
  void update(int fps);

  void getAngularAcceleration(float dt, float fps);

  void reset();

  void draw(sf::RenderWindow* window);
};