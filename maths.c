#include "maths.h"



float vlen(float x,float y,float z)
{
    return(sqrt(x*x+y*y+z*z));
}

float dot(float x1,float y1,float z1,float x2,float y2,float z2)
{
    return(x1*x2 + y1*y2 + z1*z2);    
}

void cross(float x1,float y1,float z1,float x2,float y2,float z2 , float *x,float *y,float *z)
{
    *x = y1*z2 - z1*y2;
    *y = z1*x2 - x1*z2;
    *z = x1*y2 - y1*x2;
}

