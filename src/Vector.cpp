#include "Vector.h"

Vector::Vector(float x, float y): x(x), y(y) {
    length = sqrt(x * x + y * y);
}

Vector::Vector(const Vector&other): x(other.x), y(other.y), length(other.length) {}

float Vector::getX() const {
    return x;
}

float Vector::getY() const {
    return y;
}

float Vector::getLength() const {
    return length;
}

void Vector::normalize() {
    x /= length;
    y /= length;
    length = 1;
}

Vector Vector::getProjection(const Vector& other) {
    return (*this) * other / other.length * other * (1 / other.length);
}

void Vector::invertX() {
    x = -x; // length does not change
}
void Vector::invertY() {
    y = -y; // length does not change
}

Vector& operator += (Vector& v1, const Vector& v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    v1.length = sqrt(v1.x * v1.x + v1.y * v1.y);
    return v1;
}

Vector& operator -= (Vector& v1, const Vector& v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.length = sqrt(v1.x * v1.x + v1.y * v1.y);
    return v1;
}

Vector operator + (const Vector& v1, const Vector& v2) {
    Vector result(v1);
    result += v2;
    return result;
}

Vector operator - (const Vector& v1, const Vector& v2) {
    Vector result(v1);
    result -= v2;
    return result;
}

Vector& operator *= (Vector& vector, const float coef) {
    vector.x *= coef;
    vector.y *= coef;
    vector.length = sqrt(vector.x * vector.x + vector.y * vector.y);
    return vector;
}

Vector operator * (const Vector& vector, const float coef) {
    Vector result(vector);
    result *= coef;
    return result;
}

Vector& operator /= (Vector& vector, const float coef) {
    vector.x /= coef;
    vector.y /= coef;
    vector.length = sqrt(vector.x * vector.x + vector.y * vector.y);
    return vector;
}

Vector operator / (const Vector& vector, const float coef) {
    Vector result(vector);
    result /= coef;
    return result;
}

Vector operator * (const float coef, const Vector& vector) {
    Vector result(vector);
    result *= coef;
    return result;
}

float operator * (const Vector& v1, const Vector& v2) {
    float result = v1.x * v2.x + v1.y * v2.y;
    return result;
}

float crossProduct (const Vector& v1, const Vector& v2) {
    float result = v1.x * v2.y - v1.y * v2.x;
    return result;
}

void Vector::elByElMult(const Vector& v2) {
    x *= v2.x;
    y *= v2.y;
}

float dist_quad(const Vector& p1, const Vector& p2) {
    return (p1.getX() - p2.getX()) * (p1.getX() - p2.getX()) + (p1.getY() - p2.getY()) * (p1.getY() - p2.getY());
}