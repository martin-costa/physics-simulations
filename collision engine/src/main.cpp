#include "main.hpp"

int main() {

  launch();

  while (window.isOpen()) {

    sf::Event e;
    while (window.pollEvent(e)) {
      if (sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
    }

    inputs();

    engine.update(FPS);

    engine.draw(&window);

    window.display();

    window.clear(sf::Color(0, 0, 0));

    framerate(FPS, false);
  }
}

void inputs() {

  static KeyDetector keyR(sf::Keyboard::R);
  if (keyR.typed()) {
    engine.reset();
  }

  static KeyDetector keyP(sf::Keyboard::P);
  if (keyP.typed()) {
    //Particle p1(25);

    //p1.pos = Vector2(25, HEIGHT / 2);
    //p1.vel = Vector2(10000, 0);

    //engine.addParticle(p1);

    Particle p1(25);
    Particle p2(25);

    p1.pos = Vector2(50, HEIGHT / 2);
    p1.vel = Vector2(10000, 0);

    p2.pos = Vector2(WIDTH - 50, HEIGHT / 2);
    p2.vel = Vector2(-10000, 0);

    engine.addParticle(p1);
    engine.addParticle(p2);
  }

  static KeyDetector keyG(sf::Keyboard::G);
  static MouseDetector middleMouse(sf::Mouse::Middle);
  if (keyG.typed() || middleMouse.clicked()) {
    engine.gravity = !engine.gravity;
  }

  static MouseDetector leftMouse(sf::Mouse::Left, &window);
  if (leftMouse.down()) {
    Particle p(15);
    p.immovable = true;

    sf::Vector2i mousePos = leftMouse.pos();

    p.pos = Vector2(mousePos.x, HEIGHT - mousePos.y);
    engine.addRigidParticle(p);
  }

  static MouseDetector rightMouse(sf::Mouse::Right, &window);
  if (rightMouse.clicked()) {
    Particle p(random(25, 25));

    sf::Vector2i mousePos = leftMouse.pos();

    p.pos = Vector2(mousePos.x, HEIGHT - mousePos.y);
    engine.addParticle(p);
  }

}

void launch() {
  std::cout << "Collision Engine\n\n";
}