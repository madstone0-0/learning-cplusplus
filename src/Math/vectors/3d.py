from matplotlib.patches import FancyArrowPatch
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import proj3d
import numpy as np

blue = "C0"
black = "k"
red = "C3"
green = "C2"
purple = "C4"
orange = "C2"
gray = "gray"


## https://stackoverflow.com/a/22867877/1704140
class FancyArrow3D(FancyArrowPatch):
    def __init__(self, xs, ys, zs, *args, **kwargs):
        super().__init__((0, 0), (0, 0), *args, **kwargs)
        self._verts3d = xs, ys, zs

    def do_3d_projection(self, renderer=None):
        xs3d, ys3d, zs3d = self._verts3d
        xs, ys, zs = proj3d.proj_transform(xs3d, ys3d, zs3d, self.axes.M)
        self.set_positions((xs[0], ys[0]), (xs[1], ys[1]))

        return np.min(zs)


class Polygon3D:
    def __init__(self, *vertices, color=blue):
        self.vertices = vertices
        self.color = color


class Points3D:
    def __init__(self, *vectors, color=black):
        self.vectors = list(vectors)
        self.color = color


class Arrow3D:
    def __init__(self, tip, tail=(0, 0, 0), color=red):
        self.tip = tip
        self.tail = tail
        self.color = color


class Segment3D:
    def __init__(self, start_point, end_point, color=blue, linestyle="solid"):
        self.start_point = start_point
        self.end_point = end_point
        self.color = color
        self.linestyle = linestyle


class Box3D:
    def __init__(self, *vector):
        self.vector = vector


def vectorable(v):
    if type(v) == Vector3D:
        yield v
    yield v


# helper function to extract all the vectors from a list of objects
def extract_vectors_3D(objects):
    for object in objects:
        if type(object) == Polygon3D:
            for v in object.vertices:
                yield v
        elif type(object) == Points3D:
            for v in object.vectors:
                vectorable(v)

        elif type(object) == Arrow3D:
            yield object.tip
            yield object.tail
        elif type(object) == Segment3D:
            yield object.start_point
            yield object.end_point
        elif type(object) == Box3D:
            yield object.vector
        else:
            raise TypeError("Unrecognized object: {}".format(object))


def draw3d(
    *objects,
    origin=True,
    axes=True,
    width=6,
    save_as=None,
    azim=None,
    elev=None,
    xlim=None,
    ylim=None,
    zlim=None,
    xticks=None,
    yticks=None,
    zticks=None,
    depthshade=False,
):
    fig = plt.gcf()
    ax = fig.add_subplot(111, projection="3d")
    ax.view_init(elev=elev, azim=azim)

    all_vectors = list(extract_vectors_3D(objects))
    if origin:
        all_vectors.append((0, 0, 0))
    xs, ys, zs = zip(*all_vectors)

    max_x, min_x = max(0, *xs), min(0, *xs)
    max_y, min_y = max(0, *ys), min(0, *ys)
    max_z, min_z = max(0, *zs), min(0, *zs)

    x_size = max_x - min_x
    y_size = max_y - min_y
    z_size = max_z - min_z

    padding_x = 0.05 * x_size if x_size else 1
    padding_y = 0.05 * y_size if y_size else 1
    padding_z = 0.05 * z_size if z_size else 1

    plot_x_range = (min(min_x - padding_x, -2), max(max_x + padding_x, 2))
    plot_y_range = (min(min_y - padding_y, -2), max(max_y + padding_y, 2))
    plot_z_range = (min(min_z - padding_z, -2), max(max_z + padding_z, 2))
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("z")

    def draw_segment(start, end, color=black, linestyle="solid"):
        xs, ys, zs = [[start[i], end[i]] for i in range(0, 3)]
        ax.plot(xs, ys, zs, color=color, linestyle=linestyle)

    if axes:
        draw_segment((plot_x_range[0], 0, 0), (plot_x_range[1], 0, 0))
        draw_segment((0, plot_y_range[0], 0), (0, plot_y_range[1], 0))
        draw_segment((0, 0, plot_z_range[0]), (0, 0, plot_z_range[1]))

    if origin:
        ax.scatter([0], [0], [0], color="k", marker="x")

    for object in objects:
        if type(object) == Points3D:
            xs, ys, zs = zip(*object.vectors)
            # xs, ys, zs = [], [], []
            # for vec in object.vectors[0]:
            #     xs += [vec[0]]
            #     ys += [vec[1]]
            #     zs += [vec[2]]

            ax.scatter(xs, ys, zs, color=object.color, depthshade=depthshade)

        elif type(object) == Polygon3D:
            for i in range(0, len(object.vertices)):
                draw_segment(
                    object.vertices[i],
                    object.vertices[(i + 1) % len(object.vertices)],
                    color=object.color,
                )

        elif type(object) == Arrow3D:
            xs, ys, zs = zip(object.tail, object.tip)
            a = FancyArrow3D(
                xs, ys, zs, mutation_scale=20, arrowstyle="-|>", color=object.color
            )
            ax.add_artist(a)

        elif type(object) == Segment3D:
            draw_segment(
                object.start_point,
                object.end_point,
                color=object.color,
                linestyle=object.linestyle,
            )

        elif type(object) == Box3D:
            x, y, z = object.vector
            kwargs = {"linestyle": "dashed", "color": "gray"}
            draw_segment((0, y, 0), (x, y, 0), **kwargs)
            draw_segment((0, 0, z), (0, y, z), **kwargs)
            draw_segment((0, 0, z), (x, 0, z), **kwargs)
            draw_segment((0, y, 0), (0, y, z), **kwargs)
            draw_segment((x, 0, 0), (x, y, 0), **kwargs)
            draw_segment((x, 0, 0), (x, 0, z), **kwargs)
            draw_segment((0, y, z), (x, y, z), **kwargs)
            draw_segment((x, 0, z), (x, y, z), **kwargs)
            draw_segment((x, y, 0), (x, y, z), **kwargs)
        else:
            raise TypeError("Unrecognized object: {}".format(object))

    if xlim and ylim and zlim:
        plt.xlim(*xlim)
        plt.ylim(*ylim)
        ax.set_zlim(*zlim)
    if xticks and yticks and zticks:
        plt.xticks(xticks)
        plt.yticks(yticks)
        ax.set_zticks(zticks)

    if save_as:
        plt.savefig(save_as)

    plt.show()


