#include "core.h"
#include "primitives.h"

int main(int argc, char** argv)
{
    Core core(800, 600);
    OpenGLPrimCircle circle(250.0f, 400.0f, 300.0f, 30);
    while (1)
    {
        //this updates the window
        core.Update();
        //call this before rendering anything
        core.PreGLRender();
        //render stuff here...
        circle.Render();
        //call this once render is complete
        core.SwapBuffer();
    }

    return 0;
}