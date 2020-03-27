#include "font.h"

Text::Text(GLfloat rgb[3], GLfloat xy[2])
{
    memcpy(&this->rgb[0], &rgb[0], sizeof(GLfloat) * 3);
    memcpy(&this->xy[0], &xy[0], sizeof(GLfloat) * 2);
    //load the bmp then create a GL texture object for it
    if (LoadBMP())
        GenGLTexture();
}

bool Text::LoadBMP()
{
    FILE* f = fopen("font.bmp", "rb");
    if (f)
    {
        fseek(f, 18, SEEK_SET);
        fread(&bmp.width, 4, 1, f);
        fread(&bmp.height, 4, 1, f);
        fseek(f, 2, SEEK_CUR);
        fread(&bmp.bitsPerPlane, 2, 1, f);
        printf("%i %i %i\n", bmp.width, bmp.height, bmp.bitsPerPlane);
        //width * height * rgb char (3)
        bmp.data = new unsigned char[bmp.width * bmp.height * 3];
        //offset to start of pixels (not palette on 24 bpp)
        fseek(f, 54, SEEK_SET);
        fread(&bmp.data[0], (bmp.width * bmp.height) * 3, 1, f);
        fclose(f);
        return true;
    }

    return false;
}

void Text::GenGLTexture()
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.width, bmp.height, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
    delete bmp.data;
}

void Text::Render(const char *format, ...)
{
    /*const char *c;
    va_list aptr;
    int ret;
    va_start(aptr, format);
    ret = vsprintf(buffer, format, aptr);
    va_end(aptr);
    glColor3f(rgb[0], rgb[1], rgb[2]);
    glRasterPos2f(xy[0], xy[1]);
    for (c = buffer; *c != '\0'; c++) { 
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }*/

    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);                      // Draw A Quad
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 256.0f); // Top Left
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 256.0f, 256.0f); // Top Right
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 256.0f,0.0f); // Bottom Right
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f,0.0f); // Bottom Left
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}