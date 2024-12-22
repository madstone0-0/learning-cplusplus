#pragma once
#include <SDL2_gfxPrimitives.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "SDL_utils.hpp"
using namespace SDL_Utils;

struct ParticleInfo {
    Vector2<double> pos;
    double radius;
    sf::Color color;
};

class Particle {
   public:
    Particle() = default;

    Particle(const Vector2<double>& pos, const Vector2<double>& vel, int mass, Vector2<double>& gravity,
             sf::Color color = {0x00, 0x00, 0x00, 0xFF})

        : velocity{vel}, position{pos}, gravity{gravity}, color{color}, mass{2} {}

    void update(double dt = 1) {
        // Euler's method
        velocity += gravity * mass * dt;
        position += velocity * dt;
    }

    void applyForce(const Vector2<double>& force) {
        // As F = ma, a = F/m and acceleration is the change in velocity
        velocity += force / mass;
    }

    [[nodiscard]] double gRadius() const { return mass * 2; }
    void sVel(const Vector2<double>& vel) { velocity = vel; }

    [[nodiscard]] Vector2<double> gVel() const { return velocity; }

    void sPos(const Vector2<double>& pos) { position = pos; }

    [[nodiscard]] Vector2<double> gPos() const { return position; }

    [[nodiscard]] int gMass() const { return mass; }

    // [[nodiscard]] sf::CircleShape draw() {
    //     circle.setRadius(gRadius());
    //     circle.setPosition(position.at(0), position.at(1));
    //     circle.setFillColor({color.r, color.g, color.b, color.a});
    //     return circle;
    // }

    ParticleInfo draw() { return {position, gRadius(), color}; }

    // sf::CircleShape& gCircle() { return circle; }

    int outlineThickness{};

   private:
    Vector2<double> velocity{1, 1};
    Vector2<double> position{0, 0};
    Vector2<double>& gravity;
    // sf::CircleShape circle;
    sf::Color color{0x00, 0x00, 0x00, 0xFF};
    int mass{5};
};

using PartPtr = std::shared_ptr<Particle>;
