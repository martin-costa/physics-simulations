#include "engine.hpp"

#include <iostream>

Engine::Engine(int width, int height, int scale) {
  this->width = width;
  this->height = height;

  this->scale = scale;

  reset();
}

Engine::Engine() {}

void Engine::updateMovement(int fps, int iterations) {

  //push particles into centre
  for (int j = 0; j < particles.size(); j++) {
    particles[j].vel *= 0.99;
    particles[j].acc = (Vector2(width / 2, height / 2) - particles[j].pos).normalize() * 9.81;
    if (gravity) particles[j].acc = Vector2(0, -1) * 9.81;
  }

  //for (int i = 0; i < particles.size(); i++) {
  //  for (int j = 0; j < particles.size(); j++) {
  //    double d = (particles[i].pos - particles[j].pos).magnitude();
  //    if (d == 0) d = 1;
  //    if (d < 100)
  //      particles[i].acc += (particles[i].pos - particles[j].pos).normalize() * 10000 / (d * d);
  //  }
  //}

  double dt = 1.0 / (double)(iterations * fps);

  for (int i = 0; i < iterations; i++) {
    for (int j = 0; j < particles.size(); j++)
      particles[j].advance(dt, scale);

    particleCollisions(dt);
  }

  //particleCollisions(1.0 / (double)fps);

  for (int i = 0; i < particles.size(); i++)
    particles[i].acc = Vector2();
}

void Engine::particleCollisions(double dt) {
  int particleCount = particles.size();
  int rigidCount = rigidParticles.size();

  double* nValues = new double[particleCount * (particleCount + rigidCount)]();

  //get the n-values for the collisions
  double n = 0.0;
  int index1 = -1, index2 = -1;

  for (int i = 0; i < particleCount; i++) {
    for (int j = i + 1; j < particleCount + rigidCount; j++) {
      double n2 = (j < particleCount) ? calculateNValue(&particles[i], &particles[j], dt) : calculateNValue(&particles[i], &rigidParticles[j - particleCount], dt);

      nValues[i * (particleCount + rigidCount) + j] = n2;

      if (n2 <= 1.0 && n2 >= n) {
        n = n2;
        index1 = i; index2 = j;
      }
    }
  }

  double n2;

  while (n > 0.0) {

    n2 = n;

    Particle* p1 = &particles[index1];
    Particle* p2 = (index2 < particleCount) ? &particles[index2] : &rigidParticles[index2 - particleCount];

    //handle the collision with the largest n-value
    particleTakeBack(p1, n * dt, -1);
    particleTakeBack(p2, n * dt, -1);

    particleCollisionEquation(p1, p2, 0.6);

    particleTakeBack(p1, n * dt, 1);
    particleTakeBack(p2, n * dt, 1);

    //recalculate the n-values corresponding to index1 and index2
    int indices[] = { index1, index2 };
    for (int i : indices) {
      if (i < particleCount) {
        for (int j = 0; j < i; j++)
          nValues[j * (particleCount + rigidCount) + i] = calculateNValue(&particles[j], &particles[i], dt);
        for (int j = i + 1; j < particleCount; j++)
          nValues[i * (particleCount + rigidCount) + j] = calculateNValue(&particles[i], &particles[j], dt);
        for (int j = particleCount; j < particleCount + rigidCount; j++)
          nValues[i * (particleCount + rigidCount) + j] = calculateNValue(&particles[i], &rigidParticles[j - particleCount], dt);
      }
    }

    n = 0;

    //get the new largest n-value
    for (int i = 0; i < particleCount; i++) {
      for (int j = i + 1; j < particleCount + rigidCount; j++) {
        if (nValues[i * (particleCount + rigidCount) + j] <= n2 && nValues[i * (particleCount + rigidCount) + j] >= n) {
          n = nValues[i * (particleCount + rigidCount) + j];
          index1 = i; index2 = j;
        }
      }
    }

  }

  delete[] nValues;
}

void Engine::update(int fps) {

  updateMovement(fps, 5);
}

void Engine::addParticle(Particle p) {
  particles.push_back(p);
}

void Engine::addRigidParticle(Particle p) {
  rigidParticles.push_back(p);
}

void Engine::reset() {

  this->particles = std::vector<Particle>(0);

  int wallParts = 20;
  double wallPartRad = 10000.0;

  this->rigidParticles = std::vector<Particle>(4 * wallParts);

  //create "walls" out of 4 large particles
  for (int i = 0; i < 4 * wallParts; i++) {
    rigidParticles[i] = Particle(wallPartRad);
    rigidParticles[i].immovable = true;
  }

  for (int i = 0; i < wallParts; i++) {
    rigidParticles[i * 4 + 0].pos = Vector2(-wallPartRad, height * ((double)i / (double)wallParts));
    rigidParticles[i * 4 + 1].pos = Vector2(((double)i / (double)wallParts) * width,- wallPartRad);
    rigidParticles[i * 4 + 2].pos = Vector2(width + wallPartRad, height * ((double)i / (double)wallParts));
    rigidParticles[i * 4 + 3].pos = Vector2(((double)i / (double)wallParts) * width, height + wallPartRad);
  }
}

//draw all the particles
void Engine::draw(sf::RenderWindow* window) {

  drawParticleVector(window, &particles, sf::Color(0, 183, 183));

  drawParticleVector(window, &rigidParticles, sf::Color(155, 155, 155));
}

//draw an array of particles with the same color
void Engine::drawParticleVector(sf::RenderWindow* window, std::vector<Particle>* particles, sf::Color color) {
  circle.setFillColor(color);

  for (int i = 0; i < particles->size(); i++) {
    circle.setPosition((*particles)[i].pos.x, height - (*particles)[i].pos.y);
    circle.setRadius((*particles)[i].radius);
    circle.setOrigin((*particles)[i].radius, (*particles)[i].radius);
    window->draw(circle);
  }
}