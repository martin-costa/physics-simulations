#include "fluidalgorithms.hpp"

void setBnd(int N, int b, double* x) {
  for (int i = 1; i <= N; i++) {
    x[R(0, i)] = b == 1 ? -x[R(1, i)] : x[R(1, i)];
    x[R(N + 1, i)] = b == 1 ? -x[R(N, i)] : x[R(N, i)];
    x[R(i, 0)] = b == 2 ? -x[R(i, 1)] : x[R(i, 1)];
    x[R(i, N + 1)] = b == 2 ? -x[R(i, N)] : x[R(i, N)];
  }

  x[R(0, 0)] = 0.5 * (x[R(1, 0)] + x[R(0, 1)]);
  x[R(0, N + 1)] = 0.5 * (x[R(1, N + 1)] + x[R(0, N)]);
  x[R(N + 1, 0)] = 0.5 * (x[R(N, 0)] + x[R(N + 1, 1)]);
  x[R(N + 1, N + 1)] = 0.5 * (x[R(N, N + 1)] + x[R(N + 1, N)]);
}

void addSource(int N, double* x, double* s, double dt) {
  int width = (N + 2) * (N + 2);
  for (int i = 0; i < width; i++) x[i] += dt * s[i];
}

void diffuse(int N, int b, double* x, double* x0, double diff, double dt) {
  double a = dt * diff * N * N;

  for (int k = 0; k < 1; k++) { //20
    for (int i = 1; i <= N; i++) {
      for (int j = 1; j <= N; j++) {
        x[R(i, j)] = (x0[R(i, j)] + a * (x[R(i - 1, j)] + x[R(i + 1, j)] + x[R(i, j - 1)] + x[R(i, j + 1)])) / (1 + 4 * a);
      }
    }
    setBnd(N, b, x);
  }
}

void advect(int N, int b, double* d, double* d0, double* u, double* v, double dt) {
  int i0, j0, i1, j1;
  double x, y, s0, t0, s1, t1, dt0;

  dt0 = dt * N;
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++) {
      x = i - dt0 * u[R(i, j)];
      y = j - dt0 * v[R(i, j)];

      if (x < 0.5) x = 0.5;
      if (x > N + 0.5) x = N + 0.5;
      i0 = (int)x;
      i1 = i0 + 1;

      if (y < 0.5) y = 0.5;
      if (y > N + 0.5) y = N + 0.5;
      j0 = (int)y;
      j1 = j0 + 1;

      s1 = x - i0; s0 = 1 - s1; t1 = y - j0; t0 = 1 - t1;

      d[R(i, j)] = s0 * (t0 * d0[R(i0, j0)] + t1 * d0[R(i0, j1)]) + s1 * (t0 * d0[R(i1, j0)] + t1 * d0[R(i1, j1)]);
    }
  }
  setBnd(N, b, d);
}

void densStep(int N, double* x, double* x0, double* u, double* v, double diff, double dt) {
  addSource(N, x, x0, dt);
  std::swap(x0, x); diffuse(N, 0, x, x0, diff, dt);
  std::swap(x0, x); advect(N, 0, x, x0, u, v, dt);
}

void project(int N, double* u, double* v, double* p, double* div) {
  double h = 1.0f / N;

  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++) {
      div[R(i, j)] = -0.5 * h * (u[R(i + 1, j)] - u[R(i - 1, j)] + v[R(i, j + 1)] - v[R(i, j - 1)]);
      p[R(i, j)] = 0;
    }
  }
  setBnd(N, 0, div); setBnd(N, 0, p);

  for (int k = 0; k < 20; k++) { //20
    for (int i = 1; i <= N; i++) {
      for (int j = 1; j <= N; j++) {
        p[R(i, j)] = (div[R(i, j)] + p[R(i - 1, j)] + p[R(i + 1, j)] + p[R(i, j - 1)] + p[R(i, j + 1)]) / 4;
      }
    }
    setBnd(N, 0, p);
  }

  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++) {
      u[R(i, j)] -= 0.5 * (p[R(i + 1, j)] - p[R(i - 1, j)]) / h;
      v[R(i, j)] -= 0.5 * (p[R(i, j + 1)] - p[R(i, j - 1)]) / h;
    }
  }
  setBnd(N, 1, u); setBnd(N, 2, v);
}

void velStep(int N, double* u, double* v, double* u0, double* v0, double visc, double dt) {
  addSource(N, u, u0, dt);
  addSource(N, v, v0, dt);

  std::swap(u0, u);
  std::swap(v0, v);

  diffuse(N, 2, v, v0, visc, dt);
  diffuse(N, 1, u, u0, visc, dt);

  project(N, u, v, u0, v0);

  std::swap(u0, u);
  std::swap(v0, v);

  advect(N, 1, u, u0, u0, v0, dt);
  advect(N, 2, v, v0, u0, v0, dt);

  project(N, u, v, u0, v0);
}