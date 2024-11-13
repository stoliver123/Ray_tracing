#ifndef RT_H
#define RT_H

#include <stdio.h>

#include "vp.h"

typedef struct {
    VP_T center;
    double radius;
} SPHERE_T;

typedef struct {
    VP_T origin;
    VP_T dir; // direction should be normalized (i think?)
} RAY_T;

typedef struct {
    VP_T light_loc;
} LIGHT_T;

typedef struct {
    VP_T normal;
    double distance; // distance of plane
} PLANE_T;

typedef struct {
    double R; // red
    double B; // blue
    double G; // green
} COLOR_T;

typedef struct OBJ_T{
    union {
        SPHERE_T sphere;
        PLANE_T plane;
    };
    char type;
    COLOR_T color;
    COLOR_T color2;
    int checker; /* boolean */
    int (*intersect) (RAY_T ray, struct OBJ_T *obj, double *t, VP_T *int_pt, VP_T *normal); /* function pointer */
    struct OBJ_T *next; /* head of linked list */
} OBJ_T;

typedef struct {
    OBJ_T *objs;
    LIGHT_T light;
    double start_x;
    double start_y;
    double pixel_size;
} SCENE_T;

#endif