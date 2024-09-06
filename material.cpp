#ifndef _MATERIAL_
#define _MATERIAL_

#include "hitable.cpp"


float schlick(float cosine, float ref_idx) {
    float r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

class material{
public:
    bool opaque = true;
    virtual bool scatter(const ray& incident_ray, const hit_record& record, vec3& attenuation, ray_list& scattered)const = 0;
};

class lambertian:public material{
public:
    vec3 albedo;
    lambertian(const vec3 a): albedo(a){opaque = true;}
    virtual bool scatter(const ray& incident_ray, const hit_record& record, vec3& attenuation, ray_list& scattered)const{
        vec3 target = record.normal + random_in_unit_sphere();
        ray *list = new ray[1];
        list[0] = ray(record.point_of_contact, target);
        scattered = ray_list(list, 1);
        scattered.weights[0] = 1;
        attenuation = albedo;
        return true;
    }
};

class metal: public material{
public:
    vec3 albedo;
    metal(const vec3 a): albedo(a){opaque=true;}
    virtual bool scatter(const ray& incident_ray, const hit_record& record, vec3& attenuation, ray_list& scattered)const{
        vec3 reflected = reflect(incident_ray, record.normal);
        ray *list = new ray[1];
        list[0] = ray(record.point_of_contact, reflected);
        scattered = ray_list(list, 1);
        scattered.weights[0] = 1;
        attenuation = albedo;
        return (scattered[0].direction * record.normal > 0);
    }
};

class dielectric: public material{
public:
    float refractive_index;
    dielectric(float u_medium):refractive_index(u_medium){opaque=false;}
    virtual bool scatter(const ray& incident_ray, const hit_record& record, vec3& attenuation, ray_list& scattered)const{
        vec3 outward_normal;
        vec3 reflected = reflect(incident_ray, record.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflect_prob;
        float cosine;
        if (incident_ray.direction* record.normal > 0) {
                outward_normal = -record.normal;
                ni_over_nt = refractive_index;
                cosine = refractive_index * incident_ray.direction* record.normal/incident_ray.direction.length();
        }
        else {
                outward_normal = record.normal;
                ni_over_nt = 1/refractive_index;
                cosine = -refractive_index * incident_ray.direction* record.normal/incident_ray.direction.length();
        }
        if (refract(incident_ray, outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, refractive_index);
        }
        else {
            reflect_prob = 1.0;
        }
        ray *list = new ray[2];
        list[0] = ray(record.point_of_contact, reflected);
        list[1] = ray(record.point_of_contact, refracted);
        scattered = ray_list(list, 2);
        scattered.weights[0] = reflect_prob;
        scattered.weights[1] = 1-reflect_prob;
        //scattered = ray(record.point_of_contact, reflected);
        //scattered = ray(record.point_of_contact, refracted);
        return true;
    }
};

#endif