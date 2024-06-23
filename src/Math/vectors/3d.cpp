#include <fmt/core.h>
/*#include <matplotlibcpp.h>*/
#include <matplot/matplot.h>
#include <python3.12/Python.h>
#include <semaphore.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <deque>
#include <exception>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <numbers>
#include <ranges>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "math_utils.hpp"
namespace plt = matplot;

using std::cout, std::endl, std::string, std::array, std::map;
using std::make_unique;
using std::unique_ptr;
using std::vector;
using std::numbers::pi;
namespace rg = std::ranges;

struct Vector3D {
    Vector3D() = default;

    Vector3D(double x, double y, double z) : x{x}, y{y}, z{z} {}

    double x{};
    double y{};
    double z{};
};

using Vectors = vector<Vector3D>;

struct Object3D {
    explicit Object3D(string color) : color{std::move(color)} {}
    virtual ~Object3D() = default;

    string color;
};

struct Polygon3D : public Object3D {
    Polygon3D(std::initializer_list<Vector3D> l, const string& col = black) : Object3D{col}, nodes{l} {}

    explicit Polygon3D(Vectors nodes, const string& col = blue) : Object3D{col}, nodes{std::move(nodes)} {}

    ~Polygon3D() override = default;

    Vectors nodes;
    double alpha{0.4};
};

struct Points3D : public Object3D {
    Points3D(std::initializer_list<Vector3D> list, const string& color = black) : Object3D(color), nodes{list} {}
    Points3D(const vector<Vector3D>& nodes, const string& color = black) : Object3D{color}, nodes{nodes} {}
    ~Points3D() override = default;
    Vectors nodes;
};

struct Segment3D : public Object3D {
    Segment3D(const Vector3D& s, const Vector3D& e, const string& color = blue) : Object3D{color}, start{s}, stop{e} {}
    Vector3D start;
    Vector3D stop;
};

struct Arrow3D : public Object3D {
    Arrow3D(const Vector3D& s, const Vector3D& e = {0, 0, 0}, const string& color = blue)
        : Object3D{color}, tail{s}, tip{e} {}
    Vector3D tail;
    Vector3D tip;
};

using Objs = vector<unique_ptr<Object3D>>;

Vectors extractVectors(const Objs& objs) {
    Vectors res;
    for (const auto& obj : objs) {
        if (instanceof <Polygon3D>(obj)) {
            auto* poly = dynamic_cast<Polygon3D*>(obj.get());
            res.insert(res.end(), poly->nodes.begin(), poly->nodes.end());
        } else if (instanceof <Points3D>(obj)) {
            auto* point = dynamic_cast<Points3D*>(obj.get());
            res.insert(res.end(), point->nodes.begin(), point->nodes.end());
        } else if (instanceof <Segment3D>(obj)) {
            auto* seg = dynamic_cast<Segment3D*>(obj.get());
            res.push_back(seg->start);
            res.push_back(seg->stop);
        } else if (instanceof <Arrow3D>(obj)) {
            auto* arrow = dynamic_cast<Arrow3D*>(obj.get());
            res.push_back(arrow->tail);
            res.push_back(arrow->tip);
        }
    }
    return res;
}

using Keywords = map<string, string>;

template <typename DrawFunc>
    requires std::is_invocable_v<DrawFunc, vector<double>, vector<double>, vector<double>, Keywords>
void draw(const Vectors& coords, DrawFunc f, const Keywords& kw = {{}}) {
    vector<double> xs;
    vector<double> ys;
    vector<double> zs;
    for (const auto& coord : coords) {
        const auto& x = coord.x;
        const auto& y = coord.y;
        const auto& z = coord.z;
        xs.push_back(x);
        ys.push_back(y);
        zs.push_back(z);
    }
    f(xs, ys, zs, kw);
}

void drawPoints(const Vectors& coords, const string& color = blue, const string& marker = "o") {
    auto drawFunc = [](const auto& xs, const auto& ys, const auto& zs, const Keywords& kw) {
        plt::plot3(xs, ys, zs)->color(kw.at("color")).marker(kw.at("marker"));
    };
    draw(coords, drawFunc, {{"color", color}, {"marker", marker}});
}

