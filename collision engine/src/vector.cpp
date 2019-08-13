#include "vector.hpp"

//constructors
Vector2::Vector2(double x, double y) {
  this->x = x;
  this->y = y;
}

Vector2::Vector2() : Vector2(0, 0) {}

//standard vector field ops.
Vector2 Vector2::operator + (Vector2 v) {
  return Vector2(x + v.x, y + v.y);
}

void Vector2::operator += (Vector2 v) {
  x += v.x;
  y += v.y;
}

Vector2 Vector2::operator-() {
  return Vector2(-x, -y);
}

Vector2 Vector2::operator-(Vector2 v) {
  return Vector2(x - v.x, y - v.y);
}

void Vector2::operator-=(Vector2 v) {
  x -= v.x;
  y -= v.y;
}

Vector2 Vector2::operator*(double a) {
  return Vector2(x * a, y * a);
}

void Vector2::operator*=(double a) {
  x *= a;
  y *= a;
}

Vector2 Vector2::operator/(double a) {
  return Vector2(x / a, y / a);
}

void Vector2::operator/=(double a) {
  x /= a;
  y /= a;
}

//R^2 vector ops.
Vector2 Vector2::normalize() {
  if (magnitude() > 0) return (*this) / magnitude();
  return *this;
}

double Vector2::magnitude() {
  return sqrt(x * x + y * y);
}

double Vector2::dot(Vector2 v) {
  return x * v.x + y * v.y;
}

double Vector2::angle() {
  return atan2(y, x);
}

Vector2 Vector2::rotate(double theta) {
  return Vector2(x * cos(theta) - y * sin(theta), x * sin(theta) + y * cos(theta));
}

//sfml ops.
sf::Vector2f Vector2::toSf() {
  return sf::Vector2f(x, y);
}