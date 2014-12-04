#ifndef NODE_MATHS_H
#define NODE_MATHS_H


#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "ctype.h"
#include "math.h"

#ifdef USE_MEMORY_DEBUGGING
#include "memory.h"
#endif

#define pi 3.1415926535897932384626433832795

//const double pi = 3.1415926535897932384626433832795;

typedef struct _vec4
{
	double x,y,z,w;

}vec4;

typedef struct _vec2
{
	double x,y;

}vec2;

//typedef double *mat4[4];
//typedef double *mat44[16];



#endif