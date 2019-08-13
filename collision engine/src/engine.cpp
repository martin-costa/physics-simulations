#include "engine.hpp"

#include <iostream>

Engine::Engine(int width, int height, int scale) {
  this->width = width;
  this->height = height;

  this->scale = scale;

  reset();

  circle = sf::CircleShape(25);
  circle.setFillColor(sf::Color::Black);
  circle.setOrigin(25, 25);
}

Engine::Engine() {}

void Engine::updateMovement(int fps, int iterations) {

  //push particles into centre
  for (int j = 0; j < particles.size(); j++) {
    if (!particles[j].immovable) {
      particles[j].vel *= 0.99;
      particles[j].acc = (Vector2(width / 2, height / 2) - particles[j].pos).normalize() * 9.81;
      if (gravity) particles[j].acc = Vector2(0, -1) * 9.81;
    }
  }

  double dt = 1.0 / (double)(iterations * fps);

  for (int i = 0; i < iterations; i++) {
    for (int j = 0; j < particles.size(); j++) {
      particles[j].advance(dt, scale);
    }
  }

  particleCollisions(dt * iterations);
}

void Engine::particleCollisions(double dt) {
  int particleCount = particles.size();
  double* nValues = new double[particleCount * particleCount]{-1};

  //get the n-values for the collisions
  double n = 0;
  int index1 = -1, index2 = -1;

  for (int i = 0; i < particleCount; i++) {
    for (int j = 0; j < i; j++) {
      nValues[i * particleCount + j] = calculateNValue(&particles[i], &particles[j], dt);

      if (nValues[i * particleCount + j] <= 1 && nValues[i * particleCount + j] > n) {
        n = nValues[i * particleCount + j];
        index1 = i; index2 = j;
      }
    }
  }

  double n2;

  while (n != 0) {

    n2 = n;

    //std::cout << n << "\n";

    //handle the collision with the largest n-value
    particleTakeBack(&particles[index1], n * dt, -1);
    particleTakeBack(&particles[index2], n * dt, -1);

    particleCollisionEquation(&particles[index1], &particles[index2], 0.2);

    particleTakeBack(&particles[index1], n * dt, 1);
    particleTakeBack(&particles[index2], n * dt, 1);

    //recalculate the n-values corresponding to index1 and index2
    int indices[] = { index1, index2 };
    for (int i = 0; i < particleCount; i++) {

      for (int index : indices) {
        if (index > i)
          nValues[index * particleCount + i] = calculateNValue(&particles[index], &particles[i], dt);
        else if (index < i)
          nValues[i * particleCount + index] = calculateNValue(&particles[i], &particles[index], dt);
      }
    }

    n = 0;

    //get the new largest n-value
    for (int i = 0; i < particleCount; i++) {
      for (int j = 0; j < i; j++) {
        if (nValues[i * particleCount + j] <= 1 && nValues[i * particleCount + j] >= n) {
          n = nValues[i * particleCount + j];
          index1 = i; index2 = j;
        }
      }
    }

  }

  //for (int i = 4; i < particleCount; i++) {
  //  for (int j = 0; j < i; j++) {
  //    if (particles[i].collided(particles[j]))
  //      std::cout << "FATAL ERROR!";
  //  }
  //}

  delete[] nValues;
}

void Engine::update(int fps) {

  updateMovement(fps, 25);
}

void Engine::addParticle(Particle p) {
  particles.push_back(p);
}

void Engine::reset() {

  this->particles = std::vector<Particle>(4);

  int wallRad = 1000000;

  //create "walls"
  particles[0] = Particle(wallRad);
  particles[0].pos = Vector2(-wallRad, height / 2);
  particles[0].immovable = true;

  particles[1] = Particle(wallRad);
  particles[1].pos = Vector2(width / 2, -wallRad);
  particles[1].immovable = true;

  particles[2] = Particle(wallRad);
  particles[2].pos = Vector2(wallRad + width, height / 2);
  particles[2].immovable = true;

  particles[3] = Particle(wallRad);
  particles[3].pos = Vector2(width / 2, wallRad + height);
  particles[3].immovable = true;
}

void Engine::draw(sf::RenderWindow* window) {

  for (int i = 0; i < particles.size(); i++) {
    if (particles[i].immovable) circle.setFillColor(sf::Color(170, 123, 36));
    else circle.setFillColor(sf::Color(30, 133, 133));

    circle.setPosition(particles[i].pos.x, height - particles[i].pos.y);
    circle.setRadius(particles[i].radius);
    circle.setOrigin(particles[i].radius, particles[i].radius);
    window->draw(circle);
  }
}