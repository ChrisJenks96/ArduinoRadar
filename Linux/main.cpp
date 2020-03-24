#include "core.h"
#include "primitives.h"
#include "scanner.h"

int main(int argc, char** argv)
{
    unsigned int width = 800, height = 600;
    Core core(width, height);

    //create the circle which dictates the range the ultrasonic sensor can go to
    GLfloat circleRadius = 250.0f;
    GLfloat circleX = (GLfloat)width / 2.0f, circleY = (GLfloat)height / 2.0f;
    OpenGLPrimCircle circle(circleRadius, circleX, circleY, 30);
    //set up the tracking which rotates around the origin of the circle
    //detects any obstructions from the ultrasonic sensor
    GLfloat scannerStartingAngle = 0.0f;
    Scanner scanner(circleX, circleY, circleRadius, scannerStartingAngle);

    while (1)
    {
        //this updates the window
        core.Update();
        //update the angle on the tracking line
        scanner.Update(0.01f);
        //call this before rendering anything
        core.PreGLRender();
        //render stuff here...
        circle.Render();
        scanner.Render();
        //call this once render is complete
        core.SwapBuffer();
    }

    return 0;
}