#include <vector>
#include <memory>
#include <iostream>
#include <atomic>
#include <cmath>
#include "../include/Particle.h"
#include "../include/Octree.h"

OctreeNode::OctreeNode(const Vector3& center, double halfSize)
    : center(center), halfSize(halfSize) {}

bool OctreeNode::contains(const Vector3& point) const {
    Vector3 min = {center.x - halfSize, center.y - halfSize, center.z - halfSize};
    Vector3 max = {center.x + halfSize, center.y + halfSize, center.z + halfSize};
    return point.x >= min.x && point.y >= min.y && point.z >= min.z &&
           point.x <= max.x && point.y <= max.y && point.z <= max.z;
}

void OctreeNode::insert(const Particle& particle) {
    if (!contains(particle.pos)) return;

    if (particles.size() < 8 && !children[0]) {
        particles.push_back(particle);
    } else {
        if (!children[0]) subdivide();
        for (auto& child : children) {
            if (child->contains(particle.pos)) {
                child->insert(particle);
                break;
            }
        }
    }
}

void OctreeNode::subdivide() {
    double quarter = halfSize / 2.0;
    for (int i = 0; i < 8; ++i) {
        double dx = (i & 1) ? quarter : -quarter;
        double dy = (i & 2) ? quarter : -quarter;
        double dz = (i & 4) ? quarter : -quarter;
        Vector3 newCenter = {center.x + dx, center.y + dy, center.z + dz};
        children[i] = std::unique_ptr<OctreeNode>(new OctreeNode(newCenter, quarter));
    }

    for (const auto& particle : particles) {
        for (auto& child : children) {
            if (child->contains(particle.pos)) {
                child->insert(particle);
                break;
            }
        }
    }
    particles.clear();
}

void OctreeNode::NodeMass() {
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

void calculate_force(const OctreeNode* node, Particle& particle, double theta) {
    if (!node || node->totalMass == 0) return;

    Vector3 diff = {
        node->center.x - particle.pos.x,
        node->center.y - particle.pos.y,
        node->center.z - particle.pos.z
    };
    double distance = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

    if (distance == 0) return; // Evitar división por cero

    // Si el nodo es una hoja o cumple el criterio de apertura
    if (node->halfSize / distance < theta || !node->children[0]) {
        double force = (node->totalMass) / (distance * distance * distance + 1e-9);
        particle.acc.x += force * diff.x;
        particle.acc.y += force * diff.y;
        particle.acc.z += force * diff.z;
    } else {
        // Recorrer los hijos
        for (const auto& child : node->children) {
            calculate_force(child.get(), particle, theta);
        }
    }
}