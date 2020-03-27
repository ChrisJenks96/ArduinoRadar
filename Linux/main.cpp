#include "core.h"
#include "primitives.h"
#include "scanner.h"
#include "font.h"
#include "logger.h"
#include "port.h"

//https://www.bogotobogo.com/cplusplus/C11/1_C11_creating_thread.php
#include <thread>

//this function is allocated to the logger thread
void log_update(Logger* log, Scanner* scanner)
{
    //printf("LOG_THREAD log_update start\n");
    GLfloat hitCircleXY[] = {scanner->GetLine()->GetDirectionX(), 
        scanner->GetLine()->GetDirectionY()};
    log->Add(hitCircleXY[0], hitCircleXY[1], 
        MATH_RAD_TO_DEG(scanner->GetLine()->GetAngle()), scanner->GetLine()->GetLength());
    //printf("LOG_THREAD log_update done\n");
}

//this is for the general game updating thread
void game_update(Core* core, Scanner* scanner, Port* port, OpenGLPrim** prims)
{
    //printf("LOGIC_THREAD game_update start\n");
    //this updates the window
    core->Update();
    //if the COM port was not found at the start of execution then search for the COM port again
    if (port->GetCOMID() == -1){
        port->Scan();
    }

    //cast base classes to our derived classes
    OpenGLPrimCircle* circle = dynamic_cast<OpenGLPrimCircle*>(&(*prims[0]));
    OpenGLPrimCircle* hitCircle = dynamic_cast<OpenGLPrimCircle*>(&(*prims[1]));

    //boundary testing for the radar distance 
    if (scanner->GetLine()->GetLength() >= circle->GetRadius())
        scanner->GetLine()->SetLength(circle->GetRadius());
    if (scanner->GetLine()->GetLength() <= 0.0f)
        scanner->GetLine()->SetLength(0.0f);

    //update the angle on the tracking line
    scanner->Update();
    //when the radar has hit something, this is the positional vector
    GLfloat hitCircleXY[] = {scanner->GetLine()->GetDirectionX(), 
        scanner->GetLine()->GetDirectionY()};
    hitCircle->SetXY(hitCircleXY);
    //call this to recalculate the new circle
    hitCircle->RecalculateCircle();

    //printf("LOGIC_THREAD game_update done\n");
}

int main(int argc, char** argv)
{
    //allocate threads to critical tasks
    unsigned int currentThreads = std::thread::hardware_concurrency();
    std::thread* threads = new std::thread[currentThreads];

    //INIT ALL SYSTEMS
    
    //this is merely used for text right now...
    glutInit(&argc, argv);
    Port port(18);
    unsigned int width = 800, height = 600;
    Core core(width, height);
    //performance logging
    clock_t beginFrame = 0, endFrame = 0;
    unsigned int deltaTime = 0, frames = 0;
    Logger log(32);

    //GAME SPECIFIC CODE

    //if we hit something using radar, use this value to alter it
    //when nothing is hit, reset back to the circle radius
    GLfloat radarHitDistance = 125.0f;
    //used for showing frame comparisons from the log
    snapshotFrame lastFrame, beforeLastFrame;
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
    Scanner scanner(circleX, circleY, &radarHitDistance, scannerStartingAngle, 0.01f);
    //when the ultrasonic sensor hits something, this will be the prim that shows us where
    OpenGLPrimCircle hitCircle(5.0f, circleX, circleY, 8);

    while (1)
    {
        //get the starting frame time
        beginFrame = clock();

        //printf("MAIN_THREAD main() start\n");
        //UPDATE CODE
        OpenGLPrim* prims[] = {&circle, &hitCircle};
        threads[LOGIC_THREAD] = std::thread(&game_update, &core, &scanner, &port, prims);
        threads[LOG_THREAD] = std::thread(&log_update, &log, &scanner);

        int currentHeadingData[] = {lastFrame.x, lastFrame.y, beforeLastFrame.x, beforeLastFrame.y};
        float currentHeadingDegrees = XYToAngleDegrees(currentHeadingData);
        int logSize = log.Size();

        //generate the vertex/tex coord information on the CPU
        text.CreateVertexTexCoordBuffer(
            "MONITOR MODE: TRUE\n"
            "DELTA TIME: %u\n"
            "SERIAL PORT: %i\n"
            "SCANNER ANGLE: %.1f\n"
            "RADAR HIT DISTANCE: %.1f\n"
            "LAST LOG[%i]: %s|%u|%u", deltaTime, port.GetCOMID(), 
                MATH_RAD_TO_DEG(scanner.GetLine()->GetAngle()), radarHitDistance,
                    logSize-1, lastFrame.shortDateTime, lastFrame.angle, lastFrame.distance);

        //wait for the LOGIC_THREAD to finish
        threads[LOGIC_THREAD].join();
        //wait for LOG_THREAD to finish
        threads[LOG_THREAD].join();

        //RENDERING CODE - RUNS ON MAIN THREAD WHEN CPU LOGIC TASKS ARE DONE
        
        //call this before rendering anything
        core.PreGLRender();
        circle.Render(GL_LINE_LOOP);
        scanner.Render(GL_LINES);
        hitCircle.Render(GL_LINE_LOOP);
        //monitor mode will just allows us to view what's going on

        text.Render();

        //call this once render is complete
        core.SwapBuffer();

        //printf("MAIN_THREAD main() done\n");

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