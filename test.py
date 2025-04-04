import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection

# Representación de un nodo del Octree
class OctreeNode:
    def __init__(self, center, half_size, points=0, children=None):
        self.center = center
        self.half_size = half_size
        self.points = points
        self.children = children if children else []

def draw_cube(ax, center, half_size, color='blue', alpha=0.1):
    x, y, z = center
    hs = half_size
    # Define los 8 vértices del cubo
    vertices = [
        [x - hs, y - hs, z - hs],
        [x + hs, y - hs, z - hs],
        [x + hs, y + hs, z - hs],
        [x - hs, y + hs, z - hs],
        [x - hs, y - hs, z + hs],
        [x + hs, y - hs, z + hs],
        [x + hs, y + hs, z + hs],
        [x - hs, y + hs, z + hs]
    ]
    # Define las 6 caras del cubo
    faces = [
        [vertices[0], vertices[1], vertices[2], vertices[3]],
        [vertices[4], vertices[5], vertices[6], vertices[7]],
        [vertices[0], vertices[1], vertices[5], vertices[4]],
        [vertices[2], vertices[3], vertices[7], vertices[6]],
        [vertices[1], vertices[2], vertices[6], vertices[5]],
        [vertices[4], vertices[7], vertices[3], vertices[0]]
    ]
    ax.add_collection3d(Poly3DCollection(faces, facecolors=color, linewidths=0.5, edgecolors='black', alpha=alpha))

def plot_octree(ax, node):
    draw_cube(ax, node.center, node.half_size, color='cyan' if node.points else 'gray', alpha=0.2 if node.points else 0.05)
    for child in node.children:
        plot_octree(ax, child)

# Construcción manual del árbol según el texto
root = OctreeNode((0, 0, 0), 2)
root.children = [OctreeNode((-1, -1, -1), 1),
                 OctreeNode((1, -1, -1), 1),
                 OctreeNode((-1, 1, -1), 1),
                 OctreeNode((1, 1, -1), 1),
                 OctreeNode((-1, -1, 1), 1),
                 OctreeNode((1, -1, 1), 1),
                 OctreeNode((-1, 1, 1), 1),
                 OctreeNode((1, 1, 1), 1)]

child7 = root.children[7]
child7.children = [
    OctreeNode((0.5, 0.5, 0.5), 0.5, children=[
        OctreeNode((0.25, 0.25, 0.25), 0.25, points=1),
        OctreeNode((0.75, 0.25, 0.25), 0.25),
        OctreeNode((0.25, 0.75, 0.25), 0.25),
        OctreeNode((0.75, 0.75, 0.25), 0.25),
        OctreeNode((0.25, 0.25, 0.75), 0.25),
        OctreeNode((0.75, 0.25, 0.75), 0.25),
        OctreeNode((0.25, 0.75, 0.75), 0.25),
        OctreeNode((0.75, 0.75, 0.75), 0.25, points=1)
    ])
]

child7.children += [
    OctreeNode((1.5, 0.5, 0.5), 0.5),
    OctreeNode((0.5, 1.5, 0.5), 0.5),
    OctreeNode((1.5, 1.5, 0.5), 0.5),
    OctreeNode((0.5, 0.5, 1.5), 0.5),
    OctreeNode((1.5, 0.5, 1.5), 0.5),
    OctreeNode((0.5, 1.5, 1.5), 0.5),
    OctreeNode((1.5, 1.5, 1.5), 0.5)
]

# Crear gráfico
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')
plot_octree(ax, root)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title("Visualización del Octree en 3D")
plt.tight_layout()
plt.show()
