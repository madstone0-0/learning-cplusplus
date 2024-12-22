#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

#include "Particle.hpp"

class Spring {
   public:
    Spring(const PartPtr& first, const PartPtr& second, double restLength = 5)
        : restLength{restLength}, first{first}, second{second} {
        spring.setSize({2, static_cast<float>(restLength)});
    }

    void update() {
        position = first->gPos() - second->gPos();
        velocity = first->gVel() - second->gVel();
        currLength = position.length();
        auto r = first->gPos() - second->gPos();
        auto d = r.length();
        auto n = r / d;
        auto F = -K * (d - restLength) * n;
        first->applyForce(F / 2);
        second->applyForce(-1. * (F / 2));
    }

    sf::RectangleShape draw() {
        spring.setPosition(first->gPos().at(0), first->gPos().at(1));
        spring.setSize({2, static_cast<float>(currLength)});
        return spring;
    }

   private:
    Vector2<double> position;
    Vector2<double> velocity;
    double restLength{5};
    double currLength{restLength};
    double K{100};
    sf::RectangleShape spring;
    PartPtr first;
    PartPtr second;
};

using SpringPtr = std::unique_ptr<Spring>;