class Vector3D:
    def __init__(self, x, y, z) -> None:
        self.x = x
        self.y = y
        self.z = z

    def length(self) -> float:
        return np.sqrt(sum(map(lambda x: x**2, self)))

    def __iter__(self):
        return (i for i in (self.x, self.y, self.z))

    def __repr__(self) -> str:
        className = type(self).__name__
        return f"{className}({self.x:!r}, {self.y:!r}, {self.z:!r})"

    def __str__(self) -> str:
        return str(tuple(self))

    def __add__(self, other):
        try:
            sums = [sum(coords) for coords in zip(self, other)]
            return Vector3D(*sums)
        except TypeError:
            return NotImplemented

    def __radd__(self, other):
        return self + other

    def __sub__(self, other):
        try:
            diffs = [c1 - c2 for c1, c2 in zip(self, other)]
            return Vector3D(*diffs)
        except TypeError:
            return NotImplemented

    def __rsub__(self, other):
        return self - other

    def __mul__(self, scalar):
        try:
            return Vector3D(*[i * scalar for i in self])
        except TypeError:
            return NotImplemented

    def __rmul__(self, scalar):
        return self * scalar

    def __getitem__(self, key):
        match key:
            case 0:
                return self.x
            case 1:
                return self.y
            case 2:
                return self.z
            case _:
                raise IndexError("Index not found")

    def __len__(self):
        return len(list(self))


if __name__ == "__main__":
    # draw3d(
    #     Points3D((2, 2, 2), (1, -2, -2)),
    #     Arrow3D((2, 2, 2)),
    #     Arrow3D((1, -2, -2)),
    #     Segment3D((2, 2, 2), (1, -2, -2)),
    #     Box3D(2, 2, 2),
    #     Box3D(1, -2, -2),
    # )

    # draw3d(Points3D((-1, -2, 2)), Arrow3D((-1, -2, 2)), Box3D(-1, -2, 2))

    pm1 = [1, -1]

    # points = [
    #     (1, 1, 1),
    #     (1, 1, -1),
    #     (1, -1, -1),
    #     (1, -1, 1),
    #     (-1, -1, -1),
    #     (-1, -1, 1),
    #     (-1, 1, 1),
    #     (-1, 1, -1),
    # ]
    points = [Vector3D(x, y, z) for x in pm1 for y in pm1 for z in pm1]
    segs = (
        [((-1, y, z), (1, y, z)) for y in pm1 for z in pm1]
        + [((x, -1, z), (x, 1, z)) for x in pm1 for z in pm1]
        + [((x, y, -1), (x, y, 1)) for x in pm1 for y in pm1]
    )
    # draw3d(Points3D(*points), *[Segment3D(*seg) for seg in segs])
    things1 = [points[0], 4 * points[0] + points[2]]
    things2 = [points[0], points[0] - points[2]]
    # draw3d(
    #     Points3D(*things1),
    #     Arrow3D(*reversed(things1)),
    #     Points3D(*things2),
    #     # Arrow3D(*reversed(things2)),
    #     Box3D(*things1[1]),
    #     Box3D(*things2[1]),
    # )
    v1 = Vector3D(1, 1, 3)
    v2 = Vector3D(2, 4, -4)
    v3 = Vector3D(4, 2, -2)
    vecs = (Vector3D(0, 0, 0), v1, v1 + v2, v1 + v2 + v3)

    def staggerArrows(vecs):
        vecs = [Vector3D(0, 0, 0)] + list(vecs)
        return (Arrow3D(vecs[i], vecs[i - 1]) for i in range(1, len(vecs)))

    # draw3d(
    #     Points3D(*vecs[1:]),
    #     *(Arrow3D(vecs[i], vecs[i - 1]) for i in range(1, len(vecs))),
    #     *(Box3D(*vecs[i]) for i in range(1, len(vecs))),
    # )
    # draw3d(Points3D(v1, v1 * 2), *staggerArrows((v1, v1 * 2)))
    print(Vector3D(4, 3, 12).length())
