#include "font.h"

Text::Text(GLfloat rgb[3], GLfloat xy[2])
{
    memcpy(&this->rgb[0], &rgb[0], sizeof(GLfloat) * 3);
    memcpy(&this->xy[0], &xy[0], sizeof(GLfloat) * 2);
    //load the bmp then create a GL texture object for it
    if (LoadBMP())
        GenGLTexture();
    totalCharacters = 0;
    texCoordinatesUV = NULL;
    vertexXY = NULL;
    //one time creations like buffers
    firstTimeFontBuffer = true;
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

void Text::CreateVertexTexCoordBuffer(const char *format, ...)
{
    //16 characters per row
    const char* c;
    //x and y in relation to the current character
    int x, y, index = 0, font_size = 12, font_char_counter_x = 0, font_char_counter_y = 0;
    //width and height in relativity to 0-1 tex coords
    float width = 1.0f / 16.0f;
    float height = 1.0f / 8.0f;

    char oldBuffer[bufferSize];
    //copy the current buffer into a temp one, this will be used
    //the double check against the letters, if they are the same, then dont update
    if (!firstTimeFontBuffer){
        memcpy(&oldBuffer[0], &buffer[0], bufferSize);
    }

    //if its the first time, fill it full of crap data so we'll fail any comparisons against current buffer
    else if (firstTimeFontBuffer)
        memset(&oldBuffer[0], '$', bufferSize);

    va_list aptr;
    int ret;
    va_start(aptr, format);
    ret = vsprintf(buffer, format, aptr);
    va_end(aptr);

    int oldTotalCharacters = totalCharacters;
    totalCharacters = strlen(buffer);
    //get the length of the buffer (* 8 = 2 axis (x and y) * 4 sides (quad))
    //if this is the first run on the program ,we need to malloc not realloc
    if (firstTimeFontBuffer){
        vertexXY = new GLfloat[totalCharacters * 8];
        texCoordinatesUV = new GLfloat[totalCharacters * 8];
    }

    //if the current total characters is not the same as the new buffer passed in, recreate
    if (totalCharacters != oldTotalCharacters){
        vertexXY = (GLfloat*)realloc(vertexXY, sizeof(GLfloat) * totalCharacters * 8);
        texCoordinatesUV = (GLfloat*)realloc(texCoordinatesUV, sizeof(GLfloat) * totalCharacters * 8);
    }

    int oldBufferIndex = 0;
    for (c = buffer; *c != '\0'; c++) 
    { 
        //if a newline character, then drop it a line and start the x again
        if ((*c) == '\n'){
            font_char_counter_x = -font_size;
            font_char_counter_y += (font_size + 4);
        }

        //if the new and old characters are the same, no point updating
        if ((*c) != oldBuffer[oldBufferIndex])
        {
            y = ((*c) / 16) + 1;
            x = ((*c) % 16) + 1;

            //generate the quad around the letter and push it to the vertex/tex coord arrays
            texCoordinatesUV[index] = (width * (x-1));
            texCoordinatesUV[index+1] = (height * y); 
            vertexXY[index] = (font_char_counter_x + font_size);
            vertexXY[index+1] = (font_char_counter_y + (font_size * 2)); // Top Left

            texCoordinatesUV[index+2] = (width * x);
            texCoordinatesUV[index+3] = (height * y); 
            vertexXY[index+2] = (font_char_counter_x + font_size * 2);
            vertexXY[index+3] = (font_char_counter_y + (font_size * 2)); // Top Right

            texCoordinatesUV[index+4] = (width * x);
            texCoordinatesUV[index+5] = (height * (y-1));
            vertexXY[index+4] = (font_char_counter_x + font_size * 2);
            vertexXY[index+5] = (font_char_counter_y + font_size); // Bottom Right

            texCoordinatesUV[index+6] = (width * (x-1));
            texCoordinatesUV[index+7] = (height * (y-1));

            vertexXY[index+6] = (font_char_counter_x + font_size);
            vertexXY[index+7] = (font_char_counter_y + font_size); // Bottom Left
        }

        //move the font size along on the x for the next character
        font_char_counter_x += font_size;
        //move onto the next set of vertex/tex coord indexes
        index += 8;

        oldBufferIndex++;
    }

    firstTimeFontBuffer = false;
}

void Text::Render()
{
    //bind the font texture
    int index = 0;
    glBindTexture(GL_TEXTURE_2D, textureID);
    for (int i = 0; i < totalCharacters; i++) 
    { 
        glBegin(GL_QUADS);                      // Draw A Quad
            glTexCoord2f(texCoordinatesUV[index], texCoordinatesUV[index+1]); glVertex2f(vertexXY[index], vertexXY[index+1]); // Top Left
            glTexCoord2f(texCoordinatesUV[index+2], texCoordinatesUV[index+3]); glVertex2f(vertexXY[index+2], vertexXY[index+3]); // Top Right
            glTexCoord2f(texCoordinatesUV[index+4], texCoordinatesUV[index+5]); glVertex2f(vertexXY[index+4], vertexXY[index+5]); // Bottom Right
            glTexCoord2f(texCoordinatesUV[index+6], texCoordinatesUV[index+7]); glVertex2f(vertexXY[index+6], vertexXY[index+7]); // Bottom Left
        glEnd();

        index += 8;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}