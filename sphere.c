#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "vp.h"
#include "rt.h"

// this function checks whether some ray intersects some sphere
// it returns whether 0 or 1 depending on whether it is true or false
// it sets the t variable, intersection point variable, and normal variable

int intersect_sphere(RAY_T ray, OBJ_T *object, double *t, 
                    VP_T *int_pt /*intersection point*/, VP_T *normal) {
    int to_return;
    double A;
    double B;
    double C;
    SPHERE_T sphere = object->sphere;

    A = 1;

    // dot product between ray's direction vector and the vector from the sphere's center to the ray's origin.
    B = 2 * (ray.dir.x * (ray.origin.x - sphere.center.x) +
            ray.dir.y * (ray.origin.y - sphere.center.y) +
            ray.dir.z * (ray.origin.z - sphere.center.z));

    // squared eclidean distance from the ray's origin to the center of the sphere
    C = (((ray.origin.x - sphere.center.x) * (ray.origin.x - sphere.center.x)) +
         ((ray.origin.y - sphere.center.y) * (ray.origin.y - sphere.center.y)) +
         ((ray.origin.z - sphere.center.z) * (ray.origin.z - sphere.center.z)) - 
         ((sphere.radius) * (sphere.radius)));

    double discriminant = ((B*B) - (4*A*C));
    
    
    if (discriminant <= 0) {
        to_return = 0;
    }

    else {
        to_return = 1;
        double t1 = ((-B + sqrt(discriminant)) / (2*A));
        double t2 = ((-B - sqrt(discriminant)) / (2*A));

        // for using whichever t value is closer to origin
        if (t1 <= t2) {
            *t = t1;
        }
        else {
            *t = t2;
        }
        int_pt->x = ray.origin.x + (*t) *ray.dir.x;
        int_pt->y = ray.origin.y + (*t) *ray.dir.y;
        int_pt->z = ray.origin.z + (*t) *ray.dir.z;

        normal->x = (int_pt->x - sphere.center.x);
        normal->y = (int_pt->y - sphere.center.y);
        normal->z = (int_pt->z - sphere.center.z);
        *normal = normalize(normal);

    }

    return to_return;
}