#include "core.h"
#include "primitives.h"
#include "scanner.h"
#include "font.h"
#include "logger.h"
#include "port.h"

int main(int argc, char** argv)
{
    //this is merely used for text right now...
    glutInit(&argc, argv);
    unsigned int width = 800, height = 600;
    Core core(width, height);

    //if we hit something using radar, use this value to alter it
    //when nothing is hit, reset back to the circle radius
    GLfloat radarHitDistance = 125.0f;
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

    Logger log(32);
    Port port(18);
    port.Scan();

    while (1)
    {
        //boundary testing for the radar distance 
        if (radarHitDistance >= circleRadius)
            radarHitDistance = circleRadius;
        
        //this updates the window
        core.Update();
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

        log.Add(hitCircleXY[0], hitCircleXY[1], MATH_RAD_TO_DEG(scanner.GetLine()->GetAngle()), radarHitDistance);

        text.SetXY(xy[0], xy[1]);
        text.Render("Monitor Mode: TRUE");
        text.AddXY(0.0f, 15.0f);

        //if we haven't connected to a serial port yet, scan until we do
        if (port.GetCOMID() == 0)
            port.Scan();
        text.Render("Serial Port: %i", port.GetCOMID());
        
        text.AddXY(0.0f, 15.0f);
        text.Render("Scanner Angle (Degrees): %f\n", MATH_RAD_TO_DEG(scanner.GetLine()->GetAngle()));
        text.AddXY(0.0f, 15.0f);
        text.Render("Radar Hit Distance: %f", radarHitDistance);
        text.AddXY(0.0f, 15.0f);
        //output all the relevant data
        if (log.Size() > 0){
            snapshotFrame lastFrame = log.GetLastFrame();
            text.Render("Last Log[%i]: %s|%u|%u", log.Size()-1, lastFrame.shortdateTime,
                lastFrame.angle, lastFrame.distance);
        }
        //if there is no data, we output nothing
        else
            text.Render("Last Log: NULL");

        text.AddXY(0.0f, 15.0f);
        //find heading between two logged points
        if (log.Size() >= 2){
            snapshotFrame lastFrame = log.GetLastFrame();
            snapshotFrame beforeLastFrame = log.GetFrame((log.Size()-1)-1);
            text.Render("Current Heading: (%u,%u)", 
                (lastFrame.x - beforeLastFrame.x), (lastFrame.y - beforeLastFrame.y));
            unsigned int xy[] = {lastFrame.x, lastFrame.y, beforeLastFrame.x, beforeLastFrame.y};
            text.AddXY(0.0f, 15.0f);
            text.Render("Current Heading Angle: %f", XYToAngleDegrees(xy));
        }

        else
        {
            text.Render("Current Heading: 2 Log Samples Required");
            text.AddXY(0.0f, 15.0f);
            text.Render("Current Heading Angle: 2 Log Samples Required");
        }
        
        //call this once render is complete
        core.SwapBuffer();
    }

    return 0;
}