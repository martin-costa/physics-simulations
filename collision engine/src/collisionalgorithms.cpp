#include "collisionalgorithms.hpp"

#include <iostream>

//collision equation for a particle colliding with a plane
void planeCollisionEquation(Particle* p, double angle, double e) {
  double resultant = p->vel.magnitude();
  double phi = angle - atan2(p->vel.y, p->vel.x);

  double phi2 = atan(tan(phi) * e);
  double resultant2 = (resultant * cos(phi)) / (cos(phi2));

  p->vel.x = (resultant2 * cos(phi2 + angle));
  p->vel.y = (resultant2 * sin(phi2 + angle));
}

//collision equation for two particles colliding
void particleCollisionEquation(Particle* p, Particle* q, double e) {

  if (p->immovable == true) {
    particleRigidCollisionEquation(q, p, e);
    return;
  }
  if (q->immovable == true) {
    particleRigidCollisionEquation(p, q, e);
    return;
  }

  double v1 = p->vel.magnitude();
  double v2 = q->vel.magnitude();

  double m1 = p->mass;
  double m2 = q->mass;

  double phi = atan2(p->pos.y - q->pos.y, p->pos.x - q->pos.x);

  double theta1 = atan2(p->vel.y, p->vel.x);
  double theta2 = atan2(q->vel.y, q->vel.x);

  Vector2 v1r = Vector2(cos(theta1 - phi), sin(theta1 - phi)) * v1;
  Vector2 v2r = Vector2(cos(theta2 - phi), sin(theta2 - phi)) * v2;

  double u1xr = (v1r.x * (m1 - e * m2) + (1 + e) * m2 * v2r.x) / (m1 + m2);
  double u2xr = (v2r.x * (m2 - e * m1) + (1 + e) * m1 * v1r.x) / (m1 + m2);

  double u1x = u1xr * cos(phi) - v1r.y * sin(phi);
  double u1y = u1xr * sin(phi) + v1r.y * cos(phi);
  double u2x = u2xr * cos(phi) - v2r.y * sin(phi);
  double u2y = u2xr * sin(phi) + v2r.y * cos(phi);

  p->vel = Vector2(u1x, u1y);
  q->vel = Vector2(u2x, u2y);
}

//collision equation for particle p colliding into immovable particle q
void particleRigidCollisionEquation(Particle* p, Particle* q, double e) {
  double v1 = p->vel.magnitude();
  double v2 = q->vel.magnitude();

  double phi = atan2(p->pos.y - q->pos.y, p->pos.x - q->pos.x);

  double theta1 = atan2(p->vel.y, p->vel.x);
  double theta2 = atan2(q->vel.y, q->vel.x);

  Vector2 v1r = Vector2(cos(theta1 - phi), sin(theta1 - phi)) * v1;
  Vector2 v2r = Vector2(cos(theta2 - phi), sin(theta2 - phi)) * v2;

  double u1xr = (- v1r.x * e + (1 + e) * v2r.x);

  double u1x = u1xr * cos(phi) - v1r.y * sin(phi);
  double u1y = u1xr * sin(phi) + v1r.y * cos(phi);

  p->vel = Vector2(u1x, u1y);
}

//calculate the n-value of a collision
double calculateNValue(Particle* p, Particle* q, double dt) {
  Vector2 d = p->pos - q->pos;
  Vector2 e = (q->vel - p->vel) * dt;

  double a = pow(e.x, 2) + pow(e.y, 2);
  double b = 2 * (e.x * d.x + e.y * d.y);
  double c = pow(d.x, 2) + pow(d.y, 2) - pow(p->radius + q->radius, 2);

  double disc = (b * b) - (4 * a * c);

  if (a == 0 || disc < 0) return 0;
  return (-b + sqrt(disc)) / (2 * a);
}

//aux methods ==>

void particleTakeBack(Particle* p, double n, int direction) {
  p->pos += p->vel * direction * n;
}

double random(int lowerBound, int upperBound) {
  int number = upperBound;
  if (lowerBound != upperBound) {
    number = (std::rand() % (upperBound - lowerBound)) - upperBound;
  }
  return number;
}