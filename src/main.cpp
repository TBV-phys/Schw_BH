#include <iostream>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>

#include "physics/physicsBH.h"
#include "physics/RK4.h"
#include "render/draws.h"


struct AppState {
    std::vector<Photon> photons;
    double lastMouseX   = 0;
    double lastMouseY   = 0;
    int winWidth        = 800;
    int winHeight       = 800;
    float rangeX        = 10;
    int photonBurst     = 15;
};

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    AppState* myBH = static_cast<AppState*>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &myBH->lastMouseX, &myBH->lastMouseY);

        double mx, my;
        mx = (myBH->lastMouseX / myBH->winWidth) * 2 * (myBH->rangeX) - myBH->rangeX;
        my = myBH->rangeX - (myBH->lastMouseY / myBH->winHeight) * 2 * (myBH->rangeX);

        for (size_t i = 0; i < myBH->photonBurst; i++)
        {
            float alpha0 = 2.0f * M_PI * i / (myBH->photonBurst);
            float theta0 = (mx > 0) ? std::atan(my/mx) : M_PI + std::atan(my/mx);
            myBH->photons.emplace_back(std::sqrt(mx * mx + my * my), theta0, alpha0);
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    AppState* myBH = static_cast<AppState*>(glfwGetWindowUserPointer(window));
    myBH->lastMouseX = xpos;
    myBH->lastMouseY = ypos;
}

// MAIN

int main(void)
{
    AppState myBH;
    
    // Inicialización GLFW
    if (!glfwInit()) return -1;
    
    /* For use of Core 4.1 and Shaders
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // OBLIGATORIO en Mac
     */

    GLFWwindow* window = glfwCreateWindow(myBH.winWidth, myBH.winHeight, "NullGeo_SchwarzschildBH", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-myBH.rangeX, myBH.rangeX, -myBH.rangeX, myBH.rangeX, -1, 1);

    // Asociar estado a la ventana
    glfwSetWindowUserPointer(window, &myBH);

    // Set callbacks
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Inicializar fotones manualmente
   
    std::vector<double> initialB = {
        4, -4,
        3.75, -3.75,
        3.5, -3.5,
        3.25, -3.25,
        3, -3,
        std::sqrt(27)/2, -std::sqrt(27)/2,
        2.25, -2.25,
        2, -2,
        1.75, -1.75,
        1.5, -1.5,
        1.25, -1.25,
        1, -1,
        0.75, -0.75,
        0.5, -0.5,
        0.25, -0.25,
        0
    };
    
    
    for (auto b : initialB)
        myBH.photons.emplace_back(b, -myBH.rangeX);

    float dt  = 0.1;
    float tol = 0.01;

    // Loop principal
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        drawGrid(10, 10, {-myBH.rangeX, myBH.rangeX, -myBH.rangeX, myBH.rangeX});
        drawCircle(0,0,1,30,{0.3,0.0,0.0,1.0});
        drawHorizon(0, 0, 1.5, 100, 50, {0.4,0.0,0.0});

        for (auto& p : myBH.photons)
        {
            // Draws
            drawCircle(p.pos.R * std::cos(p.pos.theta), p.pos.R * std::sin(p.pos.theta), 0.02, 5, {1.0, 1.0, 1.0, p.fadeFactor});
            drawTrail(p, true);
            // Calculations for Active photons
            if(p.state == PhotonState::Active){
                // Integrador
                RK4(p.pos, dt, DSystem, p.b, tol);
                // Trail update
                if (p.trail.size < p.trail.ptsTrailMax)
                {
                    p.trail.R.push_back(p.pos.R);
                    p.trail.theta.push_back(p.pos.theta);
                    p.trail.size = p.trail.R.size();
                }
                else
                {
                    p.trail.R[p.trail.start] = p.pos.R;
                    p.trail.theta[p.trail.start] = p.pos.theta;
                    p.trail.start = (p.trail.start + 1) % p.trail.ptsTrailMax;
                }
                // New states
                if (p.pos.R < 1.0 + tol) {
                    p.state = PhotonState::Captured;
                }
                else if (p.pos.R > std::sqrt(2) * myBH.rangeX) {
                    p.state = PhotonState::Escaped;
                }
            }
            
            if (p.state == PhotonState::Captured || p.state == PhotonState::Escaped)
            {
                if (p.trail.size > 0)
                {
                    p.trail.start = (p.trail.start + 1) % p.trail.ptsTrailMax;
                    p.trail.size--;
                }else if(p.trail.size == 0 && p.fadeFactor > tol){
                    p.fadeFactor *= 0.95;
                }else{
                    p.state = PhotonState::Faded;
                }
            }


            
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //printf("%s\n", glGetString(GL_VERSION));
    //printf("%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glfwTerminate();
    
    return 0;
}

