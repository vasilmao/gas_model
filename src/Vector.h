#ifndef INCLUDE_VECTOR_CLASS
#define INCLUDE_VECTOR_CLASS

#include <stdio.h>
#include <math.h>
#include "Tools.h"

class Vector {

private:
    float x = 0;
    float y = 0;
    float length = 0;

public:
    Vector(float x, float y);
    Vector(const Vector& other);

    Vector(){}
    ~Vector(){
        // printf("vector destroyed!\n");
    }

    float getX() const;
    float getY() const;
    float getLength() const;
    void normalize();
    Vector getProjection(const Vector& other);

    void invertX();
    void invertY();

    friend Vector& operator += (Vector& v1,     const Vector& v2);
    friend Vector& operator -= (Vector& v1,     const Vector& v2);
    friend Vector operator + (const Vector& v1, const Vector& v2);
    friend Vector operator - (const Vector& v1, const Vector& v2);
    friend Vector& operator *= (Vector& vector, const float coef);
    friend Vector operator * (const Vector& vector, const float coef);
    friend Vector& operator /= (Vector& vector, const float coef);
    friend Vector operator / (const Vector& vector, const float coef);
    friend Vector operator * (const float coef, const Vector& vector);
    friend float operator * (const Vector& v1, const Vector& v2);
    friend float crossProduct (const Vector& v1, const Vector& v2);
    void elByElMult(const Vector& v2);

};

float dist_quad(const Vector& p1, const Vector& p2);


#endif