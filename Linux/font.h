#ifndef FONT_H
#define FONT_H

#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
#include <GL/glut.h>

typedef struct BMP
{
    unsigned int width; //18 byte offset
    unsigned int height; //22 byte offset
    unsigned short bitsPerPlane; //28 byte offset
    unsigned char* data;
} BMP;

class Text
{
    public:
        Text(){};
        Text(GLfloat rgb[3], GLfloat xy[2]);
        void SetXY(GLfloat x, GLfloat y){xy[0] = x; xy[1] = y;}
        void AddXY(GLfloat x, GLfloat y){xy[0] += x; xy[1] += y;}
        void Render(const char *format, ...);
        ~Text(){glDeleteTextures(1, &textureID);}
    private:
        BMP bmp;
        GLuint textureID;
        GLfloat xy[2];
        GLfloat rgb[3];
        //fixed size buffer for formatted text
        char buffer[128];

        bool LoadBMP();
        void GenGLTexture();
};

#endif