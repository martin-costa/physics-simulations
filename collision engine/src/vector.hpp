#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

class Vector2 {

public:
  double x;
  double y;

  //constructors
  Vector2(double x, double y);

  Vector2();

  //standard vector field ops.
  Vector2 operator+(Vector2 v);

  void operator+=(Vector2 v);

  Vector2 operator-();

  Vector2 operator-(Vector2 v);

  void operator-=(Vector2 v);

  Vector2 operator*(double a);

  void operator *= (double a);

  Vector2 operator/(double a);

  void operator/=(double a);

  //R^2 vector ops.
  Vector2 normalize();

  double magnitude();

  double dot(Vector2 v);

  double angle();

  Vector2 rotate(double theta);

  //sfml ops.
  sf::Vector2f toSf();
};