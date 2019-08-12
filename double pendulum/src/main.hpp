#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

#include "pendulum.hpp"
#include "framerate.hpp"

#define WIDTH 900
#define HEIGHT 900

#define SCALE 900

#define PI 3.14159265359

#define FPS 60

void launch();

int main();

void inputs();

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "double pendulum chaos", sf::Style::Titlebar);

int count = 10;
std::vector<DoublePendulum> pendules(count);

//std::vector<sf::VertexArray> traces(count);