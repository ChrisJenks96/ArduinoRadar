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
    rgb = new GLfloat(sizeof(GLfloat) * this->numVertices);
    memcpy(&rgb[0], &col[0], sizeof(GLfloat) * 3);
    vertices = new GLfloat(sizeof(GLfloat) * this->numVertices);
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

    //* 2 is for x and y as GLfloat isn't 2d array
    unsigned int numVertices = step * 2;
    //default green colour
    GLfloat default_colour[] = {0.0f, 1.0f, 0.0f};
    AllocColourVertices(numVertices, default_colour);

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

void OpenGLPrimCircle::Render()
{
    unsigned int index = 0, counter = 0;
    GLfloat* rgb = GetRGB();
    GLfloat* vertices = GetVertex();
    glBegin(GL_LINE_LOOP);
        for (; index < step; index++){
            glColor3f(rgb[0], rgb[1], rgb[2]); glVertex2f(vertices[counter], vertices[counter+1]);
            counter+=2;
        }
    glEnd();
}