#include "Screen.hpp"

#include "imgui-SFML.h"
#include "imgui.h"

ScreenPtr Screen::instance{};

ScreenPtr& Screen::Instance() {
    // if (!instance) instance = ScreenPtr{new Screen{std::make_unique<SFML_Backend>()}};
    if (!instance) instance = ScreenPtr{new Screen{}};
    return instance;
}

bool Screen::init(const std::string& title, Vector2<int> dim, bool fullscreen, double dt) {
    win->setFramerateLimit(60);
    this->title = title;
    this->fullscreen = fullscreen;
    this->dim.at(0) = dim.at(0);
    this->dim.at(1) = dim.at(1);
    for (int i{}; i < 10; i++) addRandomParticle();
    this->dt = dt;
    create();
    running = true;
    return ImGui::SFML::Init(*win);
}

void Screen::create() {
    auto style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;
    win->create(
        {
            (uint)dim.at(0),
            (uint)dim.at(1),
            32,
        },
        title, style);
}

void Screen::destroy() { win->close(); }

void Screen::toggleFullscreen() {
    fullscreen = !fullscreen;
    destroy();
    create();
}

Vector2<double> Screen::collisionResponse(const Vector2<double>& v, const Vector2<double>& n) const {
    return v - (1 + e) * n * (v * n).at(0);
}

void Screen::update() {
    for (auto& part : particles) {
        const auto& pos = part->gPos();
        const auto& vel = part->gVel();
        const auto& radius = part->gRadius();

        // Left wall collision
        if (pos.at(0) - radius <= 0) {
            if ((vel * nL).at(0) < 0) {  // Check if the object is moving towards the wall
                auto newVel = collisionResponse(vel, nL);
                part->sVel(newVel);

                auto depth = -(pos.at(0) - radius);
                part->sPos(pos + nL * depth);
            }
        }

        // Right wall collision
        if (pos.at(0) + radius >= dim.at(0)) {
            if ((vel * nR).at(0) < 0) {  // Check if the object is moving towards the wall
                auto newVel = collisionResponse(vel, nR);
                part->sVel(newVel);

                auto depth = (pos.at(0) - radius) - dim.at(0) + radius * 2;
                part->sPos(pos + nR * depth);
            }
        }

        // Bottom wall collision
        if (pos.at(1) + radius >= dim.at(1)) {
            if ((vel * nB).at(0) < 0) {  // Check if the object is moving towards the wall
                auto newVel = collisionResponse(vel, nB);
                part->sVel(newVel);

                auto depth = (pos.at(1) - radius) - dim.at(1) + radius * 2.1;
                part->sPos(pos + nB * depth);
            }
        }

        // Top wall collision
        if (pos.at(1) - radius <= 0) {
            if ((vel * nT).at(0) < 0) {  // Check if the object is moving towards the wall
                auto newVel = collisionResponse(vel, nT);
                part->sVel(newVel);

                auto depth = -(pos.at(1) - radius);
                part->sPos(pos + nT * depth);
            }
        }

        part->update(dt);
    }

    for (auto it = particles.begin(); it != particles.end();) {
        const auto& part = *it;
        const auto& pos = part->gPos();
        if (pos.at(0) > dim.at(0) + part->gRadius() + 10 || pos.at(0) + part->gRadius() + 10 < 0 ||
            pos.at(1) > dim.at(1) + part->gRadius() + 10 || pos.at(1) + part->gRadius() + 10 < 0) {
            removeParticle(it);
        } else {
            ++it;
        }
    }

    const auto& [x, y] = win->getSize();
    dim.at(0) = (int)x - 10;
    dim.at(1) = (int)y - 10;
}

void Screen::addRandomParticle() {
    std::default_random_engine rng{std::random_device{}()};
    std::uniform_real_distribution<double> rngX{0, (double)dim.at(0)};
    std::uniform_real_distribution<double> rngY{0, (double)dim.at(1)};
    std::uniform_int_distribution<int> rngMass{2, 15};
    std::uniform_real_distribution<double> rngVel{-20, 100};
    std::uniform_int_distribution<int> rngColor{0, 255};

    double x = rngX(rng);
    double y = rngY(rng);
    double velX = rngVel(rng);
    double velY = rngVel(rng);
    int mass = rngMass(rng);
    sf::Color color = {static_cast<uint8_t>(rngColor(rng)), static_cast<uint8_t>(rngColor(rng)),
                       static_cast<uint8_t>(rngColor(rng)), 255};
    addParticle({x, y}, {velX, velY}, mass, color);
}

