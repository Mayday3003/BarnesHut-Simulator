#include "../include/Particle.h"
#include <cmath>

// Suma de dos vectores
Vector3 vector_add(const Vector3& a, const Vector3& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

// Resta de dos vectores
Vector3 vector_sub(const Vector3& a, const Vector3& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

// Producto escalar de dos vectores
double vector_dot(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Magnitud de un vector
double vector_magnitude(const Vector3& v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Normalización de un vector
Vector3 vector_normalize(const Vector3& v) {
    double mag = vector_magnitude(v);
    if (mag == 0) return {0, 0, 0};
    return {v.x / mag, v.y / mag, v.z / mag};
}
