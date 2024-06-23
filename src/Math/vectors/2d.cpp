#include <fmt/core.h>
#include <matplot/matplot.h>
#include <matplotlibcpp.h>
#include <python3.12/Python.h>
#include <semaphore.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <deque>
#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <numbers>
#include <ranges>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "math_utils.hpp"
namespace plt = matplot;

using std::cout, std::endl, std::string, std::array;
using std::make_unique;
using std::unique_ptr;
using std::vector;
using std::numbers::pi;
namespace rg = std::ranges;

struct CartesianCoord {
    double x;
    double y;
};

struct PolarCoord {
    double length;
    double angle;
};

struct Vector2D {
    Vector2D() : x{}, y{}, angle{}, len{} {}
    explicit Vector2D(const CartesianCoord& c) : x{c.x}, y{c.y}, angle{atan2(y, x)}, len{sqrt(x * x + y * y)} {}
    Vector2D(double x, double y) : Vector2D{CartesianCoord{x, y}} {}
    explicit Vector2D(const PolarCoord p) : angle{p.angle}, len{p.length} {
        x = len * cos(angle);
        y = len * sin(angle);
    }

    Vector2D operator+(const Vector2D& other) const { return {this->x + other.x, this->y + other.y}; }

    [[nodiscard]] double length() const noexcept { return len; }
    [[nodiscard]] double distance(const Vector2D& other) const noexcept {
        return sqrt(pow(other.x - this->x, 2) + pow(other.y - this->y, 2));
    }
    [[nodiscard]] auto displacement(const Vector2D& other) const noexcept { return *this - other; }

    template <Numeric Num>
    Vector2D operator*(const Num& num) const {
        return {x * num, y * num};
    }

    Vector2D operator-() const { return *this * -1; }

    Vector2D operator-(const Vector2D& other) const noexcept { return *this + (-other); }

    double x;
    double y;
    double angle;
    double len;
};

using Vectors = vector<Vector2D>;

struct Object {
    explicit Object(string color) : color{std::move(color)} {}
    virtual ~Object() = default;

    string color;
};

struct Polygon : public Object {
    Polygon(std::initializer_list<Vector2D> list, const string& color = black) : Object(color), nodes{list} {}
    explicit Polygon(const vector<Vector2D>& nodes, const string& color = blue) : Object{color}, nodes{nodes} {}
    vector<Vector2D> nodes;
    ~Polygon() override = default;
    double alpha{0.4};
};

struct Points : public Object {
    Points(std::initializer_list<Vector2D> list, const string& color = black) : Object(color), nodes{list} {}
    explicit Points(const vector<Vector2D>& nodes, const string& color = black) : Object{color}, nodes{nodes} {}
    ~Points() override = default;
    vector<Vector2D> nodes;
};

struct Segment : public Object {
    Segment(const Vector2D& s, const Vector2D& e, const string& color = blue) : Object{color}, start{s}, stop{e} {}
    Vector2D start;
    Vector2D stop;
};

struct Arrow : public Object {
    Arrow(const Vector2D& s, const Vector2D& e, const string& color = blue) : Object{color}, tail{s}, tip{e} {}
    Vector2D tail;
    Vector2D tip;
};

template <typename DrawFunc>
    requires std::is_invocable_v<DrawFunc, vector<double>, vector<double>, string>
void draw(const vector<Vector2D>& coords, DrawFunc f, const string& format = "") {
    vector<double> xs;
    vector<double> ys;
    for (const auto& coord : coords) {
        const auto& x = coord.x;
        const auto& y = coord.y;
        xs.push_back(x);
        ys.push_back(y);
    }
    f(xs, ys, format);
}

void drawPoints(const vector<Vector2D>& coords, const string& color = "") {
    auto drawFunc = [](const auto& xs, const auto& ys, const string& format) { plt::plot(xs, ys, format); };
    draw(coords, drawFunc, color + "o");
}

void drawPolygon(const vector<Vector2D>& coords, const string& format = "") {
    auto drawFunc = [](const auto& xs, const auto& ys, const string& format) { plt::plot(xs, ys, format); };
    vector<Vector2D> poly{coords};
    poly.push_back(coords.at(0));
    draw(poly, drawFunc, format);
}

void drawSegment(const Vector2D& a, const Vector2D& b) { drawPolygon(vector<Vector2D>{a, b}, "r-"); }

void drawSegment(const Segment& segment, const string& color = "r-") {
    drawPolygon(vector<Vector2D>{segment.start, segment.stop}, color);
}

