#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "vp.h"


// this function returns a length by squaring the x, y, and z components and taking the square root.
static double v_length(VP_T *vector_point) {
    double length;
    double to_square;

    to_square = (vector_point->x * vector_point->x) +
                (vector_point->y * vector_point->y) +
                (vector_point->z * vector_point->z);
    
    if (to_square < 0) {
        fprintf(stderr, "trying to sqrt a negative! to_square is: %f\n", to_square);
        exit(1);
    }
    length = sqrt(to_square);
    return length;
}

// normalizes a vector point by dividing x, y, and z by the length
VP_T normalize(VP_T *vector_point) {
    VP_T normalized;

    double length = v_length(vector_point);
    
    normalized.x = vector_point->x/length;
    normalized.y = vector_point->y/length;
    normalized.z = vector_point->z/length;

    return normalized;
}

// takes the dot product of two vectors
double dot_product(VP_T v1, VP_T v2) {
    double dp;
    dp = ((v1.x * v2.x) +
          (v1.y * v2.y) +
          (v1.z * v2.z));
    return dp;
}

// subtracts the second vector point from the first
VP_T vp_subtract(VP_T v1, VP_T v2) {
    VP_T to_return;
    to_return.x = v1.x - v2.x;
    to_return.y = v1.y - v2.y;
    to_return.z = v1.z - v2.z;
    return to_return;
}

// find the euclidean distance between two vector points
double distance(VP_T v1, VP_T v2) {
    double to_return;
    double comp_x = v2.x - v1.x;
    double comp_y = v2.y - v1.y;
    double comp_z = v2.z - v1.z;

    comp_x = comp_x * comp_x;
    comp_y = comp_y * comp_y;
    comp_z = comp_z * comp_z;

    double sum = comp_x + comp_y + comp_z;

    if (sum < 0) {
        fprintf(stderr, "trying to sqrt a negative! to_square is: %f\n", sum);
        exit(1);
    }
    to_return = sqrt(sum);

    return to_return;
}