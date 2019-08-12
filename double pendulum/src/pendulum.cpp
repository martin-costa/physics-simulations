#include "pendulum.hpp"

DoublePendulum::DoublePendulum() : 
  DoublePendulum(sf::Vector2f(0, 0), 1) {}

DoublePendulum::DoublePendulum(sf::Vector2f pos, float scale) {
  this->pendulumPos = pos;
  this->scale = scale;

  L1 = 200;
  L2 = 200;

  m1 = 50;
  m2 = 50;

  reset();

  ball1.setOrigin(r1, r1);
  ball2.setOrigin(r2, r2);

  ball1.setFillColor(sf::Color::Blue);
  ball2.setFillColor(sf::Color::Red);
}

void DoublePendulum::update(int fps) {
  omega1 *= damping;
  omega2 *= damping;

  int iterations = 100;
  for (int i = 0; i < iterations; i++) {
    getAngularAcceleration(1.0 / iterations, fps);

    omega1 += omega1prime * (1.0 / iterations);
    omega2 += omega2prime * (1.0 / iterations);

    theta1 += omega1 * (1.0 / iterations);
    theta2 += omega2 * (1.0 / iterations);
  }

  ball1Pos = pendulumPos + sf::Vector2f(L1 * cos(theta1 - PI / 2), L1 * sin(theta1 - PI / 2));
  ball2Pos = ball1Pos + sf::Vector2f(L2 * cos(theta2 - PI / 2), L2 * sin(theta2 - PI / 2));
}

void DoublePendulum::getAngularAcceleration(float dt, float fps) {
  float g = 9.81 * scale * dt / fps;

  float den = 2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2);

  omega1prime = -g * (2 * m1 + m2) * sin(theta1) - m2 * g * sin(theta1 - 2 * theta2) - 2 * sin(theta1 - theta2) * m2 * (omega2 * omega2 * L2 + omega1 * omega1 * L1 * cos(theta1 - theta2));
  omega1prime /= (L1 * den);

  omega2prime = 2 * sin(theta1 - theta2) * (omega1 * omega1 * L1 * (m1 + m2) + g * (m1 + m2) * cos(theta1) + omega2 * omega2 * L2 * m2 * cos(theta1 - theta2));
  omega2prime /= (L2 * den);
}

void DoublePendulum::reset() {
  theta1 = PI;
  theta2 = PI;

  omega1 = 0.01;
  omega2 = 0;
}

void DoublePendulum::draw(sf::RenderWindow* window) {
  int HEIGHT = window->getSize().y;

  ball1.setPosition(ball1Pos.x, HEIGHT - ball1Pos.y);
  ball2.setPosition(ball2Pos.x, HEIGHT - ball2Pos.y);

  rods[0].position = sf::Vector2f(pendulumPos.x, HEIGHT - pendulumPos.y);
  rods[1].position = ball1.getPosition();

  rods[2].position = ball1.getPosition();
  rods[3].position = ball2.getPosition();

  window->draw(rods);

  window->draw(ball1);
  window->draw(ball2);
}