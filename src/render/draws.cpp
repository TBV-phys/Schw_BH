#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include "draws.h"

void drawGrid(int xLines, int yLines, const std::array<float,4> proyectionRange){
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glBegin(GL_LINES);
    
    glColor4f(1, 1, 1, 0.5);
    // x-axis
    glVertex2f(proyectionRange[0], 0);
    glVertex2f(proyectionRange[1],0);
    // y-axis
    glVertex2f(0,proyectionRange[2]);
    glVertex2f(0,proyectionRange[3]);
    // grid
    glColor4f(1, 1, 1, 0.2);
    int dx = (proyectionRange[1]-proyectionRange[0])/xLines;
    int dy = (proyectionRange[3]-proyectionRange[2])/yLines;
    
    for(int x = proyectionRange[0] + dx; x < proyectionRange[1]; x += dx){
        glVertex2f(x, proyectionRange[2]);
        glVertex2f(x, proyectionRange[3]);
    }
    for(int y = proyectionRange[2] + dy; y < proyectionRange[3]; y += dy){
        glVertex2f(proyectionRange[0],y);
        glVertex2f(proyectionRange[1],y);
    }
    glEnd();
}

void drawCircle(float cx, float cy, float r, int segments, const std::array<float,4>& colorRGBA){
    glBegin(GL_TRIANGLE_FAN);
    
    glColor4f(colorRGBA[0], colorRGBA[1], colorRGBA[2], colorRGBA[3]); // color del círculo
    glVertex2f(cx, cy); // centro
    
    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * M_PI * i / segments;
        float x = cx + cos(angle) * r;
        float y = cy + sin(angle) * r;
        glVertex2f(x, y);
    }
    
    glEnd();
}

void drawHorizon(float cx, float cy, float r, int points, int segments, const std::array<float,3>& colorRGB){
    glLineWidth(3.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glBegin(GL_LINE_STRIP);
    
    
    bool alphaB = false;
    int segment_size = points/segments;
    
    for (size_t i = 0; i <= points; i++)
    {
        float angle = 2.0f * M_PI * i / points;
        float x = cx + cos(angle) * r;
        float y = cy + sin(angle) * r;
        
        alphaB = (i % segment_size == 0) ? !alphaB : alphaB;

        float alpha = (alphaB == true) ? 1 : 0;
        
        glColor4f(colorRGB[0], colorRGB[1], colorRGB[2], alpha);
        glVertex2f(x, y);
    }
        
    glEnd();
}

void drawTrail(Photon& p, bool polar){
    
    glLineWidth(3.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glBegin(GL_LINE_STRIP);
    glColor4f(1.0,1.0,1.0,1.0);
    
    
    for(size_t i = 0; i < p.trail.size; i++){
        size_t idx = (p.trail.start + i) % p.trail.ptsTrailMax;
        float a = (p.trail.size > 1) ? float(i) / float(p.trail.size - 1) : 1.0f;
        float alpha = std::pow(a, 2.5f);
        glColor4f(1.0,1.0,1.0,alpha);
        if(polar){
            glVertex2f(p.trail.R[idx] * std::cos(p.trail.theta[idx]), p.trail.R[idx] * std::sin(p.trail.theta[idx]));
        }else{
            glVertex2f(p.trail.R[idx],p.trail.theta[idx]);
            }
    }
    
    glEnd();
}
