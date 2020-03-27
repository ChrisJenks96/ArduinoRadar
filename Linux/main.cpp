#include "core.h"
#include "primitives.h"
#include "scanner.h"
#include "font.h"
#include "logger.h"
#include "port.h"

#include <thread>

int main(int argc, char** argv)
{
    Port port(18);
    //allocate threads to critical tasks
    unsigned int currentThreads = std::thread::hardware_concurrency();
    std::thread* threads = new std::thread[currentThreads];

    //start scanning for the ports while the main thread sets up
    threads[LOGIC_THREAD] = std::thread(&Port::Scan, &port);

    //this is merely used for text right now...
    glutInit(&argc, argv);
    unsigned int width = 800, height = 600;
    Core core(width, height);
    Logger log(32);

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

    //performance logging
    clock_t beginFrame = 0, endFrame = 0;
    unsigned int deltaTime = 0, frames = 0;

    //thread must be joined before join call and destruction
    if (threads[LOGIC_THREAD].joinable())
        threads[LOGIC_THREAD].join();

    //if the COM port is still not found, set flag for searching again
    bool COMPortSearch;
    if (port.GetCOMID() == -1)
        COMPortSearch = true;
    else
        COMPortSearch = false;

    while (1)
    {
        //get the starting frame time
        beginFrame = clock();

        //UPDATE CODE

        //if the COM port was not found at the start of execution then search for the COM port again
        if (port.GetCOMID() == -1){
            threads[LOGIC_THREAD] = std::thread(&Port::Scan, &port);
            COMPortSearch = true;
        }

        //boundary testing for the radar distance 
        if (radarHitDistance >= circleRadius)
            radarHitDistance = circleRadius;
        if (radarHitDistance <= 0.0f)
            radarHitDistance = 0.0f;
        //update the angle on the tracking line
        scanner.Update(0.01f);
        scanner.GetLine()->SetLength(radarHitDistance);
        //when the radar has hit something, this is the positional vector
        GLfloat hitCircleXY[] = {scanner.GetLine()->GetDirectionX(radarHitDistance), 
            scanner.GetLine()->GetDirectionY(radarHitDistance)};
        hitCircle.SetXY(hitCircleXY);
        //call this to recalculate the new circle
        hitCircle.RecalculateCircle();
        
        threads[LOG_THREAD] = std::thread(&Logger::Add, &log, hitCircleXY[0], hitCircleXY[1], MATH_RAD_TO_DEG(scanner.GetLine()->GetAngle()), radarHitDistance);
        //log.Add(hitCircleXY[0], hitCircleXY[1], MATH_RAD_TO_DEG(scanner.GetLine()->GetAngle()), radarHitDistance);

        //RENDERING CODE

        //this updates the window
        core.Update();
        
        //call this before rendering anything
        core.PreGLRender();
        
        circle.Render(GL_LINE_LOOP);
        scanner.Render(GL_LINES);
        hitCircle.Render(GL_LINE_LOOP);
        text.SetXY(xy[0], xy[1]);
        //for performance testing
        text.Render("Deltatime: %u", deltaTime);
        text.AddXY(0.0f, 15.0f);
        //monitor mode will just allows us to view what's going on
        text.Render("Monitor Mode: TRUE");
        text.AddXY(0.0f, 15.0f);
        //finish off the COM port search before printing out to screen
        if (COMPortSearch)
        {
            if (threads[LOGIC_THREAD].joinable())
                threads[LOGIC_THREAD].join();
            //if the port has been acquired, no need for thread to search for COM port
            if (port.GetCOMID() != -1)
                COMPortSearch = false;
        }

        text.Render("Serial Port: %i", port.GetCOMID());
        text.AddXY(0.0f, 15.0f);
        text.Render("Scanner Angle (Degrees): %f\n", MATH_RAD_TO_DEG(scanner.GetLine()->GetAngle()));
        text.AddXY(0.0f, 15.0f);
        text.Render("Radar Hit Distance: %f", radarHitDistance);
        text.AddXY(0.0f, 15.0f);
        
        //acquire all log data from seperate thread before log information printing
        if (threads[LOG_THREAD].joinable())
            threads[LOG_THREAD].join();

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
            snapshotFrame beforeLastFrame = log.GetFrame(log.Size()-1);
            text.Render("Current Heading: (%u,%u)", 
                (lastFrame.x - beforeLastFrame.x), (lastFrame.y - beforeLastFrame.y));
            int xy[] = {lastFrame.x, lastFrame.y, beforeLastFrame.x, beforeLastFrame.y};
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

        endFrame = clock();
        frames++;
        //every 50 frames update the delta time 
        if (frames > 50){
            deltaTime = endFrame - beginFrame;
            frames = 0;
        }
    }

    delete threads;
    return 0;
}