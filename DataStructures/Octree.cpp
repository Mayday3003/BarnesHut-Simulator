#include <vector>
#include <memory>
#include <iostream>
#include <atomic>
#include <cmath>
#include "../include/Particle.h"

struct Vec3 {
    double x, y, z;

    Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    bool operator<=(const Vec3& other) const {
        return x <= other.x && y <= other.y && z <= other.z;
    }

    bool operator>=(const Vec3& other) const {
        return x >= other.x && y >= other.y && z >= other.z;
    }
};

class OctreeNode {
public:
    Vec3 center;
    double halfSize;
    std::vector<Particle> particles;
    std::unique_ptr<OctreeNode> children[8];
    double totalMass = 0.0;

    OctreeNode(const Vec3& center, double halfSize)
        : center(center), halfSize(halfSize) {}

    bool contains(const Particle& particle) const {
        Vec3 min = {center.x - halfSize, center.y - halfSize, center.z - halfSize};
        Vec3 max = {center.x + halfSize, center.y + halfSize, center.z + halfSize};
        return Vec3{particle.pos.x, particle.pos.y, particle.pos.z} >= min &&
               Vec3{particle.pos.x, particle.pos.y, particle.pos.z} <= max;
    }

    void insert(const Particle& particle) {
        if (!contains(particle)) return;

        if (particles.size() < 8 && !children[0]) {
            particles.push_back(particle);
        } else {
            if (!children[0]) subdivide();
            for (auto& child : children) {
                if (child->contains(particle)) {
                    child->insert(particle);
                    break;
                }
            }
        }
    }

    void subdivide() {
        double quarter = halfSize / 2.0;
        for (int i = 0; i < 8; ++i) {
            double dx = (i & 1) ? quarter : -quarter;
            double dy = (i & 2) ? quarter : -quarter;
            double dz = (i & 4) ? quarter : -quarter;
            Vec3 newCenter = {center.x + dx, center.y + dy, center.z + dz};
            children[i] = std::make_unique<OctreeNode>(newCenter, quarter);
        }

        for (const auto& particle : particles) {
            for (auto& child : children) {
                if (child->contains(particle)) {
                    child->insert(particle);
                    break;
                }
            }
        }
        particles.clear();
    }

    void NodeMass() {
        totalMass = 0.0;
        if (children[0]) {
            for (const auto& child : children) {
                if (child) {
                    child->NodeMass();
                    totalMass += child->totalMass;
                }
            }
        } else {
            for (const auto& particle : particles) {
                totalMass += particle.mass;
            }
        }
    }
};

void calculate_force(const OctreeNode* node, Particle& particle, double theta) {
    if (!node || node->totalMass == 0) return;

    Vec3 diff = {
        node->center.x - particle.x,
        node->center.y - particle.y,
        node->center.z - particle.z
    };
    double distance = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

    // Si el nodo es una hoja o cumple el criterio de apertura
    if (node->halfSize / distance < theta || !node->children[0]) {
        double force = (node->totalMass) / (distance * distance * distance + 1e-9); // Evitar división por cero
        particle.ax += force * diff.x;
        particle.ay += force * diff.y;
        particle.az += force * diff.z;
    } else {
        // Recorrer los hijos
        for (const auto& child : node->children) {
            calculate_force(child.get(), particle, theta);
        }
    }
}