using Objs = vector<unique_ptr<Object>>;

vector<Vector2D> extractVectors(const Objs& objs) {
    vector<Vector2D> res;
    for (const auto& obj : objs) {
        if (instanceof <Polygon>(obj)) {
            auto* poly = dynamic_cast<Polygon*>(obj.get());
            res.insert(res.end(), poly->nodes.begin(), poly->nodes.end());
        } else if (instanceof <Points>(obj)) {
            auto* point = dynamic_cast<Points*>(obj.get());
            res.insert(res.end(), point->nodes.begin(), point->nodes.end());
        } else if (instanceof <Segment>(obj)) {
            auto* seg = dynamic_cast<Segment*>(obj.get());
            res.push_back(seg->start);
            res.push_back(seg->stop);
        } else if (instanceof <Arrow>(obj)) {
            auto* arrow = dynamic_cast<Arrow*>(obj.get());
            res.push_back(arrow->tail);
            res.push_back(arrow->tip);
        }
    }
    return res;
}

template <typename... Args>
Objs un(std::unique_ptr<Object> obj, Args&&... args) {
    Objs objects;
    objects.push_back(std::move(obj));
    (objects.push_back(std::move(args)), ...);  // Fold expression to unpack variadic arguments
    return objects;
}

/*void drawObjects(const Objs& objects, bool origin = true, bool axes = true, const array<int, 2>& grid = {1, 1},*/
/*                 bool niceAspectRatio = true, int width = 6) {*/
void drawObjects(const Objs& objects, bool origin = true, bool axes = true, const array<int, 2>& grid = {1, 1},
                 bool niceAspectRatio = true, int width = 6) {
    auto allVectors{extractVectors(objects)};

    vector<double> xs(allVectors.size());
    vector<double> ys(allVectors.size());
    rg::for_each(allVectors, [&xs, &ys](const auto& coord) {
        xs.push_back(coord.x);
        ys.push_back(coord.y);
    });
    xs.push_back(0);
    ys.push_back(0);
    auto maxX = rg::max(xs);
    auto minX = rg::min(xs);
    auto maxY = rg::max(ys);
    auto minY = rg::min(ys);
    double xPad = std::max(std::ceil(0.05 * (maxX - minX)), (double)grid.at(0));
    double yPad = std::max(std::ceil(0.05 * (maxY - minY)), (double)grid.at(1));

    auto roundUpToMultiple = [](auto val, auto size) { return floor((val + size) / size) * size; };

    auto roundDownToMultiple = [](auto val, auto size) { return -floor((-val - size) / size) * size; };
    array<double, 2> xlim{(floor(minX - xPad) / grid[0] * grid[0]), (ceil((maxX + xPad) / grid[0]) * grid[0])};
    array<double, 2> ylim{(floor(minY - yPad) / grid[1] * grid[1]), (ceil((maxY + yPad) / grid[1]) * grid[1])};

    if (!grid.empty()) {
        plt::xlim(xlim);
        plt::ylim(ylim);
    } else {
        xPad = 0.05 * (maxX - minX);
        yPad = 0.05 * (maxY - minY);
        plt::xlim(array<double, 2>{minX - xPad, maxX + xPad});
        plt::ylim(array<double, 2>{minY - yPad, maxY + yPad});
    }

    /*if (origin) plt::plot(vector<double>{0, 0}, vector<double>{0, 0}, "ks");*/
    if (origin) {
        auto s = plt::scatter(vector<double>{0}, vector<double>{0});
        s->color("k");
        s->marker("x");
    }

    if (!grid.empty()) {
        vector<double> xTicks;
        vector<double> yTicks;
        auto xlim = plt::xlim();
        auto ylim = plt::ylim();

        for (auto i{xlim.at(0)}; i < xlim.at(1); i += grid.at(0)) xTicks.push_back(i);

        for (auto i{ylim.at(0)}; i < ylim.at(1); i += grid.at(1)) yTicks.push_back(i);

        plt::xticks(xTicks);
        plt::yticks(yTicks);
        plt::grid(true);
    }

    plt::hold(true);
    if (axes) {
        plt::axis(true);
        /*auto x = plt::plot(vector{static_cast<int>(xlim.at(0)), 0}, vector{static_cast<int>(xlim.at(1)), 0});*/
        /*x->line_width(2);*/
        /*x->color("black");*/
        /*plt::axhline(0, 0., 1, {{"color", "k"}});*/
        /*plt::axvline(0, 0., 1, {{"color", "k"}});*/
    }

    for (const auto& object : objects) {
        if (instanceof <Polygon>(object)) {
            auto* poly = dynamic_cast<Polygon*>(object.get());
            drawPolygon(poly->nodes, poly->color);
        } else if (instanceof <Points>(object)) {
            auto* points = dynamic_cast<Points*>(object.get());
            drawPoints(points->nodes, points->color);
        } else if (instanceof <Arrow>(object)) {
            auto* arrow = dynamic_cast<Arrow*>(object.get());
            const auto& tip = arrow->tip;
            const auto& tail = arrow->tail;
            auto tipLen = (xlim.at(1) - xlim.at(0)) / 20;
            auto len = sqrt(pow(tip.y - tail.y, 2) + pow(tip.x - tail.x, 2));
            auto nLen = len - tipLen;
            auto newY = (tip.y - tail.y) * (nLen / len);
            auto newX = (tip.x - tail.x) * (nLen / len);
            auto a = plt::arrow(tail.x, tail.y, newX, newY);
            a->color(arrow->color);
            a->line_width(tipLen / 1.5);
        } else if (instanceof <Segment>(object)) {
            auto* seg = dynamic_cast<Segment*>(object.get());
            drawSegment(*seg);
        }
    }

    plt::show();
    plt::hold(false);
}

