#include "scanner.h"

Scanner::Scanner(GLfloat x, GLfloat y, GLfloat* length, GLfloat angle, GLfloat angleIncrement)
{
    this->angleIncrement = angleIncrement;
    line = new OpenGLPrimLine(x, y, length, angle);
}

void Scanner::Update()
{
    //work out destination point as it relies on the angle
    line->SetVertex(2, line->GetX() + cosf(line->GetAngle()) * line->GetLength());
    line->SetVertex(3, line->GetY() + sinf(line->GetAngle()) * line->GetLength());
    line->addAngle(angleIncrement);
    if (line->GetAngle() > (MATH_PI * 2)){
        line->SetAngle(0.0f);
    }
}

void Scanner::Render(unsigned int renderType)
{
    line->Render(renderType);
}