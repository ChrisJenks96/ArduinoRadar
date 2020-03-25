#ifndef SCANNER_H
#define SCANNER_H

#include "primitives.h"

class Scanner
{
    public:
        Scanner(){}
        Scanner(GLfloat x, GLfloat y, GLfloat length, GLfloat angle);
        void Update(GLfloat angleIncrement);
        OpenGLPrimLine* GetLine(){return line;}
        void Render(unsigned int renderType);
        ~Scanner(){delete line;}
    private:
        OpenGLPrimLine* line;
};

#endif