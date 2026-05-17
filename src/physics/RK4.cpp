#include "RK4.h"
#include "physicsBH.h"

void RK4(State& s, double dt, State (*dsystem)(const State&, double, float), double b, float tol)
{
    double h = dt * (1.1 - 1/s.R);
    
    State k1 = dsystem(s, b, tol);
    State temp;

    temp.R = s.R + 0.5 * h * k1.R;
    temp.v = s.v + 0.5 * h * k1.v;
    temp.theta = s.theta + 0.5 * h * k1.theta;

    State k2 = dsystem(temp, b, tol);

    temp.R = s.R + 0.5 * h * k2.R;
    temp.v = s.v + 0.5 * h * k2.v;
    temp.theta = s.theta + 0.5 * h * k2.theta;

    State k3 = dsystem(temp, b, tol);

    temp.R = s.R + h * k3.R;
    temp.v = s.v + h * k3.v;
    temp.theta = s.theta + h * k3.theta;

    State k4 = dsystem(temp, b, tol);

    // Actualizamos s
    s.R     += h * (k1.R + 2*k2.R + 2*k3.R + k4.R) / 6;
    s.v     += h * (k1.v + 2*k2.v + 2*k3.v + k4.v) / 6.0;
    s.theta += h * (k1.theta + 2*k2.theta + 2*k3.theta + k4.theta) / 6.0;
}
