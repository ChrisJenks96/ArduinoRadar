#include "core.h"

Core::Core()
{
    this->width = 640;
    this->height = 480;
    CreateWindow(640, 480);
}

Core::Core(unsigned int width, unsigned int height)
{
    CreateWindow(width, height);
    this->width = width;
    this->height = height;
}

void Core::CreateWindow(unsigned int width, unsigned int height)
{
    //NULL means graphical info to our local screen
    disp = XOpenDisplay(NULL);
    if (disp == NULL){
        printf("Cannot connect to the X server\n");
        exit(0);
    }

    //root is the desktop background window
    root = DefaultRootWindow(disp);
    //provide opengl with a specification of what we want (attr in header)
    vi = glXChooseVisual(disp, 0, coreAtt);
    if (vi == NULL){
        printf("No appropriate visual found\n");
        exit(0);
    }

    else
        printf("Visual %p selected\n", (void*)vi->visualid); //hex output glxinfo

    //create colour map based on settings
    cmap = XCreateColormap(disp, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    //Set which events we want to include via bitwise or mask
    swa.event_mask = ExposureMask | KeyPressMask;

    //create main window based off gl settings, event masks, colormap
    window = XCreateWindow(disp, root, 0, 0, width, height, 0, 
        vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    
    //make window appear
    XMapWindow(disp, window);
    //title bar setup
    XStoreName(disp, window, "ArduinoRadarApp");

    //bind opengl to the window
    context = glXCreateContext(disp, vi, NULL, GL_TRUE);
    glXMakeCurrent(disp, window, context);
    //enable textures
    glEnable(GL_TEXTURE_2D);
}

void Core::SetWindowTitle(const char *format, ...)
{
    char buffer[32];
    va_list aptr;
    int ret;
    va_start(aptr, format);
    ret = vsprintf(buffer, format, aptr);
    va_end(aptr);
    XStoreName(disp, window, buffer);
}

void Core::Update()
{
    //get event information per this frame
    XGetWindowAttributes(disp, window, &gwa);
}

void Core::PreGLRender()
{
    glViewport(0.0f, 0.0f, width, height);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, 0.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void Core::SwapBuffer()
{
    glXSwapBuffers(disp, window);
}

Core::~Core()
{
    glXMakeCurrent(disp, None, NULL);
    glXDestroyContext(disp, context);
    XDestroyWindow(disp, window);
    XCloseDisplay(disp);
}