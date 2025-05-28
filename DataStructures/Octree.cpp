#include <vector>
#include <memory>
#include <iostream>
#include <atomic>

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

// Cualquier duda con la clase del arbol, pregunten a JuanJoBecerra
class OctreeNode {
public:
    Vec3 center;
    double halfSize;
    std::vector<Point> points;
    std::unique_ptr<OctreeNode> children[8];
    double totalMass = 0.0;
    int id = -1; // Unique identifier, -1 means unassigned

    static constexpr int MAX_POINTS = 8; // No cambiar esto a menos que conozcas un 5to espacio dimensional

    static std::atomic<int> id_counter; // Controlador ID's

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
            for (const auto& p : points) {
                totalMass += p.mass;
            }
        }
        // Assign ID if node has mass and hasn't been assigned yet
        if (totalMass > 0 && id == -1) {
            id = id_counter++;
        }
    }

    void printTree(int depth = 0) const {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "Nodo (Centro: "
                  << center.x << ", " << center.y << ", " << center.z
                  << ", halfSize: " << halfSize << ")"
                  << " con " << points.size() << " puntos"
                  << ", masa total: " << totalMass
                  << ", id: " << id << std::endl;
        for (int i = 0; i < 8; ++i) {
            if (children[i]) {
                std::cout << indent << "|____ Hijo " << i << ":" << std::endl;
                children[i]->printTree(depth + 1);
            }
        }
    }
};

std::atomic<int> OctreeNode::id_counter{0};

int main() {
    OctreeNode root({0, 0, 0}, 10.0);
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
    root.insert(Point({5.0, 5.1, -0.1}, 7.1));
    root.insert(Point({-7.2, 3.3, 2.2}, 4.5));
    root.insert(Point({8.8, -6.6, 1.1}, 2.3));
    root.insert(Point({-9.9, 9.9, -9.9}, 5.7));
    root.insert(Point({3.3, -3.3, 3.3}, 1.2));
    root.insert(Point({-2.2, 2.2, -2.2}, 3.8));
    root.insert(Point({7.7, 7.7, 7.7}, 6.6));
    root.insert(Point({-8.8, -8.8, -8.8}, 8.8));
    root.insert(Point({4.4, 0.0, -4.4}, 2.9));
    root.insert(Point({0.0, 4.4, 4.4}, 1.7));
    root.insert(Point({-4.4, 0.0, 4.4}, 3.3));
    root.insert(Point({0.0, -4.4, -4.4}, 4.4));
    root.insert(Point({6.1, -2.3, 1.5}, 2.2));
    root.insert(Point({-1.1, 6.2, -3.3}, 5.5));
    root.insert(Point({2.2, -6.2, 3.3}, 7.7));
    root.insert(Point({-3.3, 1.1, 6.2}, 9.9));
    root.insert(Point({9.9, -1.1, -6.2}, 1.1));
    root.insert(Point({-6.2, 9.9, 1.1}, 2.2));
    root.insert(Point({1.1, -9.9, 6.2}, 3.3));
    root.insert(Point({-1.1, 1.1, -1.1}, 4.4));
    root.insert(Point({2.2, 2.2, 2.2}, 5.5));
    root.insert(Point({-2.2, -2.2, -2.2}, 6.6));
    root.insert(Point({3.3, 3.3, 3.3}, 7.7));
    root.insert(Point({-3.3, -3.3, -3.3}, 8.8));
    root.insert(Point({4.4, 4.4, 4.4}, 9.9));
    root.insert(Point({-4.4, -4.4, -4.4}, 1.0));
    root.insert(Point({5.5, 5.5, 5.5}, 2.0));
    root.insert(Point({-5.5, -5.5, -5.5}, 3.0));
    root.insert(Point({6.6, 6.6, 6.6}, 4.0));
    root.insert(Point({-6.6, -6.6, -6.6}, 5.0));
    
    root.NodeMass();  // <--- Importante calcular la masa después de insertar
    root.printTree();

    return 0;
}
