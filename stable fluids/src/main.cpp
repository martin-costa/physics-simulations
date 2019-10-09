#include "main.hpp"

int main() {

  launch();

  sf::Event e;

  while (display.window.isOpen()) {
    while (display.window.pollEvent(e)) {
      if (sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) display.window.close();
    }

    mainLoop();

    framerate(FPS, true);

    display.window.display();
    display.window.clear(sf::Color(0, 0, 0));
  }

  return 0;
}

void mainLoop() {
  for (int i = 0; i < voxelCount; i++) {
    dens_prev[i] = 0;
    u_prev[i] = 0;
    v_prev[i] = 0;
  }

  //COOL SIMULATIONS_____________________________

  //BIG FLAME
  /*dens_prev[R(sceneWidth / 2, 5)] = 1000;
  for (int i = 0; i <= 240; i+= 10) {
    v_prev[R(sceneWidth / 2 - 8, i)] = 15;
    v_prev[R(sceneWidth / 2, i)] = 15;
    v_prev[R(sceneWidth / 2 + 8, i)] = 15;
  }*/

  //THE SUN
  /*for (double i = 0; i < 2*pi; i += 0.05) {
    dens_prev[R((int)(120*2*cos(i)) + 2 * 180, (int)(2 * 120*sin(i)) + 2 * 180)] = 500;

    u_prev[R((int)(2 * 120 * cos(i)) + 2 * 180, (int)(2 * 120 * sin(i)) + 2 * 180)] = 10*-sin(i);
    v_prev[R((int)(2 * 120 * cos(i)) + 2 * 180, (int)(2 * 120 * sin(i)) + 2 * 180)] = 10*cos(i);
  }*/

  //____________________________________________

  input();

  velStep(sceneWidth, u, v, u_prev, v_prev, viscosity, deltaT);
  densStep(sceneWidth, dens, dens_prev, u, v, diffusion, deltaT);

  display.draw(dens, u, v);
}

void input() {
  static sf::Vector2i mousePos = sf::Vector2i(0, 0);
  sf::Vector2i pos = sf::Mouse::getPosition(display.window);

  //add velocity
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    if (0 < pos.x / voxelWidth < sceneWidth && 0 < (-pos.y + windowWidth) / voxelWidth < sceneWidth) {
      int i = pos.x / voxelWidth;
      int j = (-pos.y + windowWidth) / voxelWidth;

      u[R(i, j)] = (pos.x - mousePos.x) * force;
      v[R(i, j)] = (-pos.y + mousePos.y) * force;
    }
  }
  mousePos = pos;

  //add source
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && 0 < mousePos.x / voxelWidth < sceneWidth
    && 0 < (-mousePos.y + windowWidth) / voxelWidth < sceneWidth) {

    dens[R(mousePos.x / voxelWidth, (-mousePos.y + windowWidth) / voxelWidth)] = source;
  }

  //grid
  static KeyDetector keyK(sf::Keyboard::K);
  if (keyK.typed()) display.toggleGrid();

  //lines
  static KeyDetector keyL(sf::Keyboard::L);
  if (keyL.typed()) display.toggleVelField();

  //reset
  static KeyDetector keyR(sf::Keyboard::R);
  if (keyR.typed()) {
    for (int i = 0; i < voxelCount; i++) {
      dens[i] = 0;
      u[i] = 0;
      v[i] = 0;
    }
  }
}

void launch() {
  //some text
  std::cout << "Simple Fluid Simulation\n" << std::endl;
  std::cout << "controls:" << std::endl;
  std::cout << "Left Click --> add force" << std::endl;
  std::cout << "Right Click --> add source" << std::endl;
  std::cout << "K --> Grid" << std::endl;
  std::cout << "R --> Reset" << std::endl;
  std::cout << "L --> Velocity Field\n" << std::endl;
}
