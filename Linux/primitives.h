#ifndef PRIMS_H
#define PRIMS_H

#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <GL/gl.h>

#define MATH_PI 3.14159f
#define MATH_RAD_TO_DEG(x) (x * (180.0f / MATH_PI))
#define MATH_DEG_TO_RAD(x) (x * (MATH_PI / 180.0f))

//basic base class for opengl primitives
class OpenGLPrim
{
    public:
        OpenGLPrim();
        void AllocColourVertices(int numVertices, GLfloat col[3]);
        void FreeColourVertices();
        void SetVertex(unsigned int id, GLfloat value);
        GLfloat* GetVertex(){return vertices;}
        GLfloat* GetRGB(){return rgb;}
        unsigned int GetVerticesCount(){return numVertices;}
        virtual void Render() = 0;
        ~OpenGLPrim(){FreeColourVertices();}
    private:
        GLfloat* rgb;
        GLfloat* vertices;
        unsigned int numVertices;
};

class OpenGLPrimCircle : public OpenGLPrim
{
    public:
        OpenGLPrimCircle(){}
        OpenGLPrimCircle(GLfloat radius, GLfloat x, GLfloat y, unsigned int step);
        void Render();
        ~OpenGLPrimCircle(){}
    private:
        GLfloat radius;
        GLfloat x, y;
        unsigned int step;
};

class OpenGLPrimLine : public OpenGLPrim
{
    public:
        OpenGLPrimLine(){}
        OpenGLPrimLine(GLfloat x, GLfloat y, GLfloat length, GLfloat angle);
        GLfloat GetX(){return x;}
        GLfloat GetY(){return y;}
        GLfloat GetLength(){return length;}
        GLfloat GetAngle(){return angle;}
        void SetAngle(GLfloat newAngle){angle = newAngle;}
        void addAngle(GLfloat newAngle){angle += newAngle;}
        void subAngle(GLfloat newAngle){angle -= newAngle;}
        void Render();
        ~OpenGLPrimLine(){}
    private:
        GLfloat x, y;
        GLfloat length;
        GLfloat angle;
};

#endif