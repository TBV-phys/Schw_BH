#pragma once
#include "physicsBH.h"

void RK4(State& s, double dt, State (*dsystem)(const State&, double, float), double b, float tol);
