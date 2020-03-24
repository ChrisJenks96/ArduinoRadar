#ifndef FONT_H
#define FONT_H

#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
#include <GL/glut.h>

class Text
{
    public:
        Text(){};
        Text(GLfloat rgb[3], GLfloat xy[2]);
        void Render(char *format, ...);
        ~Text(){};
    private:
        GLfloat xy[2];
        GLfloat rgb[3];
        //fixed size buffer for formatted text
        char buffer[128];
};

#endif