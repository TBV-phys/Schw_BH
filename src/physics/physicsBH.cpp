#include "physicsBH.h"

double dv(double R, double b)
{
    double inv_R = 1.0 / R;
    double fR = 1.0 - inv_R;

    return inv_R * inv_R * fR + b * b * inv_R * inv_R * inv_R * inv_R * fR * fR * (R -2.5);
}

State DSystem(const State& s, double b, float tol)
{
    return {
        dv(s.R, b),
        s.v,
        -b * (1.0 - 1.0 / s.R) / (s.R * s.R)
    };
}
