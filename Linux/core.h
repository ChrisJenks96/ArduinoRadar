#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

static GLint coreAtt[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

class Core
{
    public:
        Core();
        Core(unsigned int width, unsigned int height);
        void Update();
        void PreGLRender();
        void SwapBuffer();
        ~Core();
    private:  
        Display* disp;
        Window root;
        XVisualInfo* vi;
        Colormap cmap;
        XSetWindowAttributes swa;
        Window window;
        GLXContext context;
        XWindowAttributes gwa;
        XEvent event;

        unsigned int width, height;
        void CreateWindow(unsigned int width, unsigned int height);   
};