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
    double mass;

    Point(const Vec3& pos, double mass = 1.0) : position(pos), mass(mass) {}
};

class OctreeNode {
public:
    Vec3 center;
    double halfSize;
    std::vector<Point> points;
    std::unique_ptr<OctreeNode> children[8];
    double totalMass = 0.0;

    static constexpr int MAX_POINTS = 8;

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
                if (child->contains(p.position)) {
                    child->insert(p);
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

        // Reinsertar puntos en los hijos
        for (const auto& p : points) {
            for (auto& child : children) {
                if (child->contains(p.position)) {
                    child->insert(p);
                    break;
                }
            }
        }
        points.clear(); // Limpiar puntos del nodo actual
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
            // Sumar la masa real de los puntos del nodo hoja
            for (const auto& p : points) {
                totalMass += p.mass;
            }
        }
    }

    void printTree(int depth = 0) const {
        std::string indent(depth * 2, ' ');

        std::cout << indent << "Nodo (Centro: " 
                << center.x << ", " << center.y << ", " << center.z 
                << ", halfSize: " << halfSize << ")"
                << " con " << points.size() << " puntos"
                << ", masa total: " << totalMass << std::endl;

        for (int i = 0; i < 8; ++i) {
            if (children[i]) {
                std::cout << indent << "|____ Hijo " << i << ":" << std::endl;
                children[i]->printTree(depth + 1);
            }
        }
    }
};


int main() {
    OctreeNode root({0, 0, 0}, 4.0);
    root.insert(Point({0.5, 0.5, 0.5}, 1.0));
    root.insert(Point({0.6, 0.6, 0.6}, 2.0));
    root.insert(Point({-0.3, -0.4, 0.1}, 3.5));
    root.insert(Point({1.5, 1.5, 1.5}, 4.0));
    root.insert(Point({-1.5, -1.5, -1.5}, 5.0));
    root.insert(Point({0.1, 0.1, 0.1}, 6.0));
    root.insert(Point({-3.5, -0.5, -0.5}, 7.0));
    root.insert(Point({2.5, 2.5, 2.5}, 8.0));
    root.insert(Point({-2.5, -2.5, -2.5}, 9.0));
    root.insert(Point({0.2, 0.2, 0.2}, 10.0));
    
    root.NodeMass();  // <--- Importante calcular la masa después de insertar
    root.printTree();

    return 0;
}