Vectors translate(Vector2D translation, const Vectors& vectors) {
    Vectors res{};
    rg::transform(vectors, std::back_inserter(res), [&translation](auto coords) { return coords + translation; });
    return res;
}
template <Numeric Num>
Vectors scale(Num factor, const Vectors& vectors) {
    Vectors res{};
    rg::transform(vectors, std::back_inserter(res), [&factor](auto coords) { return coords * factor; });
    return res;
}

Objs hundredDinos() {
    Vectors dino{{6, 4},  {3, 1},  {1, 2},  {-1, 5}, {-2, 5},  {-3, 4}, {-4, 4}, {-5, 3}, {-5, 2}, {-2, 2}, {-5, 1},
                 {-4, 0}, {-2, 1}, {-1, 0}, {0, -3}, {-1, -4}, {1, -4}, {2, -3}, {1, -2}, {3, -1}, {5, 1}};
    // dino = scale(0.5, dino);
    Objs res{};
    Vectors translations{};
    for (int x = -5; x < 5; x++)
        for (int y = -5; y < 5; y++) {
            translations.emplace_back(x * 12., y * 10.);
        }
    for (auto trans : translations) {
        res.push_back(make_unique<Polygon>(translate(trans, dino)));
    }
    return res;
}

/* Vectors */
/*int main() {*/
/*    try {*/
/*        vector<Vector2D> dino{{6, 4},  {3, 1},   {1, 2},  {-1, 5}, {-2, 5}, {-3, 4}, {-4, 4},*/
/*                              {-5, 3}, {-5, 2},  {-2, 2}, {-5, 1}, {-4, 0}, {-2, 1}, {-1, 0},*/
/*                              {0, -3}, {-1, -4}, {1, -4}, {2, -3}, {1, -2}, {3, -1}, {5, 1}};*/
/*        // plt::xkcd();*/
/*        vector<unique_ptr<Object>> obs{};*/
/**/
/*        obs.push_back(std::make_unique<Polygon>(dino));*/
/*        Vectors dinoTrans{};*/
/*        rg::transform(dino, std::back_inserter(dinoTrans),*/
/*                      [](const auto& coord) { return coord + Vector2D{-1.5, -2.5}; });*/
/*        Vectors dinoScale{};*/
/*        rg::transform(dino, std::back_inserter(dinoScale),*/
/*                      [](const auto& coord) { return (coord * 0.5 + Vector2D{5, 5}); });*/
/*        obs.push_back(std::make_unique<Polygon>(dino));*/
/*        obs.push_back(std::make_unique<Points>(dino, blue));*/
/*        obs.push_back(std::make_unique<Polygon>(dinoTrans, red));*/
/*        obs.push_back(std::make_unique<Points>(dinoTrans, red));*/
/*        obs.push_back(std::make_unique<Polygon>(dinoScale, green));*/
/*        obs.push_back(std::make_unique<Points>(dinoScale, green));*/
/*        for (size_t i{}; i < dino.size(); i++) obs.push_back(make_unique<Arrow>(dino.at(i), dinoTrans.at(i), red));*/
/*        for (size_t i{}; i < dino.size(); i++) obs.push_back(make_unique<Arrow>(dino.at(i), dinoScale.at(i),
 * green));*/
