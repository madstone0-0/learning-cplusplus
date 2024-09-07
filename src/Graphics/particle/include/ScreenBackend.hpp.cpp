#include "Screen.hpp"

#include "SFML_Backend.hpp"

ScreenPtr Screen::instance{};

BackendPtr<RenderWindowPtr, RenderWindowPtr> Screen::backend{std::make_unique<SFML_Backend>()};

ScreenPtr& Screen::Instance() {
    // if (!instance) instance = ScreenPtr{new Screen{std::make_unique<SFML_Backend>()}};
    if (!instance) instance = ScreenPtr{new Screen{}};
    return instance;
}

bool Screen::init(const std::string& title, int xpos, int ypos, Vector2<int> dim, bool fullscreen, double dt) {
    backend->init(title, xpos, ypos, dim, fullscreen);
    for (int i{}; i < 10; i++) addRandomParticle();
    this->dt = dt;
    running = true;
    return true;
}

void Screen::update() {
    // Function to compute the collision response
    auto collisionRes = [](const Vector2<double>& v, const Vector2<double>& n, double e = 0.5) {
        return v - (1 + e) * n * (v * n).at(0);
    };

    for (auto& part : particles) {
        const auto& pos = part->gPos();
        const auto& vel = part->gVel();
        const auto& radius = part->gRadius();

        // Left wall collision
        if (pos.at(0) - radius <= 0) {
            auto n = Vector2<double>{1, 0};
            if ((vel * n).at(0) < 0) {  // Check if the object is moving towards the wall
                auto newVel = collisionRes(vel, n);
                part->sVel(newVel);

                auto depth = -(pos.at(0) - radius) + radius;
                part->sPos(pos + n * depth);
            }
        }

        // Right wall collision
        if (pos.at(0) + radius >= backend->getDimens().at(0)) {
            auto n = Vector2<double>{-1, 0};
            if ((vel * n).at(0) < 0) {  // Check if the object is moving towards the wall
                auto newVel = collisionRes(vel, n);
                part->sVel(newVel);

                auto depth = (pos.at(0) - radius) - backend->getDimens().at(0) + radius * 2;
                part->sPos(pos + n * depth);
            }
        }

        // Bottom wall collision
        if (pos.at(1) + radius >= backend->getDimens().at(1)) {
            auto n = Vector2<double>{0, -1};
            if ((vel * n).at(0) < 0) {  // Check if the object is moving towards the wall
                auto newVel = collisionRes(vel, n);
                part->sVel(newVel);

                auto depth = (pos.at(1) - radius) - backend->getDimens().at(1) + radius * 2;
                part->sPos(pos + n * depth);
            }
        }

        // Top wall collision
        if (pos.at(1) - radius <= 0) {
            auto n = Vector2<double>{0, 1};
            if ((vel * n).at(0) < 0) {  // Check if the object is moving towards the wall
                auto newVel = collisionRes(vel, n);
                part->sVel(newVel);

                auto depth = -(pos.at(1) - radius);
                part->sPos(pos + n * depth);
            }
        }

        part->update(dt);
    }

    for (auto it = particles.begin(); it != particles.end();) {
        const auto& part = *it;
        const auto& pos = part->gPos();
        if (pos.at(0) > backend->getDimens().at(0) + part->gRadius() + 10 || pos.at(0) + part->gRadius() + 10 < 0 ||
            pos.at(1) > backend->getDimens().at(1) + part->gRadius() + 10 || pos.at(1) + part->gRadius() + 10 < 0) {
            removeParticle(it);
        } else {
            ++it;
        }
    }

    const auto dims = backend->getDimens();
    backend->getDimens().at(0) = (int)backend->getDimens().at(0);
    backend->getDimens().at(1) = (int)backend->getDimens().at(1);
}

void Screen::addRandomParticle() {
    std::default_random_engine rng{std::random_device{}()};
    std::uniform_real_distribution<double> rngX{0, (double)backend->getDimens().at(0)};
    std::uniform_real_distribution<double> rngY{0, (double)backend->getDimens().at(1)};
    std::uniform_int_distribution<int> rngMass{2, 15};
    std::uniform_real_distribution<double> rngVel{-5, 5};

    double x = rngX(rng);
    double y = rngY(rng);
    double velX = rngVel(rng);
    double velY = rngVel(rng);
    int mass = rngMass(rng);
    addParticle({x, y}, {velX, velY}, mass);
}

void Screen::addParticle(const Vector2<double>& pos, const Vector2<double>& vel, int mass) {
    particles.emplace_back(std::make_unique<Particle>(pos, vel, mass));
    numParticles++;
}

void Screen::addParticle(const Vector2<double>& pos, const Vector2<double>& vel, int mass, SDL_Color color) {
    particles.emplace_back(std::make_unique<Particle>(pos, vel, mass, color));
    numParticles++;
}

void Screen::removeParticle(const PartPtr& particle) {
    auto res = std::remove(particles.begin(), particles.end(), particle);
    numParticles--;
}

void Screen::render() {
    backend->beginRender();
    for (const auto& part : particles) backend->render(part);
    backend->endRender();
}

void Screen::handleEvents() {
    TheInputHandler::Instance()->update();
    backend->handleEvent();

    // const double minDt = 0.0;
    // const double maxDt = 1.0;
    // const double step = 0.02;
    // if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
    //     for (int i{}; i < 10; i++) addRandomParticle();
    //
    // else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_PERIOD)) {
    //     dt += step;
    //     if (dt > maxDt) {
    //         dt = maxDt;
    //     }
    //     std::cout << std::format("dt: {}\n", dt);
    // } else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_COMMA)) {
    //     dt -= step;
    //     if (dt < minDt) {
    //         dt = minDt;
    //     }
    //     std::cout << std::format("dt: {}\n", dt);
    // }
    //
    // if (TheInputHandler::Instance()->mBState(MOUSE_BUTTONS::LEFT)) {
    //     const auto& mPos = TheInputHandler::Instance()->mPos();
    //     addParticle(*mPos, {1, 1}, 5, {0xFF, 0x00, 0x00, 0xFF});
    // }
}

void Screen::quit() { running = false; }

void Screen::clean() {
    if (backend) {
        backend->clean();  // Ensure proper shutdown
        backend.reset();   // Release the backend to avoid use-after-free
    }
    cout << "Cleaned Screen\n";
}
