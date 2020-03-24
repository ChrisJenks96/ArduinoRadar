#include "font.h"

Text::Text(GLfloat rgb[3], GLfloat xy[2])
{
    memcpy(&this->rgb[0], &rgb[0], sizeof(GLfloat) * 3);
    memcpy(&this->xy[0], &xy[0], sizeof(GLfloat) * 2);
}

void Text::Render(char *format, ...)
{
    const char *c;
    va_list aptr;
    int ret;
    va_start(aptr, format);
    ret = vsprintf(buffer, format, aptr);
    va_end(aptr);
    glColor3f(rgb[0], rgb[1], rgb[2]);
    glRasterPos2f(xy[0], xy[1]);
    for (c = buffer; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}