/*        drawObjects(obs);*/
/**/
/*        drawObjects(un(make_unique<Points>(vector<Vector2D>{{2, 2}, {-1, 3}}),*/
/*                       make_unique<Segment>(Vector2D{2, 2}, Vector2D{-1, 3}, red),*/
/*                       make_unique<Points>(vector<Vector2D>{-Vector2D{2, 2} + Vector2D{-1, 3}}),*/
/*                       make_unique<Points>(vector<Vector2D>{-Vector2D{2, 2}.displacement(Vector2D{-1, 3})}),*/
/*                       std::make_unique<Polygon>(dino)));*/
/**/
/*        auto newDino{translate({-1.5, -2.5}, dino)};*/
/*        drawObjects(un(std::make_unique<Polygon>(newDino, green), std::make_unique<Points>(newDino, green)));*/
/**/
/*        obs.clear();*/
/*        vector<Vector2D> quad{};*/
/*        for (auto i{-10}; i < 11; i++) quad.emplace_back(i * 1., pow(i, 2));*/
/*        obs.push_back(make_unique<Points>(quad));*/
/*        drawObjects(obs, true, true, {1, 10}, false);*/
/**/
/*        drawObjects(hundredDinos(), false, false, {1, 1}, false);*/
/*        double max = -999;*/
/*        Vector2D maxV{};*/
/*        for (const auto& v : dino)*/
/*            if (v.length() > max) {*/
/*                max = v.length();*/
/*                maxV = v;*/
/*            }*/
/**/
/*        cout << "( " << maxV.x << ", " << maxV.y << " ): " << max << '\n';*/
/**/
/*    } catch (const std::exception& e) {*/
/*        std::cerr << "Error: " << e.what() << endl;*/
/*        return 0;*/
/*    }*/
/*    return 0;*/
/*}*/
/* Vectors */

Vectors rotate(const double& rotate, const Vectors& vectors) {
    Vectors res{};
    rg::transform(vectors, std::back_inserter(res),
                  [&rotate](const Vector2D& x) { return Vector2D{PolarCoord{x.len, x.angle + rotate}}; });
    return res;
}

Vectors polygon(unsigned int sides, double len = 4) {
    Vectors res{};
    res.reserve(sides);
    for (unsigned n{}; n < sides; n++) {
        res.emplace_back(PolarCoord{len, 2 * pi * n / sides});
    }
    return res;
}

/* Angles */
using std::cos;
int main() {
    Vector2D fromPol{PolarCoord{5, toRad(37)}};
    Vector2D norm{10, 10};
    /*drawObjects(un(make_unique<Points>(vector{fromPol, norm})));*/

    Vectors polars{};
    for (int i{}; i < 1000; i++) {
        polars.emplace_back(PolarCoord(cos(5 * i * pi / 500.), 2 * pi * i / 1000.));
    }
    /*drawObjects(un(make_unique<Polygon>(polars)));*/

    // Transforming collections of vectors
    Vectors dino{{6, 4},  {3, 1},  {1, 2},  {-1, 5}, {-2, 5},  {-3, 4}, {-4, 4}, {-5, 3}, {-5, 2}, {-2, 2}, {-5, 1},
                 {-4, 0}, {-2, 1}, {-1, 0}, {0, -3}, {-1, -4}, {1, -4}, {2, -3}, {1, -2}, {3, -1}, {5, 1}};
    Vectors dinoRotated{};
    const auto rotAngle{pi / 4};
    rg::transform(dino, std::back_inserter(dinoRotated),
                  [&rotAngle](const auto& x) { return Vector2D{PolarCoord(x.len, x.angle + rotAngle)}; });
    /*drawObjects(un(make_unique<Polygon>(dino, gray), make_unique<Polygon>(dinoRotated, red)));*/

    // Combining vector transformations
    auto newDino{translate({8, 8}, rotate(5 * pi / 3, dino))};
    /*drawObjects(un(make_unique<Polygon>(newDino), make_unique<Polygon>(dino, gray)));*/

    drawObjects(un(make_unique<Polygon>(polygon(7)), make_unique<Polygon>(polygon(3))));

    drawObjects(un(make_unique<Polygon>(dino, gray), make_unique<Polygon>(translate({8, 8}, dino)),
                   make_unique<Polygon>(rotate(5 * pi / 3, translate({8, 8}, dino)), green)));

    return 0;
}
/* Angles */