void Screen::addParticle(const Vector2<double>& pos, const Vector2<double>& vel, int mass) {
    particles.emplace_back(std::make_unique<Particle>(pos, vel, mass, gravity));
    numParticles++;
}

void Screen::addParticle(const Vector2<double>& pos, const Vector2<double>& vel, int mass, sf::Color color) {
    particles.emplace_back(std::make_unique<Particle>(pos, vel, mass, gravity, color));
    numParticles++;
}

void Screen::removeParticle(const PartPtr& particle) {
    auto res = std::remove(particles.begin(), particles.end(), particle);
    numParticles--;
}

void Screen::renderUI() {
    ImGui::Begin("Particles");
    ImGui::Text("Particles: %zu", numParticles);
    ImGui::Text("dt: %f", dt);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::InputDouble("dt", &dt, dt, 0.1, "%.2f");

    // Change gravity
    ImGui::InputDouble("Gravity X", &gravity.at(0), 0.1, 1.0, "%.2f");
    ImGui::InputDouble("Gravity Y", &gravity.at(1), 0.1, 1.0, "%.2f");

    // Change elasticity
    ImGui::InputDouble("Elasticity", &e, 0.1, 1.0, "%.2f");

    // Change new particle values
    ImGui::InputDouble("New Particle Vel X", &newParticleVel.at(0), 0.1, 1.0, "%.2f");
    ImGui::InputDouble("New Particle Vel Y", &newParticleVel.at(1), 0.1, 1.0, "%.2f");

    ImGui::InputInt("Particle Mass", &newParticleMass);

    ImGui::ColorPicker4("Particle Color", floatArr);
    newParticleColor = {static_cast<uint8_t>(floatArr[0] * 255), static_cast<uint8_t>(floatArr[1] * 255),
                        static_cast<uint8_t>(floatArr[2] * 255), static_cast<uint8_t>(floatArr[3] * 255)};

    // Purge particles
    ImGui::InputInt("Number to Keep", &targetNumParticles);
    if (ImGui::Button("Purge")) {
        purgeParticles(targetNumParticles);
    }

    ImGui::End();
}

void Screen::purgeParticles(int targetNumParticles) {
    size_t i{};
    while (numParticles > targetNumParticles) {
        removeParticle(particles.begin() + i);
        i++;
    }
}

void Screen::render() {
    ImGui::SFML::Update(*win, dT.restart());
    renderUI();
    win->clear({0xE4, 0xE4, 0xE4, 255});
    for (const auto& part : particles) win->draw(part->draw());
    ImGui::SFML::Render(*win);
    win->display();
}

void Screen::handleEvents() {
    // TheInputHandler::Instance()->update();
    sf::Event e{};
    const double minDt = 0.0;
    const double maxDt = 1.0;
    const double step = 0.02;
    while (win->pollEvent(e)) {
        ImGui::SFML::ProcessEvent(*win, e);
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) quit();

        if (e.type == sf::Event::Closed)
            ParticleScreen::Instance()->quit();
        else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F11)
            toggleFullscreen();
        else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space)
            for (int i{}; i < 10; i++) addRandomParticle();
        else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Period) {
            dt += step;
            if (dt > maxDt) {
                dt = maxDt;
            }
            // std::cout << std::format("dt: {}\n", dt);
        } else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Comma) {
            dt -= step;
            if (dt < minDt) {
                dt = minDt;
            }
            // std::cout << std::format("dt: {}\n", dt);
        }

        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            mousePressed = true;
        }

        if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) {
            mousePressed = false;
        }
    }

    if (ImGui::GetIO().WantCaptureMouse) {
        // ImGui is capturing mouse input, so skip additional processing
        mousePressed = false;
        return;
    }

    if (mousePressed) {
        auto mPos = sf::Mouse::getPosition(*win);
        addParticle({(float)mPos.x, (float)mPos.y}, newParticleVel, newParticleMass, newParticleColor);
    }
}

void Screen::quit() { running = false; }

void Screen::clean() {
    win->close();
    ImGui::SFML::Shutdown();
    win.reset();
    cout << "Cleaned Screen\n";
}
