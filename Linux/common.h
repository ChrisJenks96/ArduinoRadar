#ifndef COMMON_H
#define COMMON_H

#include <math.h>

#define MATH_PI 3.14159f
#define MATH_RAD_TO_DEG(x) (x * (180.0f / MATH_PI))
#define MATH_DEG_TO_RAD(x) (x * (MATH_PI / 180.0f))

#define RENDER_TYPE_LINE_LOOP 0
#define RENDER_TYPE_TRIANGLE_FAN 1

//get the angle between two vectors
static float XYToAngleDegrees(unsigned int xy[4])
{
    unsigned int dotProd = (xy[0] * xy[2]) + (xy[1] * xy[3]);
    unsigned int xy1Mag = sqrt((xy[0] * xy[0]) + (xy[1] * xy[1]));
    unsigned int xy2Mag = sqrt((xy[2] * xy[2]) + (xy[3] * xy[3]));
    return MATH_RAD_TO_DEG(cosf(dotProd / (xy1Mag * xy2Mag)));
}

static float XYToAngleRadians(unsigned int xy[4])
{
    return MATH_DEG_TO_RAD(XYToAngleDegrees(xy));
}

#endif