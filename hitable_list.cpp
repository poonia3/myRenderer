#ifndef _HITABLELIST_
#define _HITABLELIST_

#include "hitable.cpp"

class hitable_list: public hitable{
public:
    hitable **list;
    int size;
    hitable_list(){}
    hitable_list(hitable **l, int n){list = l; size = n;}
    virtual bool hit(const ray& incident_ray, float min_dist, float max_dist, hit_record& record)const;
};

bool hitable_list::hit(const ray& incident_ray, float min_dist, float max_dist, hit_record& record)const{
    hit_record temp;
    bool hit_something = false;
    float closest_interaction = max_dist;
    for(int i = 0; i < size; i++){
        if(list[i]->hit(incident_ray, min_dist, closest_interaction, temp)){
            hit_something = true;
            closest_interaction = temp.distance;
            record = temp;
        }
    }
    return hit_something;
}

class light_list{
public:
    light_sources *list;
    int size;
    light_list(){}
    light_list(light_sources* l, int n):list(l), size(n){}
    inline light_sources& operator[](int i){return list[i];}
    inline light_sources operator[](int i)const{return list[i];}
    inline void delete_list(){delete[] list;}
};

#endif