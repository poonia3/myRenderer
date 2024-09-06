#ifndef  _CAMERA_
#define _CAMERA_
#define M_pi 3.141592653589793

#include "ray.cpp"
#include "hitable.cpp"


class camera{
public:
    vec3 lower_left_corner, horizontal, vertical, origin;
    vec3 u, v, w;
    float lens_radius;
    camera(){
        lower_left_corner = vec3(-2.0, -1.0, -3.0);
        horizontal = vec3(4.0, 0.0, 0.0);
        vertical = vec3(0.0, 4.0, 0.0);
        origin = vec3(0.0, 0.0, 0.0);
    }
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperature, float focus_dist){
            lens_radius = aperature/2;
            float theta = vfov*M_pi/180;
            float half_height = tan(theta/2);
            float half_width = aspect * half_height;
            origin = lookat;
            w = (lookfrom - lookat).normalised();
            u = (vup^w).normalised();
            v = (w ^ u).normalised();
            lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
            horizontal = 2*half_width*u*focus_dist;
            vertical = 2*half_height*v*focus_dist;
        }

    
    camera(vec3 v1, vec3 h, vec3 v, vec3 o){
        lower_left_corner = v1;
        horizontal = h;
        vertical = v;
        origin = o;
    }
    ray getRay(float x, float y){
        vec3 rd = lens_radius*random_in_unit_sphere();
            vec3 offset = u * rd.x + v * rd.y;
            offset = vec3(0,0,0);
            return ray(origin + offset,lower_left_corner + x*horizontal + y*vertical- origin - offset);
    }
};

#endif