#ifndef SCANNER_H
#define SCANNER_H

#include "primitives.h"

class Scanner
{
    public:
        Scanner(){}
        Scanner(GLfloat x, GLfloat y, GLfloat length, GLfloat angle);
        void Update(GLfloat angleIncrement);
        void Render();
        ~Scanner(){delete line;}
    private:
        OpenGLPrimLine* line;
};

#endif