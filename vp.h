#ifndef VP_H
#define VP_H
#include <stdio.h>


typedef struct {
    double x;
    double y;
    double z;
} VP_T;

// normalize function prototype
VP_T normalize(VP_T *vector_point);
// dot product function prototype
double dot_product(VP_T v1, VP_T v2);
// subtract function prototype
VP_T vp_subtract(VP_T v1, VP_T v2);
// distance function prototype
double distance(VP_T v1, VP_T v2);
#endif