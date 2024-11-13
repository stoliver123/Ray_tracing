#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>

#include "vp.h"
#include "rt.h"
#include "sphere.h"
#include "light.h"
#include "plane.h"

#define FILE_INPUT "creative_scene.txt"
#define FILE_OUTPUT "creative_scene.ppm"

// trace is an intermediate function betweeen intersect functions and illuminate
// it takes the scene and ray as arguments, and checks to see which object in the
// scene interesects the ray closest to the viewpoint

static COLOR_T trace(RAY_T ray,  SCENE_T scene) {

    int res;
    int closest_res = 0;
    double closest_t = DBL_MAX;
    VP_T closest_int_pt;
    VP_T closest_normal;
    
    double t;
    VP_T int_pt;
    VP_T normal;

    VP_T light_loc = scene.light.light_loc;

    COLOR_T to_print;

    COLOR_T background;
    background.R = 0.3;
    background.G = 0.3;
    background.B = 0.5;

    OBJ_T *cur = scene.objs;
    OBJ_T *closest_obj = scene.objs;
    for(cur = scene.objs; cur != NULL; cur = cur->next) {
        res = cur->intersect(ray, cur, &t, &int_pt, &normal);
        if (t < closest_t && res == 1) {
            closest_t = t;
            closest_int_pt = int_pt;
            closest_normal = normal;
            closest_obj = cur;
            closest_res = res;
        }        
    }
    if (closest_res == 1) {
        to_print = illuminate(closest_obj, closest_int_pt, closest_normal, ray, scene);
    } else {
         to_print = background;
    }
    return to_print;
}

// init reads in the file, scans the text and reads it into the objs in 
// the scene

static void
init(char* filepath, SCENE_T *scene) {
    FILE *fp;
    fp = fopen(filepath, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file, %s\n", filepath);
        return;
    }
    char c;
    while (fscanf(fp, "%c", &c) != EOF) {
        
        if (c == 's') {
            OBJ_T *node = (OBJ_T *)malloc(sizeof(OBJ_T));
            SPHERE_T sphere;

            fscanf(fp, "%lf %lf %lf", &sphere.center.x, &sphere.center.y, &sphere.center.z);
            fscanf(fp, "%lf", &sphere.radius);
            fscanf(fp, "%lf %lf %lf", &node->color.R, &node->color.G, &node->color.B);

            node->checker = 0;
            node->intersect = intersect_sphere;
            node->type = 's';
            node->sphere = sphere;

            if (scene->objs == NULL) { /* add to linked list */
                node->next = NULL;
            } else {
                node->next = scene->objs;                
            }
            scene->objs = node;
        } 

        else if (c == 'p') {
            OBJ_T *node = (OBJ_T *)malloc(sizeof(OBJ_T));
            PLANE_T plane;
            
            fscanf(fp, "%lf %lf %lf", &plane.normal.x, &plane.normal.y, &plane.normal.z);
            fscanf(fp, "%lf", &plane.distance);
            fscanf(fp, "%lf %lf %lf", &node->color.R, &node->color.G, &node->color.B);
            fscanf(fp, "%lf %lf %lf", &node->color2.R, &node->color2.G, &node->color2.B);

            if (node->color.R == node->color2.R && 
                node->color.G == node->color2.G && 
                node->color.B == node->color2.B) {
                node->checker = 0;
            } else {
                node->checker = 1;
            }

            node->intersect = intersect_plane;
            node->type = 'p';
            node->plane = plane;

            if (scene->objs == NULL) { /* add to linked list */
                node->next = NULL;
            } else {
                node->next = scene->objs;                
            }
            scene->objs = node;
        }

        else if (c == 'l') {
            fscanf(fp, "%lf %lf %lf",
            &scene->light.light_loc.x, &scene->light.light_loc.y, &scene->light.light_loc.z);
        }
    }
}

// sets resolution, creates/opens the output file, creates all of the rays 
// sets each pixel color and writes them to the output file
int main() {
    double x_resolution =  640;
    double y_resolution = 480;
    double aspect_ratio = (double)x_resolution / (double)y_resolution;
    
    RAY_T ray; 
    ray.origin.x = 0;
    ray.origin.y = 0;
    ray.origin.z = 0;

    SCENE_T scene;
    scene.start_x = (-1 * ((aspect_ratio)/2.0));
    scene.start_y = 0.5;
    scene.pixel_size = (1.0/y_resolution);   

    init(FILE_INPUT, &scene);
    
    FILE *fp;
    fp = fopen(FILE_OUTPUT, "w");

    fprintf(fp, "P6\n%d %d\n255\n", (int)x_resolution, (int)y_resolution);
    
    for(double y = 0; y < y_resolution; y++) {
        for (double x = 0; x < x_resolution; x++) { 
            // note, if we put <= here we get an off by one error and the whole image is distorted

            ray.dir.x = scene.start_x + (x * scene.pixel_size);
            ray.dir.y = scene.start_y - (y * scene.pixel_size);
            ray.dir.z = 1; // note, must reset z = 1 since we are normalizing every iteration!
            ray.dir = normalize(&ray.dir);

            COLOR_T pixel_color;

            pixel_color = trace(ray, scene);
            fprintf(fp, "%c%c%c",(unsigned char)(pixel_color.R * 255),
                                 (unsigned char)(pixel_color.G * 255),
                                 (unsigned char)(pixel_color.B * 255));
        }
    }   
    return 0;
}
// any function not used outside source file, 
// do eiffel towel for creative thing