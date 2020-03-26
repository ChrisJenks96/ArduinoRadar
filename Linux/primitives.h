#ifndef PRIMS_H
#define PRIMS_H

#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <GL/gl.h>

#include "common.h"

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
        void SetNumVertices(unsigned int nv){numVertices = nv;}
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
        void SetXY(GLfloat xy[2]){x = xy[0]; y = xy[1];}
        void AddXY(GLfloat xy[2]){x += xy[0]; y += xy[1];}
        void SubXY(GLfloat xy[2]){x -= xy[0]; y -= xy[1];}
        void RecalculateCircle();
        void Render(){}
        void Render(unsigned int renderType);
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
        //if you want to return direction x,y ... pass in 0.0f as param else you can add a length
        GLfloat GetDirectionX(GLfloat len){return x + cosf(angle) * len;}
        GLfloat GetDirectionY(GLfloat len){return y + sinf(angle) * len;}
        GLfloat SetLength(GLfloat len){length = len;}
        GLfloat GetAngle(){return angle;}
        void SetAngle(GLfloat newAngle){angle = newAngle;}
        void addAngle(GLfloat newAngle){angle += newAngle;}
        void subAngle(GLfloat newAngle){angle -= newAngle;}
        void Render(){};
        void Render(unsigned int renderType);
        ~OpenGLPrimLine(){}
    private:
        GLfloat x, y;
        GLfloat length;
        GLfloat angle;
};

#endif