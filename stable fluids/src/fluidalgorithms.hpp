#pragma once

#include <iostream>
#include <ctime>
#include <cstdlib>

int R(int i, int j);

void setBnd(int N, int b, double* x);

void addSource(int N, double* x, double* s, double dt);

void diffuse(int N, int b, double* x, double* x0, double diff, double dt);

void advect(int N, int b, double* d, double* d0, double* u, double* v, double dt);

void densStep(int N, double* x, double* x0, double* u, double* v, double diff, double dt);

void project(int N, double* u, double* v, double* p, double* div);

void velStep(int N, double* u, double* v, double* u0, double* v0, double visc, double dt);