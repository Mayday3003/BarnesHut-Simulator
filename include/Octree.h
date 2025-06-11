#ifndef OCTREE_H
#define OCTREE_H

#include "Particle.h"
#include <vector>
#include <memory>
#include <atomic>

struct Vec3 {
    double x, y, z;

    Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    bool operator<=(const Vec3& other) const;
    bool operator>=(const Vec3& other) const;
};

struct Point {
    Vec3 position;
    double mass;

    Point(const Vec3& pos, double mass = 1.0);
};

class OctreeNode {
public:
    Vec3 center;
    double halfSize;
    std::vector<Point> points;
    std::unique_ptr<OctreeNode> children[8];
    double totalMass = 0.0;
    int id = -1;

    static constexpr int MAX_POINTS = 8;
    static std::atomic<int> id_counter;

    OctreeNode(const Vec3& center, double halfSize);

    bool contains(const Vec3& point) const;
    void insert(const Point& p);
    void subdivide();
    void NodeMass();
    void printTree(int depth = 0) const;
};

void calculate_force(const OctreeNode* node, Particle& particle, double theta);

#endif // OCTREE_H
