#ifndef _RAY_
#define _RAY_
#include "vec3.cpp"

class ray{
public:
    vec3 origin, direction;
    ray(){}
    ray(const ray& r){origin = r.origin; direction = r.direction;}
    ray(const vec3& ORIGIN,const vec3& DIRECTION){ origin = ORIGIN; direction = DIRECTION;}
    inline vec3 orig() const {return origin;}
    inline vec3 dir() const {return direction;}
    inline vec3 point_at_perimeter(const float& distance){return origin + direction * distance;}
    inline vec3 point_at_perimeter(const float& distance)const{return origin + direction * distance;}
};

vec3 reflect(const ray& incident_ray, const vec3& normal){
    return incident_ray.direction - 2 * (normal * incident_ray.direction) * normal;
}

bool refract(const ray& incident_ray, const vec3& normal, float ui_over_ut, vec3& refracted_ray){ 
    vec3 incidence = incident_ray.direction;
    float dt = incidence * normal;
    float discriminant = 1.0 - ui_over_ut*ui_over_ut*(1-dt*dt);
    if (discriminant > 0) {
        refracted_ray = ui_over_ut*(incidence - normal*dt) - normal*sqrt(discriminant);
        return true;
    }
    else
        refracted_ray = vec3(1,0,0);
        return false;
}

class ray_list{
public:
    ray *list;
    float *weights;
    int size;
    ray_list(){}
    ray_list(ray* l, int n):list(l), size(n){
        weights = new float[n];
    }
    inline ray& operator[](int i){return list[i];}
    inline ray operator[](int i)const{return list[i];}
    inline void delete_list(){
        delete[] list;
        delete[] weights;
    }
};

#endif