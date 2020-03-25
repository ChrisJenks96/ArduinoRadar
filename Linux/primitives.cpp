#include "primitives.h"

//BASE CLASS

OpenGLPrim::OpenGLPrim()
{
    rgb = NULL;
    vertices = NULL;
    numVertices = 0;
}

void OpenGLPrim::AllocColourVertices(int numVertices, GLfloat col[3])
{
    this->numVertices = numVertices;
    rgb = new GLfloat[3];
    memcpy(&rgb[0], &col[0], sizeof(GLfloat) * 3);
    vertices = new GLfloat[this->numVertices];
}

void OpenGLPrim::FreeColourVertices()
{
    if (rgb != NULL)
        delete rgb;
    if (vertices != NULL)
        delete vertices;
}

void OpenGLPrim::SetVertex(unsigned int id, GLfloat value)
{
    //the id has to be less than the max allocated vertices
    if (id < this->numVertices){
        vertices[id] = value;
    }

    else
        printf("id is out of bounds");
}

//CIRCLE CLASS

OpenGLPrimCircle::OpenGLPrimCircle(GLfloat radius, GLfloat x, GLfloat y, unsigned int step)
{
    //set up default values for the circles
    this->radius = radius;
    this->x = x;
    this->y = y;
    this->step = step;

    //default green colour
    GLfloat default_colour[] = {0.0f, 1.0f, 0.0f};
    //* 2 is for x and y as GLfloat isn't 2d array
    AllocColourVertices(step * 2, default_colour);

    GLfloat theta = 0.0f;
    unsigned int counter = 0;
    for (int i = 0; i < step; i++)
    {
        theta = 2.0f * MATH_PI * (float(i) / (float)step);
        SetVertex(counter, x + radius * cosf(theta));
        SetVertex(counter+1, y + radius * sinf(theta));
        counter+=2; 
    }
}

void OpenGLPrimCircle::RecalculateCircle()
{
    GLfloat theta = 0.0f;
    unsigned int counter = 0;
    for (int i = 0; i < step; i++)
    {
        theta = 2.0f * MATH_PI * (float(i) / (float)step);
        SetVertex(counter, x + radius * cosf(theta));
        SetVertex(counter+1, y + radius * sinf(theta));
        counter+=2; 
    }
}

void OpenGLPrimCircle::Render(unsigned int renderType)
{
    unsigned int index = 0, counter = 0;
    GLfloat* rgb = GetRGB();
    GLfloat* vertices = GetVertex();
    glBegin(renderType);   
        for (; index < step; index++){
            glColor3f(rgb[0], rgb[1], rgb[2]); glVertex2f(vertices[counter], vertices[counter+1]);
            counter+=2;
        }
    glEnd();
}

//LINE CLASS

OpenGLPrimLine::OpenGLPrimLine(GLfloat x, GLfloat y, GLfloat length, GLfloat angle)
{
    //set up default values for the circles
    this->x = x;
    this->y = y;
    this->angle = angle;
    this->length = length;

    //default green colour
    GLfloat default_colour[] = {0.0f, 1.0f, 0.0f};
    //alloc 2 verts origin and dest
    AllocColourVertices(4, default_colour);

    //origin point
    SetVertex(0, x);
    SetVertex(1, y);

    SetVertex(2, x + cosf(angle) * length);
    SetVertex(3, y + sinf(angle) * length);
}

void OpenGLPrimLine::Render(unsigned int renderType)
{
    GLfloat* rgb = GetRGB();
    GLfloat* vertices = GetVertex();

    glBegin(renderType);
        glColor3f(rgb[0], rgb[1], rgb[2]); glVertex2f(vertices[0], vertices[1]);
        glColor3f(rgb[0], rgb[1], rgb[2]); glVertex2f(vertices[2], vertices[3]);
    glEnd();
}