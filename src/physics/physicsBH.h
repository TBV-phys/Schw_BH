#pragma once
#include <vector>
#include <array>
#include <cmath>

// Photon definitions

struct Trail {
    int ptsTrailMax;
    unsigned long size;
    int start;
    std::vector<double> R;
    std::vector<double> theta;
};

struct State {
    double v, R, theta;
};

enum class PhotonState {
    Active,     // Active integration with RK4
    Escaped,    // R > Rmax
    Captured,   // R < 1
    Faded       // Innactive and visualy faded
};

struct Photon {
    double b;
    State pos;
    Trail trail;
    float fadeFactor = 1.0f;
    
    PhotonState state = PhotonState::Active;

    // Constructors
    Photon(double bValue, double xin) : b(bValue)
    {
        // Posición inicial
        pos.R = std::sqrt(xin*xin + b*b);
        pos.v = - (1 - 1/pos.R) * std::sqrt(1 - b*b*(1 - 1/pos.R)/(pos.R*pos.R));
        pos.theta = std::atan(b/xin) + M_PI;

        // Inicializar trail
        trail.ptsTrailMax = 1000;
        trail.size = 1;
        trail.start = 0;
        trail.R.reserve(trail.ptsTrailMax);
        trail.theta.reserve(trail.ptsTrailMax);
        trail.R.push_back(pos.R);
        trail.theta.push_back(pos.theta);
    }
    
    Photon(double R0, double theta0, double alpha0) : pos{0.0, R0, theta0} {
        
        b = R0 * std::sin(pos.theta - alpha0) / std::sqrt(1- 1/R0);
        int sign = (std::cos(alpha0 - theta0) >= 0) ? 1 : -1;
        pos.v = sign * (1 - 1/pos.R) * std::sqrt(1 - b*b*(1 - 1/pos.R)/(pos.R*pos.R));
        trail.ptsTrailMax = 1000;
        trail.size = 1;
        trail.start = 0;
        trail.R.reserve(trail.ptsTrailMax);
        trail.theta.reserve(trail.ptsTrailMax);
        trail.R.push_back(pos.R);
        trail.theta.push_back(pos.theta);
    }
};

// System dynamics

double dv(double R, double b);

State DSystem(const State& s, double b, float tol);
