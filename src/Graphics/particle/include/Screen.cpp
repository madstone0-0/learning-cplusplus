#include "Screen.hpp"

#include <imgui.h>

ScreenPtr Screen::instance{};

ScreenPtr& Screen::Instance() {
    static bool firstCall{true};
    if (!instance) instance = ScreenPtr{new Screen{}};
    if (firstCall) {
        firstCall = false;
        std::atexit(cleanupImGui);
    }
    return instance;
}

bool Screen::init(const std::string& title, Vector2<int> dim, bool fullscreen, double dt, uint framerate) {
    win->setFramerateLimit(framerate);
    this->title = title;
    this->fullscreen = fullscreen;
    this->dim.at(0) = dim.at(0);
    this->dim.at(1) = dim.at(1);
    border.setFillColor({0x00, 0x00, 0x00, 0x00});
    border.setOutlineThickness(borderThickness);
    border.setOutlineColor(sf::Color::Black);
    borderXMin = 0;
    borderYMin = 0;
    borderXMax = dim.at(0);
    borderYMax = dim.at(1);
    border.setPosition(borderXMin, borderYMin);
    border.setOrigin(borderXMin, borderYMin);
    border.setSize({borderXMax, borderYMax});
    // for (int i{}; i < 10; i++) addRandomParticle();
    physics->setWinDim(borderXMin, borderXMax, borderYMin, borderYMax);
    physics->dt = dt;
    create();
    running = true;
    cout << std::format("Created window with FPS: {} and Dimensions:\n{}", framerate, std::string{dim});
    isInitialized = ImGui::SFML::Init(*win);
    return isInitialized;
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

void Screen::cullOutOfBoundsParticles() {
    if (!cull) return;
    for (auto it = particles.begin(); it != particles.end();) {
        const auto& part = *it;
        const auto& pos = part->gPos();
        if (pos.at(0) > borderXMax + part->gRadius() + 10 || pos.at(0) + part->gRadius() + 10 < borderXMin ||
            pos.at(1) > borderYMax + part->gRadius() + 10 || pos.at(1) + part->gRadius() + 10 < borderYMin) {
            removeParticle(it);
        } else {
            ++it;
        }
    }
}

void Screen::update() {
    for (auto& part : particles) {
        physics->handleBoxCollision(part);

        part->update(physics->dt);
    }
    physics->handleParticleCollision();

    for (auto& spring : springs) spring->update();

    cullOutOfBoundsParticles();

    const auto& [x, y] = win->getSize();
    dim.at(0) = (int)x - 10;
    dim.at(1) = (int)y - 10;
    if (borderMatchDimens) {
        borderXMax = dim.at(0);
        borderYMax = dim.at(1);
        border.setSize({borderXMax, borderYMax});
        physics->setWinDim(borderXMin, borderXMax, borderYMin, borderYMax);
    }
}

void Screen::addRandomParticle() {
    std::default_random_engine rng{std::random_device{}()};
    std::uniform_real_distribution<double> rngX{0, (double)dim.at(0)};
    std::uniform_real_distribution<double> rngY{0, (double)dim.at(1)};
    std::uniform_int_distribution<int> rngMass{2, 10};
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
    particles.emplace_back(std::make_unique<Particle>(pos, vel, mass, physics->gravity));
    numParticles++;
}

void Screen::addParticle(const Vector2<double>& pos, const Vector2<double>& vel, int mass, sf::Color color) {
    particles.emplace_back(std::make_unique<Particle>(pos, vel, mass, physics->gravity, color));
    numParticles++;
}

void Screen::removeParticle(const PartPtr& particle) {
    auto res = std::remove(particles.begin(), particles.end(), particle);
    numParticles--;
}

void Screen::selectParticle(PartPtr& selection, const Callback& endCallback) {
    const int slack = 3;
    auto mPos = sf::Mouse::getPosition(*win);
    for (const auto& particle : particles) {
        const auto radius = particle->gRadius();
        const auto pos = particle->gPos();
        if (pos.at(0) - radius <= mPos.x + slack && mPos.x - slack <= pos.at(0) + radius &&
            pos.at(1) - radius <= mPos.y + slack && mPos.y - slack <= pos.at(1) + radius) {
            selection = particle;
            // selection->gCircle().setOutlineColor(sf::Color::Red);
            // selection->gCircle().setOutlineThickness(5);
            selection->outlineThickness = 5;

            return;
        }
        endCallback(selection);
    }
}

void Screen::renderUI() {
    ImGui::Begin("Selected Particle");
    if (!selectedParticle)
        ImGui::Text("No Selected Particle");
    else {
        ImGui::Text("Velocity: (%.2f, %.2f)", selectedParticle->gVel().at(0), selectedParticle->gVel().at(1));
        ImGui::Text("Position: (%.2f, %.2f)", selectedParticle->gPos().at(0), selectedParticle->gPos().at(1));
        ImGui::Text("Mass: %d", selectedParticle->gMass());
        ImGui::Text("Radius: %.2f", selectedParticle->gRadius());
    }
    ImGui::End();

    ImGui::Begin("Particles");
    ImGui::Text("Particles: %zu", numParticles);
    ImGui::Text("dt: %f", physics->dt);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::InputDouble("dt", &(physics->dt), physics->dt, 0.1, "%.2f");

    ImGui::Checkbox("Cull Out of Bounds particles", &cull);

    if (ImGui::Checkbox("Place Mode", &placeMode)) {
        springMode = false;
    }

    // Collision Detection Method
    std::vector<std::string> methodStrings;
    methodStrings.reserve(4);
    for (int i = 0; i < 4; i++) {
        methodStrings.push_back(parseCollisionMethods(static_cast<CollisionDetection>(i)));
    }

    // Convert to vector of const char* for ImGui
    std::vector<const char*> items;
    items.reserve(methodStrings.size());
    for (const auto& str : methodStrings) {
        items.push_back(str.c_str());
    }

    int currentItem = static_cast<int>(physics->collisionMethod);
    if (ImGui::Combo("Collision Method", &currentItem, items.data(), items.size())) {
        physics->collisionMethod = static_cast<CollisionDetection>(currentItem);
    }

    // Border Control
    ImGui::Checkbox("Border Match Screen Dimensions", &borderMatchDimens);
    if (!borderMatchDimens) {
        ImGui::SliderFloat("Border X", &borderXMax, 0, dim.at(0));
        ImGui::SliderFloat("Border Y", &borderYMax, 0, dim.at(1));
        border.setSize({borderXMax, borderYMax});
    }

    if (ImGui::Checkbox("Precision Placement", &precisionPlacement)) {
        placeMode = true;
        springMode = false;
    }

    if (ImGui::Checkbox("Spring Mode", &springMode)) {
        placeMode = false;
        precisionPlacement = false;
    }

    // Change coefficient of friction
    ImGui::InputDouble("Friction Coefficient", &(physics->frictionCoef), 0.1, 10.0, "%.2f");

    // Change gravity
    ImGui::InputDouble("Gravity X", &(physics->gravity.at(0)), 0.1, 1.0, "%.2f");
    ImGui::InputDouble("Gravity Y", &(physics->gravity.at(1)), 0.1, 1.0, "%.2f");

    // Change elasticity
    ImGui::InputDouble("Elasticity", &(physics->e), 0.1, 1.0, "%.2f");

    // Change new particle values
    ImGui::InputDouble("New Particle Vel X", &newParticleVel.at(0), 0.1, 1.0, "%.2f");
    ImGui::InputDouble("New Particle Vel Y", &newParticleVel.at(1), 0.1, 1.0, "%.2f");

    ImGui::InputInt("Particle Mass", &newParticleMass);

    ImGui::ColorEdit4("Particle Color", floatArr);
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
    selectedParticle = nullptr;
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
    win->draw(border);
    // for (const auto& part : particles) win->draw(part->draw());
    for (const auto& part : particles) {
        const auto& [position, radius, color] = part->draw();
        particleCircle.setRadius(radius);
        particleCircle.setPosition(position.at(0), position.at(1));
        particleCircle.setFillColor(color);
        particleCircle.setOutlineColor(sf::Color::Red);
        particleCircle.setOutlineThickness(part->outlineThickness);
        win->draw(particleCircle);
    }
    for (const auto& spring : springs) win->draw(spring->draw());
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
            physics->dt += step;
            if (physics->dt > maxDt) {
                physics->dt = maxDt;
            }
            // std::cout << std::format("dt: {}\n", dt);
        } else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Comma) {
            physics->dt -= step;
            if (physics->dt < minDt) {
                physics->dt = minDt;
            }
            // std::cout << std::format("dt: {}\n", dt);
        }

        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            mousePressed = true;
        }

        if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) {
            mousePressed = false;
        }

        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::S) {
            if (firstSpringParticle != nullptr && secondSpringParticle != nullptr) {
                springs.push_back(std::make_unique<Spring>(firstSpringParticle, secondSpringParticle));
                firstSpringParticle = nullptr;
                secondSpringParticle = nullptr;
            }
        }
    }

    if (ImGui::GetIO().WantCaptureMouse) {
        // ImGui is capturing mouse input, so skip additional processing
        mousePressed = false;
        return;
    }

    if (!placeMode && !springMode && mousePressed) {
        selectParticle(selectedParticle);
    }

    if (!placeMode && springMode && mousePressed) {
        if (firstSpringParticle && secondSpringParticle) return;

        if (!firstSpringParticle) {
            selectParticle(firstSpringParticle, [](PartPtr& particle) {});
        }

        if (firstSpringParticle != nullptr && secondSpringParticle == nullptr) {
            selectParticle(secondSpringParticle, [this](PartPtr& particle) {
                if (firstSpringParticle != nullptr) {
                    // firstSpringParticle->gCircle().setOutlineThickness(0);  // Reset outline thickness
                    firstSpringParticle->outlineThickness = 5;
                }
                if (particle != nullptr) {
                    // particle->gCircle().setOutlineThickness(0);  // Reset outline thickness
                    particle->outlineThickness = 0;
                }
                particle = nullptr;
                firstSpringParticle = nullptr;
            });
        }
    }

    if (placeMode && mousePressed) {
        auto mPos = sf::Mouse::getPosition(*win);
        addParticle({(float)mPos.x, (float)mPos.y}, newParticleVel, newParticleMass, newParticleColor);
        if (precisionPlacement) mousePressed = false;
    }
}

void Screen::quit() { running = false; }

void Screen::cleanupImGui() {
    auto& instance = Screen::Instance();
    if (instance->isInitialized && instance->win) {
        ImGui::SFML::Shutdown(*instance->win);
        instance->isInitialized = false;
    }
}

void Screen::clean() {
    if (win) {
        win->close();  // Then close the window
        win.reset();   // Finally reset the pointer
        cout << "Cleaned Screen\n";
    }
}
