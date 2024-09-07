#pragma once
#include <SDL2_gfxPrimitives.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "SDL_utils.hpp"
using namespace SDL_Utils;
class Particle {
   public:
    Particle() = default;

    Particle(const Vector2<double>& pos, const Vector2<double>& vel, int mass, Vector2<double>& gravity,
             sf::Color color = {0x00, 0x00, 0x00, 0xFF})

        : velocity{vel}, position{pos}, gravity{gravity}, color{color}, mass{mass} {}

    void update(double dt = 1) {
        velocity += gravity * mass * dt;
        position += velocity * dt;
    }

    [[nodiscard]] double gRadius() const { return mass * 2; }

    void sVel(const Vector2<double>& vel) { velocity = vel; }

    [[nodiscard]] Vector2<double> gVel() const { return velocity; }

    void sPos(const Vector2<double>& pos) { position = pos; }

    [[nodiscard]] Vector2<double> gPos() const { return position; }

    [[nodiscard]] int gMass() const { return mass; }

    [[nodiscard]] sf::CircleShape draw() const {
        auto circle = sf::CircleShape(gRadius());
        circle.setPosition(position.at(0), position.at(1));
        circle.setFillColor({color.r, color.g, color.b, color.a});
        return circle;
    }

    // void draw(const SDL_Utils::RendererPtr& renderer) const {
    //     filledCircleRGBA(renderer.get(), (int)std::round(position.at(0)), (int)std::round(position.at(1)), gRadius(),
    //                      color.r, color.g, color.b, color.a);
    // }

   private:
    Vector2<double> velocity{1, 1};
    Vector2<double> position{0, 0};
    Vector2<double>& gravity;
    sf::Color color{0x00, 0x00, 0x00, 0xFF};
    int mass{5};
};

using PartPtr = std::shared_ptr<Particle>;
