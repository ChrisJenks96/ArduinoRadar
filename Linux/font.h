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
        void CreateVertexTexCoordBuffer(const char *format, ...);
        void SetXY(GLfloat x, GLfloat y){xy[0] = x; xy[1] = y;}
        void AddXY(GLfloat x, GLfloat y){xy[0] += x; xy[1] += y;}
        void Render();
        ~Text(){glDeleteTextures(1, &textureID); delete texCoordinatesUV; delete vertexXY;}
    private:
        BMP bmp;
        GLuint textureID;
        GLfloat* texCoordinatesUV;
        GLfloat* vertexXY;
        GLfloat xy[2];
        GLfloat rgb[3];
        //fixed size buffer for formatted text
        static const int bufferSize = 256;
        char buffer[bufferSize];
        int totalCharacters;
        bool firstTimeFontBuffer;

        bool LoadBMP();
        void GenGLTexture();
};

#endif