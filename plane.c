#include <stdio.h>
#include <math.h>

#include "plane.h"

// this function checks whether some ray intersects some plane
// it returns whether 0 or 1 depending on whether it is true or false
// it sets the t variable, intersection point variable, and normal variable

int intersect_plane(RAY_T ray, OBJ_T *obj,  double *t, VP_T *int_pt, VP_T *normal /* for calculating light */) {

    PLANE_T plane = obj->plane;
    int to_return = 1;
    
    double dp = dot_product(ray.dir, plane.normal);

    if (dp == 0) {
        return 0;
    }
    
    *t = -(dot_product(plane.normal, ray.origin) + plane.distance) / dp;
    if (*t < 0) /* behind us */ {
        return 0;
    }
    int_pt->x = ray.origin.x + (*t) *ray.dir.x;
    int_pt->y = ray.origin.y + (*t) *ray.dir.y;
    int_pt->z = ray.origin.z + (*t) *ray.dir.z;

    normal->x = (plane.normal.x);
    normal->y = (plane.normal.y);
    normal->z = (plane.normal.z);
    *normal = normalize(normal);
    return 1;
}