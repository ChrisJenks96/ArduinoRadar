#ifndef COMMON_H
#define COMMON_H

#include <math.h>

//THREAD

#define LOGIC_THREAD 0
#define LOG_THREAD 1

//MATH

#define MATH_PI 3.14159f
#define MATH_RAD_TO_DEG(x) (x * (180.0f / MATH_PI))
#define MATH_DEG_TO_RAD(x) (x * (MATH_PI / 180.0f))

#define RENDER_TYPE_LINE_LOOP 0
#define RENDER_TYPE_TRIANGLE_FAN 1

//get the angle between two vectors
static float XYToAngleDegrees(int xy[4])
{
    int dotProd = (xy[0] * xy[2]) + (xy[1] * xy[3]);
    int xy1Magnitude = sqrt((xy[0] * xy[0]) + (xy[1] * xy[1]));
    int xy2Magnitude = sqrt((xy[2] * xy[2]) + (xy[3] * xy[3]));
    return MATH_RAD_TO_DEG(cosf((float)dotProd / ((float)xy1Magnitude * (float)xy2Magnitude)));
}

static float XYToAngleRadians(int xy[4])
{
    return MATH_DEG_TO_RAD(XYToAngleDegrees(xy));
}

#endif