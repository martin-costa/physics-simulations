#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

#include "engine.hpp"
#include "framerate.hpp"
#include "inputdetector.hpp"

#define WIDTH 1600
#define HEIGHT 900

#define FPS 60

void launch();

int main();

void inputs();

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "collision engine", sf::Style::Titlebar);

Engine engine(WIDTH, HEIGHT, HEIGHT/3);