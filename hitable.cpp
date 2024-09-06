#ifndef _HITABLE_
#define _HITABLE_

#include "ray.cpp"
#include <cstdlib>

float random(){
    return (float(rand()))/(RAND_MAX+1);
}

vec3 random_in_unit_sphere(float size=0.05){
    vec3 p;
    do{
        p = 2*size*(vec3(random()-1, random()-1, random()-1));
    }while(p.length() >= size);
    return p;
}

class material;

struct hit_record{
    float distance;
    vec3 point_of_contact;
    vec3 normal;
    material *mat_ptr;
};

class hitable{
public:
    virtual bool hit(const ray& incident_ray, float min_dist, float max_dist, hit_record& record) const = 0;
};

class sphere: public hitable{
public:
    vec3 centre;
    float radius;
    sphere(){}
    sphere(vec3 centre, float r, material *m):centre(centre), radius(r), mat_ptr(m){}
    virtual bool hit(const ray& incident_ray, float min_dist, float max_dist, hit_record& record)const;
    material *mat_ptr;
};

bool sphere::hit(const ray& incident_ray, float min_dist, float max_dist, hit_record& record)const{
    vec3 del_centre = incident_ray.origin - centre;
    float a = incident_ray.direction * incident_ray.direction;
    float b = incident_ray.direction * del_centre;
    float c = del_centre * del_centre - radius*radius;
    float discriminant = b*b - a*c;

    if(discriminant < 0) return false; 

    float sqrt_disc = sqrt(discriminant);
    float distance = (- b - sqrt_disc)/a;
    if(distance > min_dist && distance < max_dist){
        record.distance = distance;
        record.point_of_contact = incident_ray.point_at_perimeter(record.distance);
        record.normal = (record.point_of_contact - centre)/radius;
        record.mat_ptr = mat_ptr;
        return true;
    }
    distance = (- b + sqrt_disc)/a;
    if(distance > min_dist && distance < max_dist){
        record.distance = distance;
        record.point_of_contact = incident_ray.point_at_perimeter(record.distance);
        record.normal = (record.point_of_contact - centre)/radius;
        record.mat_ptr = mat_ptr;
        return true;
    }
    return false;
}

class light_sources{
public:
    vec3 centre;
    vec3 colour;
    float max_intensity;
    light_sources(){}
    light_sources(vec3 c, vec3 col, float i):centre(c), colour(col), max_intensity(i){}
    float distanceToLight(vec3 point){
        return (centre-point).length();
    }
};

#endif 