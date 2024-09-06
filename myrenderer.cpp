#include "vec3.cpp"
#include "ray.cpp"
#include "hitable.cpp"
#include "hitable_list.cpp"
#include "material.cpp"
#include "camera.cpp"
#include <fstream>
#include <string>

std::ofstream point_file;

vec3 check_intensity(const vec3& point, hitable *world, light_list lights, int depth);
vec3 color(const ray& r, hitable *world, light_list lights, int depth);

int main(){
    float width = 500, height = 500, average = 10;
    
    vec3 lookfrom = vec3(-4,0.5,5), lookat = vec3(0,0,-3);
    int j = 0;
    hitable *list[4];
    //                        Coordinates       //Radius    //Material
    list[0] = new sphere(vec3(  0,      0,-3)  , 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));  //The Blue sphere
    list[1] = new sphere(vec3(  0, -100.5,-3)  , 100, new lambertian(vec3(0.4, 0.8, 0.8)));  //The Big sphere
    list[2] = new sphere(vec3(  1,      0,-3)  , 0.5, new      metal(vec3(0.8, 0.6, 0.2)));  //The Yellow Sphere
    list[3] = new sphere(vec3( -1,      0,-3)  , 0.5, new dielectric(1.5));                  //The Glass sphere
    hitable *world = new hitable_list(list,4);

    light_sources* LightList;
    LightList = new light_sources[2];
    LightList[0] = light_sources(vec3( -10, 10, -10), vec3(1,1,1), 150);
    LightList[1] = light_sources(vec3(  15,  5,  15), vec3(1,1,1), 150);

    light_list lights(LightList, 2);

    float i = 0.9;
        j++;
        std::ofstream file("output"+std::to_string(j)+".ppm", std::ios::binary);
        std::cout << "output/output"+std::to_string(j)+".ppm\n";
        file << "P6\n" << width << " " << height << "\n255\n";
        //            Location,  // Look at            // Vertical // Field of view,    //   Aspect Ratio         //Aperature //Focal Length
        camera Camera(lookfrom, 0.5*(lookfrom+lookat), vec3(0,1,0),              50, float(width)/float(height),   3.0      ,i*(lookat-lookfrom).length() );

        ray r(vec3(0,0,0), vec3(0,1,0));
        for(int i = height; i > 0; i--){
            for(int j = 0; j < width; j++){
                vec3 col(0,0,0);
                for(int avg = 0; avg < average; avg++){
                    float y = float(i+random())/float(height);
                    float x = float(j+random())/float(width);
                    r = Camera.getRay(x, y);
                    col = col + color(r, world, lights, 0);
                }
                col = col / average;

                file << char(col.r()*255) << char(col.g()*255) << char(col.b()*255);
            }
        }
        file.close();
    //}
}

vec3 color(const ray& r, hitable *world, light_list lights, int depth) {
    hit_record rec;
    if(depth > 5){
        vec3 unit_direction = r.direction.normalised();
        float t = 0.5*(unit_direction.y + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
    if(world->hit(r, 0.01, 10000, rec)) {
        ray_list scattered;
        vec3 attenuation;
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
            vec3 col(0,0,0);
            vec3 lightIntensity = check_intensity(rec.point_of_contact, world, lights, 0);
            for(int i = 0; i < scattered.size; i++)
                col = col + scattered.weights[i] * product(attenuation, color(scattered[i], world, lights, depth+1));
            scattered.delete_list();
            return col;
        }
        else {
            return vec3(0,0,0);
        }
    }
    else{
        vec3 unit_direction = r.direction.normalised();
        float t = 0.5*(unit_direction.y + 1.0);
        return t*vec3(0.5, 0.7, 1.0);
    }    
}

vec3 check_intensity(const vec3& point, hitable *world, light_list lights, int depth){
    vec3 intensity(0,0,0);
    hit_record rec;

    if(depth < 2){for(int i = 0; i < lights.size; i++){
        ray shadow_ray = ray(point, lights[i].centre - point);
        if(world->hit(shadow_ray, 0.01, 1000, rec)){
            if(!rec.mat_ptr->opaque){
                ray_list scattered;
                vec3 attenuation;
                if(rec.mat_ptr->scatter(shadow_ray, rec, attenuation, scattered)){
                    if(scattered.weights[1] != 0){
                        //std::cout << intensity << ":i ";
                        if(world->hit(scattered[1], 0.01, 1000, rec)){
                            intensity = intensity + scattered.weights[1]  * check_intensity(rec.point_of_contact, world, lights, 1+depth);
                        }
                        else{
                            intensity = intensity + scattered.weights[1]  * check_intensity(rec.point_of_contact, world, lights, 1+depth);
                        }
                    }
                }
                scattered.delete_list();
            }
        }
        else{
            float distance = lights[i].distanceToLight(point);
            intensity = intensity + (lights[i].colour * lights[i].max_intensity) / (distance*distance);
        }
    }}
    return intensity;
}