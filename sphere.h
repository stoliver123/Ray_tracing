#ifndef SPHERE_H
#define SPHERE_H

#include "vp.h"
#include "rt.h"


int intersect_sphere(RAY_T ray, OBJ_T *object, double *t, 
                    VP_T *int_pt, VP_T *normal);

#endif