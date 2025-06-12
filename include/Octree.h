#ifndef OCTREE_H
#define OCTREE_H

#include "Particle.h"
#include <vector>
#include <memory>
#include <atomic>


class OctreeNode {
public:
    Vector3 center;
    double halfSize;
    std::vector<Particle> particles;
    std::unique_ptr<OctreeNode> children[8]; 
    double totalMass = 0.0;
    int id = -1; 
    static constexpr int MAX_POINTS = 8; 
    static std::atomic<int> id_counter;

    OctreeNode(const Vector3& center, double halfSize);

    bool contains(const Vector3& point) const;
    void insert(const Particle& particle);
    void subdivide(); 
    void NodeMass(); 
    void printTree(int depth = 0) const; 
};

void calculate_force(const OctreeNode* node, Particle& particle, double theta);

#endif