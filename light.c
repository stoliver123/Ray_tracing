#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "vp.h"
#include "rt.h"
#include "light.h"

// shadow test creates a ray from the intersection point in illuminate to
// the light location, if any object (not including the object the intersection point is on)
// intersects the ray the function returns 1 for true, if there is no intersection
// it returns 0 for false

static int shadow_test(VP_T light_loc, VP_T int_pt, SCENE_T scene, OBJ_T *obj) {
    RAY_T shadow_ray;
    shadow_ray.origin = int_pt;
    shadow_ray.dir = vp_subtract(light_loc, int_pt);
    shadow_ray.dir = normalize(&shadow_ray.dir);
    
    int res;
    double t;
    VP_T unused_int_pt;
    VP_T unused_normal;    

    for(OBJ_T *cur = scene.objs; cur != NULL; cur = cur->next) {
        res = cur->intersect(shadow_ray, cur, &t, &unused_int_pt, &unused_normal);
        if (cur != obj) {
            if (res == 1 && t > 0) { 
                // must have t > 0 here otherwise it will cast shadows on objects when there is an obj "behind" them
                return 1;
            }
        }
    }
    return 0;
}

// illuminate colors a certain pixel if a ray intersects the sphere
// it is mainly colored from the diffuse section adding color by comparing
// normal vector directions and the light location vector
// specular is responsible for the bright light in the middle
// note* that diffuse and specular are only added if they are not in a shadow

COLOR_T illuminate(OBJ_T *obj, VP_T int_pt, VP_T normal, RAY_T ray, SCENE_T scene) {
    COLOR_T to_return;
    VP_T light_loc = scene.light.light_loc;

    COLOR_T obj_color = obj->color;
    if (obj->checker && 
        ((int)floor(int_pt.x + 0.0001) +
         (int)floor(int_pt.y + 0.0001) +
         (int)floor(int_pt.z + 0.0001) & 1)) {
            obj_color = obj->color2;
        }
    // ambient
    to_return.R = 0.1 * obj_color.R;
    to_return.B = 0.1 * obj_color.B;
    to_return.G = 0.1 * obj_color.G;

    // light attenuation
    double dist = distance(int_pt, light_loc);
    double atten = (1/(.002 * (dist * dist) + 0.02 * dist + 0.2));

    int result;
    result = shadow_test(light_loc, int_pt, scene, obj);
    if (result == 0){
        // diffuse += adding on color
        VP_T l;
        l.x = light_loc.x - int_pt.x;
        l.y = light_loc.y - int_pt.y;
        l.z = light_loc.z - int_pt.z;
        l = normalize(&l);
        double dp;
        dp = dot_product(l, normal);
        if (dp > 0) {
            to_return.R += dp * obj_color.R * atten;
            to_return.G += dp * obj_color.G * atten;
            to_return.B += dp * obj_color.B * atten;
        }

        // specular += adding on color
        if (dp > 0) {
            VP_T r;
            r.x = l.x - normal.x * 2 * dp;
            r.y = l.y - normal.y * 2 * dp;
            r.z = l.z - normal.z * 2 * dp;
            r = normalize(&r);
            double dp2;
            dp2 = dot_product(r, ray.dir);
            if (dp2 > 0) {
                to_return.R += pow(dp2, 200) * atten;
                to_return.B += pow(dp2, 200) * atten;
                to_return.G += pow(dp2, 200) * atten;
            }
        }
    }

    // these check to see if a value is too bright to avoid overflow errors
    if (to_return.R > 1) {
        to_return.R = 1;
    }
    if (to_return.G > 1) {
        to_return.G = 1;
    }
    if (to_return.B > 1) {
        to_return.B = 1;
    }
    // fprintf(stderr, "illuminate to_return.R: %f\n", to_return.R);
    
    return to_return;
}
