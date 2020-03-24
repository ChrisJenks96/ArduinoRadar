#include "core.h"
#include "primitives.h"
#include "scanner.h"
#include "font.h"

int main(int argc, char** argv)
{
    //this is merely used for text right now...
    glutInit(&argc, argv);

    unsigned int width = 800, height = 600;
    Core core(width, height);

    GLfloat rgb[] = {0.0f, 1.0f, 0.0f};
    GLfloat xy[] = {10.0f, 30.0f};
    //create the font with col and position
    Text text(rgb, xy);

    //create the circle which dictates the range the ultrasonic sensor can go to
    GLfloat circleRadius = 250.0f;
    GLfloat circleX = (GLfloat)(width / 2.0f) + 125.0f, circleY = (GLfloat)height / 2.0f;
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

        text.Render("Scanner Angle (Degrees): %f\n", MATH_RAD_TO_DEG(scanner.GetLine()->GetAngle()));
        //call this once render is complete
        core.SwapBuffer();
    }

    return 0;
}