void drawPolygon(const Vectors& coords, const string& color = "b-") {
    auto drawFunc = [](const auto& xs, const auto& ys, const auto& zs, const Keywords& kw) {
        auto p = plt::plot3(xs, ys, zs);
        p->color(kw.at("color"));
        p->line_width(2);
    };
    Vectors poly{coords};
    poly.push_back(coords.at(0));
    draw(poly, drawFunc, {{"color", color}});
}

void drawSegment(const Vector3D& a, const Vector3D& b) { drawPolygon(Vectors{a, b}, {{}}); }

void drawSegment(const Segment3D& segment, const string& color = "r-") {
    drawPolygon(Vectors{segment.start, segment.stop}, color);
}

template <typename... Args>
Objs un(std::unique_ptr<Object3D> obj, Args&&... args) {
    Objs objects;
    objects.push_back(std::move(obj));
    (objects.push_back(std::move(args)), ...);  // Fold expression to unpack variadic arguments
    return objects;
}

void drawObjects(const Objs& objects, bool origin = true, bool axes = true, const array<int, 2>& grid = {1, 1},
                 bool niceAspectRatio = true, int width = 6, float azim = 40, float elev = 35) {
    auto allVectors{extractVectors(objects)};
    auto fig = plt::gca();
    fig->view(azim, elev);

    if (origin) allVectors.emplace_back(0, 0, 0);
    vector<double> xs;
    vector<double> ys;
    vector<double> zs;
    for (const auto& coord : allVectors) {
        const auto& x = coord.x;
        const auto& y = coord.y;
        const auto& z = coord.z;
        xs.push_back(x);
        ys.push_back(y);
        zs.push_back(z);
    }

    auto maxX{rg::max(xs)};
    auto minX{rg::min(xs)};

    auto maxY{rg::max(ys)};
    auto minY{rg::min(ys)};

    auto maxZ{rg::max(zs)};
    auto minZ{rg::max(zs)};

    auto xSize{maxX - minX};
    auto ySize{maxY - minY};
    auto zSize{maxZ - minZ};

    auto xPad{xSize != 0 ? 0.05 * xSize : 1};
    auto yPad{ySize != 0 ? 0.05 * ySize : 1};
    auto zPad{zSize != 0 ? 0.05 * zSize : 1};

    array<double, 2> pltXRng{std::min(minX - xPad, -2.), std::max(maxX + xPad, 2.)};
    array<double, 2> pltYRng{std::min(minY - yPad, -2.), std::max(maxY + yPad, 2.)};
    array<double, 2> pltZRng{std::min(minZ - zPad, -2.), std::max(maxZ + zPad, 2.)};
    fig->xlabel("x");
    fig->ylabel("y");
    fig->zlabel("z");

    plt::hold(true);
    if (axes) {
        drawSegment({pltXRng.at(0), 0, 0}, {pltXRng.at(1), 0, 0});
        drawSegment({0, pltYRng.at(0), 0}, {0, pltYRng.at(1), 0});
        drawSegment({0, 0, pltZRng.at(0)}, {0, 0, pltZRng.at(1)});
    }

    if (origin) {
        drawPoints({{0, 0, 0}}, blue, "x");
    }

    for (const auto& obj : objects) {
        if (instanceof <Polygon3D>(obj.get())) {
            auto* poly = dynamic_cast<Polygon3D*>(obj.get());
            drawPolygon(poly->nodes, poly->color);
        } else if (instanceof <Points3D>(obj.get())) {
            auto* points = dynamic_cast<Points3D*>(obj.get());
            drawPoints(points->nodes, points->color);
        } else if (instanceof <Segment3D>(obj.get())) {
            auto* seg = dynamic_cast<Segment3D*>(obj.get());
            drawSegment(*seg, seg->color);
        } else if (instanceof <Arrow3D>(obj.get())) {
            auto* arrow = dynamic_cast<Arrow3D*>(obj.get());
            drawSegment(arrow->tail, arrow->tip);
        }
    }
    plt::show();
    plt::hold(false);
}

int main() {
    drawObjects(un(make_unique<Points3D>(Vectors{{1, 1, 1}, {1, 2, 1}})));
    drawObjects(un(make_unique<Polygon3D>(Vectors{
        {1, 1, 1},
        {1, 2, 1},
        {2, 2, 1},
        {2, 1, 1},
    })));
}
