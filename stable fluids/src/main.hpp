#pragma once

#include <iostream>
#include <SFML\Graphics.hpp>

#include "framerate.hpp"
#include "clickdetector.hpp"

#include "display.hpp"
#include "fluidalgorithms.hpp"

void mainLoop();

void input();
void launch();

const double FPS = 60.0f;

//size of square winow
const int windowWidth = 700;

//size of voxels in pixels, should divide windowWidth
const int voxelWidth = 2; //3 gives 60fps with 720

//size of the square scene
const int sceneWidth = windowWidth / voxelWidth;

//amount of voxels in scene
const int voxelCount = (sceneWidth + 2) * (sceneWidth + 2);

//the display and scene
Display display(windowWidth, voxelWidth, "Fluids");

const double deltaT = 0.1;
const double diffusion = 0.0000012;
const double viscosity = 0.0000;
const double source = 100.0;
const double force = 5.0;

static double u[voxelCount], v[voxelCount], u_prev[voxelCount], v_prev[voxelCount];
static double dens[voxelCount], dens_prev[voxelCount];

const double pi = asin(1) * 2;

int R(int i, int j) {
  return (i + (sceneWidth + 2) * j);
}