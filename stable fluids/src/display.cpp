#include "display.hpp"

//constructor for the display
Display::Display(int windowWidth, int voxelWidth, std::string name) {
  this->windowWidth = windowWidth;
  this->voxelWidth = voxelWidth;
  this->sceneWidth = windowWidth / voxelWidth;

  window.create(sf::VideoMode(windowWidth, windowWidth), name, sf::Style::Close);

  //set up array of squares and set their positions
  squares = sf::VertexArray(sf::Quads, 4 * (sceneWidth + 1) * (sceneWidth + 1));

  for (int i = 0; i <= sceneWidth; i++) {
    for (int j = 0; j <= sceneWidth; j++) {
      squares[4 * (sceneWidth + 1) * i + 4 * j].position = sf::Vector2f((i - 0.5) * voxelWidth, -(j - 0.5) * voxelWidth + windowWidth);
      squares[4 * (sceneWidth + 1) * i + 4 * j + 1].position = sf::Vector2f((i - 0.5) * voxelWidth, -(j + 0.5) * voxelWidth + windowWidth);
      squares[4 * (sceneWidth + 1) * i + 4 * j + 2].position = sf::Vector2f((i + 0.5) * voxelWidth, -(j + 0.5) * voxelWidth + windowWidth);
      squares[4 * (sceneWidth + 1) * i + 4 * j + 3].position = sf::Vector2f((i + 0.5) * voxelWidth, -(j - 0.5) * voxelWidth + windowWidth);
    }
  }

  //set up lines for the grid of voxels
  lines = sf::VertexArray(sf::Lines, 4 * windowWidth);

  sf::Color color = sf::Color(80, 80, 80);
  for (int i = 0; i < sceneWidth; i++) {
    lines[4 * i].position = sf::Vector2f(0, i * voxelWidth);
    lines[4 * i + 1].position = sf::Vector2f(windowWidth, i * voxelWidth);
    lines[4 * i].color = color;
    lines[4 * i + 1].color = color;
    lines[4 * i + 2].position = sf::Vector2f(i * voxelWidth, 0);
    lines[4 * i + 3].position = sf::Vector2f(i * voxelWidth, windowWidth);
    lines[4 * i + 2].color = color;
    lines[4 * i + 3].color = color;
  }

  //set up the lines for the velocity field
  line.setSize(sf::Vector2f(6, 1));
  line.setOrigin(3, 1);
  //line.setFillColor(sf::Color(175, 175, 175));
}

//draw the density field
void Display::draw(double* dens, double* u, double* v) {
  for (int i = 0; i <= sceneWidth; i++) {
    for (int j = 0; j <= sceneWidth; j++) {
      squares[4 * (sceneWidth + 1) * i + 4 * j].color = colourMapper(dens[I(i, j)]);
      squares[4 * (sceneWidth + 1) * i + 4 * j + 1].color = colourMapper(dens[I(i, j + 1)]);
      squares[4 * (sceneWidth + 1) * i + 4 * j + 2].color = colourMapper(dens[I(i + 1, j + 1)]);
      squares[4 * (sceneWidth + 1) * i + 4 * j + 3].color = colourMapper(dens[I(i + 1, j)]);
    }
  }
  window.draw(squares);

  //draw grid and vel field
  if (showGrid) window.draw(lines);
  if (showVel) {
    for (int i = 3; i <= windowWidth; i += 8) {
      for (int j = 3; j <= windowWidth; j += 8) {
        line.setPosition(sf::Vector2f(i, -j + windowWidth));

        sf::Vector2f v(u[I((int)(i / voxelWidth), (int)(j / voxelWidth))], v[I((int)(i / voxelWidth), (int)(j / voxelWidth))]);

        line.setRotation(atan2(v.y, v.x) * (90 / asin(1)));
        double size = 255 / (1 + pow(2.71, 2 - 50 * sqrt(v.x * v.x + v.y * v.y)));
        line.setFillColor(sf::Color(size, size, size));
        window.draw(line);
      }
    }
  }
}

//maps density field to pretty colors
sf::Color Display::colourMapper(double x) {
  if (x < -3) return sf::Color(210, 255, 255); //white
  if (x < -2) return sf::Color(210 * (-2 - x), 255, 255); //turq
  if (x < -1) return sf::Color(0, 255 * (-1 - x), 255); //blue
  if (x < 0) return sf::Color(0, 0, -x * 255); //black
  if (x < 1) return sf::Color(255 * (x), 0, 0); //red
  if (x < 2) return sf::Color(255, (x - 1) * 165, 0); //orange
  if (x < 3) return sf::Color(255, (x - 2) * 90 + 165, 0); //yellow
  return sf::Color(255, 255, 0);
}