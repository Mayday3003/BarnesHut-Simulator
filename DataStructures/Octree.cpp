#include <vector>
#include <memory>
#include <iostream>

struct Vec3 {
    double x, y, z;

    Vec3(double x=0, double y=0, double z=0) : x(x), y(y), z(z) {}

    bool operator<=(const Vec3& other) const {
        return x <= other.x && y <= other.y && z <= other.z;
    }

    bool operator>=(const Vec3& other) const {
        return x >= other.x && y >= other.y && z >= other.z;
    }
};

struct Point {
    Vec3 position;
    // Add any data here, like mass or id
    Point(const Vec3& pos) : position(pos) {}
};

class OctreeNode {
public:
    Vec3 center;
    double halfSize;
    std::vector<Point> points;
    std::unique_ptr<OctreeNode> children[8];

    static constexpr int MAX_POINTS = 1;

    OctreeNode(const Vec3& center, double halfSize)
        : center(center), halfSize(halfSize) {}

    bool contains(const Vec3& point) const {
        Vec3 min = {center.x - halfSize, center.y - halfSize, center.z - halfSize};
        Vec3 max = {center.x + halfSize, center.y + halfSize, center.z + halfSize};
        return point >= min && point <= max;
    }

    void insert(const Point& p) {
        if (!contains(p.position)) return;

        if (points.size() < MAX_POINTS && !children[0]) {
            points.push_back(p);
        } else {
            if (!children[0]) subdivide();
            for (auto& child : children) {
                child->insert(p);
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

        // Reinsert existing points into children
        for (const auto& p : points) {
            for (auto& child : children) {
                if (child->contains(p.position)) {
                    child->insert(p);
                    break;
                }
            }
        }
        points.clear(); // clear local points after pushing to children
    }


    

    void printTree(int depth = 0) const {
        // Indentación visual
        std::string indent(depth * 2, ' ');

        // Mostrar información del nodo actual
        std::cout << indent << "Nodo (Centro: " 
                << center.x << ", " << center.y << ", " << center.z 
                << ", halfSize: " << halfSize << ")"
                << " con " << points.size() << " puntos" << std::endl;

        // Recorrer hijos si existen
        for (int i = 0; i < 8; ++i) {
            if (children[i]) {
                std::cout << indent << "|____ Hijo " << i << ":" << std::endl;
                children[i]->printTree(depth + 1);
            }
        }
    }
};



int main() {
    OctreeNode root({0, 0, 0}, 2.0);
    root.insert(Point({0.5, 0.5, 0.5}));
    root.insert(Point({0.6, 0.6, 0.6}));
    
    root.printTree();
    
    return 0;
}
