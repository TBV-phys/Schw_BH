#pragma once
#include "../physics/physicsBH.h"

void drawGrid(int xLines, int yLines, const std::array<float,4> proyectionRange);

void drawCircle(float cx, float cy, float r, int segments, const std::array<float,4>& colorRGBA);

void drawHorizon(float cx, float cy, float r, int points, int segments, const std::array<float,3>& colorRGB);

void drawTrail(Photon& p, bool polar = false);
