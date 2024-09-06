#ifndef _VECTOR3D_
#define _VECTOR3D_
#include <iostream>
#include <cmath>

class vec3{
public:
    float x, y, z;
    vec3(){x=0;y=0;z=0;}
    vec3(const vec3& v){x=v.x; y=v.y; z=v.z;}
    vec3(float x0, float y0, float z0){ x = x0; y = y0; z = z0; }
    inline float r(){return (x>1)?1:x;}
    inline float g(){return (y>1)?1:y;}
    inline float b(){return (z>1)?1:z;}

    inline vec3 operator+(){return *this;};
    inline vec3 operator+()const{return *this;};
    inline vec3 operator-(){return vec3(-x, -y, -z);};
    inline vec3 operator-()const{return vec3(-x, -y, -z);};
    inline float operator[](int i) const {return (!i)?x:((i==1)?y:z);}
    inline float& operator[](int i){return (!i)?x:((i==1)?y:z);}

    inline vec3 operator+(const vec3& vector2){return vec3(x+vector2.x, y+vector2.y, z+vector2.z);}
    inline vec3 operator+(const vec3& vector2)const{return vec3(x+vector2.x, y+vector2.y, z+vector2.z);}
    inline vec3 operator-(const vec3& vector2){return vec3(x-vector2.x, y-vector2.y, z-vector2.z);}
    inline vec3 operator-(const vec3& vector2)const{return vec3(x-vector2.x, y-vector2.y, z-vector2.z);}
    inline vec3 operator*(const float& scalar){return vec3(scalar*x, scalar*y, scalar*z);}
    inline vec3 operator*(const float& scalar)const{return vec3(scalar*x, scalar*y, scalar*z);}
    inline vec3 operator/(const float& scalar){float inverse = 1/scalar; return vec3(x*inverse, y*inverse, z*inverse);}
    inline vec3 operator/(const float& scalar)const{float inverse = 1/scalar; return vec3(x*inverse, y*inverse, z*inverse);}
    inline friend vec3 operator*(float scalar, const vec3& vector){return vec3(scalar*vector.x, scalar*vector.y, scalar*vector.z);}
    inline float operator*(const vec3& vector2){return (x*vector2.x + y*vector2.y + z*vector2.z);}
    inline float operator*(const vec3& vector2)const{return (x*vector2.x + y*vector2.y + z*vector2.z);}
    inline vec3 operator^(const vec3& v2){return vec3(y*v2.z - z*v2.y, z*v2.x - x*v2.z, x*v2.y - y*v2.x);}
    inline vec3 operator^(const vec3& v2)const{return vec3(y*v2.z - z*v2.y, z*v2.x - x*v2.z, x*v2.y - y*v2.x);}

    inline float length(){return sqrt(x*x + y*y + z*z);}
    inline float length()const{return sqrt(x*x + y*y + z*z);}
    inline float squared_length(){return x*x + y*y + z*z;}
    inline float squared_length()const{return x*x + y*y + z*z;}
    inline vec3 normalised(){
        float not_length = 1/length();
        return vec3(x*not_length, y*not_length, z*not_length);
    }
    inline vec3 normalised()const{
        float not_length = 1/length();
        return vec3(x*not_length, y*not_length, z*not_length);
    }
    inline vec3 make_unit_vector(){
        float not_length = 1/length();
        x *= not_length;
        y *= not_length;
        z *= not_length;
        return *this;
    }
    inline friend std::ostream& operator<<(std::ostream& os, const vec3& vec) {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }
    inline friend std::istream& operator>>(std::istream& is, vec3& vec){
        is >> vec.x >> vec.y >> vec.z;
        return is;
    }
};

inline vec3 product(vec3 v1, vec3 v2){
    return vec3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

#endif