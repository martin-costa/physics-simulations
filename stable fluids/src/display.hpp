#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

class Display {
private:
  int windowWidth;
  int voxelWidth; //should divide windowWidth

  int sceneWidth;

  sf::VertexArray lines;
  sf::VertexArray squares;

  sf::RectangleShape line;

  bool showGrid = false;
  bool showVel = false;

public:
  sf::RenderWindow window;

  //constructor for the display
  Display(int windowWidth, int voxelWidth, std::string name);

  //draw the density field
  void draw(double* dens, double* u, double* v);

  //maps density field to pretty colors
  sf::Color colourMapper(double x);

  //method to help with indexing
  int I(int i, int j) {
    return (i + (sceneWidth + 2) * j);
  }

  //methods to toggle grid and vel field
  void toggleGrid() { showGrid = !showGrid; }

  void toggleVelField() { showVel = !showVel; }
};