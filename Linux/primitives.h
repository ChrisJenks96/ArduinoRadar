#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <GL/gl.h>

#define MATH_PI 3.14159f

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