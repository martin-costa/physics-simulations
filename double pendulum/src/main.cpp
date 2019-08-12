#include "main.hpp"

int main() {

  launch();

  for (int i = 0; i < count; i++) {
    pendules[i] = DoublePendulum(sf::Vector2f(WIDTH / 2, HEIGHT / 2), HEIGHT / 3);
    pendules[i].theta1 += 0.01 * i;
  }

  //inputs();

  //for (int i = 0; i < count; i++) {
  //  traces[i] = sf::VertexArray(sf::Lines, 0);
  //  traces[i].append(sf::Vertex(sf::Vector2f(pendules[i].ball2Pos.x, HEIGHT - pendules[i].ball2Pos.y), sf::Color(0, 0, 0)));
  //}

  while (window.isOpen()) {

    //for (int i = 0; i < count; i++) {
    //  traces[i].append(sf::Vertex(sf::Vector2f(pendules[i].ball2Pos.x, HEIGHT - pendules[i].ball2Pos.y), sf::Color(50, 50, 50)));
    //  traces[i].append(sf::Vertex(sf::Vector2f(pendules[i].ball2Pos.x, HEIGHT - pendules[i].ball2Pos.y), sf::Color(50, 50, 50)));
    //}

    sf::Event e;
    while (window.pollEvent(e)) {
      if (sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
    }

    //for (int i = 0; i < count; i++)
    //  window.draw(traces[i]);

    inputs();

    window.display();

    window.clear(sf::Color::Black);

    framerate(FPS, true);
  }
}

void inputs() {

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    for (int i = 0; i < count; i++) {
      pendules[i].reset();
      pendules[i].theta1 += 0.01 * i;
    }
  
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
    for (int i = 0; i < count; i++)
      pendules[i].L2 += 2;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    for (int i = 0; i < count; i++)
      if (pendules[i].L2 > 2) pendules[i].L2 -= 2;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    for (int i = 0; i < count; i++)
      if (pendules[i].damping < 1) pendules[i].damping += 0.0001;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
    for (int i = 0; i < count; i++)
      if (pendules[i].damping > 0.98) pendules[i].damping -= 0.0001;

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    mousePos.y = HEIGHT - mousePos.y;

    for (int i = 0; i < count; i++) {
      float theta = atan2(mousePos.y - pendules[i].pendulumPos.y, mousePos.x - pendules[i].pendulumPos.x) + PI / 2;
      pendules[i].theta1 = theta;
      pendules[i].theta2 = theta;

      pendules[i].omega1 = 0;
      pendules[i].omega2 = 0;
    }
  }

  for (int i = 0; i < count; i++) {
    pendules[i].update(FPS);
    pendules[i].draw(&window);
  }
}

void launch() {
  std::cout << "Double Pendulum\n\n";
  std::cout << "Controls:\n";
  std::cout << "F --> Decrease Damping\n";
  std::cout << "D --> Increase Damping\n";
  std::cout << "O --> Increase L2\n";
  std::cout << "P --> Decrease L2\n";
  std::cout << "R --> Reset\n";
  std::cout << "Left Click --> Move Pendulum\n\n";
}