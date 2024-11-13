#ifndef PLANE_H
#define PLANE_H

#include <stdio.h>

#include "rt.h"
#include "vp.h"

int intersect_plane(RAY_T ray, OBJ_T *object, double *t, VP_T *int_pt, VP_T *normal);

#endif 