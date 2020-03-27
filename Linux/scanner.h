#ifndef SCANNER_H
#define SCANNER_H

#include "primitives.h"

class Scanner
{
    public:
        Scanner(){}
        Scanner(GLfloat x, GLfloat y, GLfloat* length, GLfloat angle, GLfloat angleIncrement);
        void Update();
        OpenGLPrimLine* GetLine(){return line;}
        void Render(unsigned int renderType);
        ~Scanner(){delete line;}
    private:
        OpenGLPrimLine* line;
        GLfloat angleIncrement;
};

#endif