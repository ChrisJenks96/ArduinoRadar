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
    GLfloat xy[] = {5.0f, 15.0f};
    //create the font with col and position
    Text text(rgb, xy);
    //create the circle which dictates the range the ultrasonic sensor can go to
    GLfloat circleRadius = 250.0f;
    GLfloat circleX = (GLfloat)(width / 2.0f) + 125.0f, circleY = (GLfloat)height / 2.0f;
    OpenGLPrimCircle circle(circleRadius, circleX, circleY, 30);
    //set up the tracking which rotates around the origin of the circle
    //detects any obstructions from the ultrasonic sensor
    GLfloat scannerStartingAngle = MATH_DEG_TO_RAD(180.0f);
    Scanner scanner(circleX, circleY, circleRadius, scannerStartingAngle);

    //when the ultrasonic sensor hits something, this will be the prim that shows us where
    OpenGLPrimCircle hitCircle(5.0f, circleX, circleY, 8);

    while (1)
    {
        //this updates the window
        core.Update();
        //if we hit something using radar, use this value to alter it
        //when nothing is hit, reset back to the circle radius
        GLfloat radarHitDistance = 125.0f;
        //update the angle on the tracking line
        scanner.Update(0.01f);
        //call this before rendering anything
        core.PreGLRender();
        //render stuff here...
        circle.Render(GL_LINE_LOOP);

        scanner.GetLine()->SetLength(radarHitDistance);
        scanner.Render(GL_LINES);

        //when the radar has hit something, this is the positional vector
        GLfloat hitCircleXY[] = {scanner.GetLine()->GetDirectionX(radarHitDistance), 
            scanner.GetLine()->GetDirectionY(radarHitDistance)};
        hitCircle.SetXY(hitCircleXY);
        //call this to recalculate the new circle
        hitCircle.RecalculateCircle();
        hitCircle.Render(GL_LINE_LOOP);

        text.SetXY(xy[0], xy[1]);
        text.Render("Monitor Mode: TRUE");
        text.AddXY(0.0f, 15.0f);
        text.Render("Serial Port: %i", -1);
        text.AddXY(0.0f, 15.0f);
        text.Render("Scanner Angle (Degrees): %f\n", MATH_RAD_TO_DEG(scanner.GetLine()->GetAngle()));
        text.AddXY(0.0f, 15.0f);
        text.Render("Radar Hit Distance: %f", radarHitDistance);
        //call this once render is complete
        core.SwapBuffer();
    }

    return 0;
}