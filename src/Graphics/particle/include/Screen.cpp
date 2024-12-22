#include "Screen.hpp"

#include <imgui.h>

ScreenPtr Screen::instance{};

ScreenPtr& Screen::Instance() {
    // if (!instance) instance = ScreenPtr{new Screen{std::make_unique<SFML_Backend>()}};
    if (!instance) instance = ScreenPtr{new Screen{}};
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
    this->dt = dt;
    create();
    running = true;
    cout << std::format("Created window with FPS: {} and Dimensions:\n{}", framerate, std::string{dim});
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
    // velocity after collision = velocity - (1 + elasticity) * normal * (velocity dot normal)
    // This inverts the velocity component that is perpendicular to the normal
    // Taking into account friction velocity after collision is now
    // velocity - (1 + elasticity) * normal velocity - friction coefficient * tangential velocity * time step
    // Where normal velocity = normal * (velocity * normal) and
    // tangential velocity = velocity - normal velocity
    auto vn = n * (v * n).at(0);
    auto vt = v - vn;
    return v - (1 + e) * vn - frictionCoef * vt * dt;
}

void Screen::handleBoxCollision(const PartPtr& particle) {
    const auto& pos = particle->gPos();
    const auto& vel = particle->gVel();
    const auto& radius = particle->gRadius();

    // Left wall collision
    if (pos.at(0) - radius <= borderXMin) {
        if ((vel * nL).at(0) < 0) {  // Check if the object is moving towards the wall
            auto newVel = collisionResponse(vel, nL);
            particle->sVel(newVel);

            auto depth = -(pos.at(0) - radius);
            particle->sPos(pos + nL * depth);
        }
    }

    // Right wall collision
    if (pos.at(0) + radius >= borderXMax) {
        if ((vel * nR).at(0) < 0) {  // Check if the object is moving towards the wall
            auto newVel = collisionResponse(vel, nR);
            particle->sVel(newVel);

            auto depth = (pos.at(0) - radius) - borderXMax + radius * 2;
            particle->sPos(pos + nR * depth);
        }
    }

    // Bottom wall collision
    if (pos.at(1) + radius >= borderYMax) {
        if ((vel * nB).at(0) < 0) {  // Check if the object is moving towards the wall
            auto newVel = collisionResponse(vel, nB);
            particle->sVel(newVel);

            auto depth = (pos.at(1) - radius) - borderYMax + radius * 2;
            particle->sPos(pos + nB * depth);
        }
    }

    // Top wall collision
    if (pos.at(1) - radius <= borderYMin) {
        if ((vel * nT).at(0) < 0) {  // Check if the object is moving towards the wall
            auto newVel = collisionResponse(vel, nT);
            particle->sVel(newVel);

            auto depth = -(pos.at(1) - radius);
            particle->sPos(pos + nT * depth);
        }
    }
}

std::vector<PartPtr> Screen::sweepAndPrune(const PartPtr& particle, bool xAxis) {
    std::vector<PartPtr> activeIntervals;
    const auto& pos = particle->gPos();
    const auto& radius = particle->gRadius();
    activeIntervals.reserve(particles.size());
    activeIntervals.push_back(particle);
    for (const auto& otherParticle : particles) {
        if (particle == otherParticle) continue;

        const auto& otherPos = otherParticle->gPos();
        const auto& otherRadius = otherParticle->gRadius();

        int axisIndex = xAxis ? 0 : 1;
        if (pos.at(axisIndex) + radius >= otherPos.at(axisIndex) - otherRadius) {
            activeIntervals.push_back(otherParticle);
        } else {
            // Since particles are sorted, we can break early once we find a non-overlapping interval
            break;
        }
    }
    return activeIntervals;
}

std::vector<std::vector<PartPtr>> Screen::sweepAndPrune(bool xAxis) {
    std::vector<std::vector<PartPtr>> res;
    std::sort(particles.begin(), particles.end(), [&xAxis](const PartPtr& a, const PartPtr& b) {
        if (xAxis) {
            return a->gPos().at(0) < b->gPos().at(0);
        } else {
            return a->gPos().at(1) < b->gPos().at(1);
        }
    });

    std::vector<PartPtr> activeIntervals;
    for (const auto& particle : particles) {
        const auto& pos = particle->gPos();
        const auto& radius = particle->gRadius();
        activeIntervals.erase(
            std::remove_if(activeIntervals.begin(), activeIntervals.end(),
                           [&](const PartPtr& p) {
                               const auto& otherPos = p->gPos();
                               const auto& otherRadius = p->gRadius();
                               int axisIndex = xAxis ? 0 : 1;
                               return (pos.at(axisIndex) - radius > otherPos.at(axisIndex) + otherRadius);
                           }),
            activeIntervals.end());
        activeIntervals.push_back(particle);
        if (activeIntervals.size() > 1) res.push_back(activeIntervals);
    }

    return res;
}

std::vector<std::vector<PartPtr>> Screen::uniSpacePartitioning(size_t gridSpace) {
    // Make grid from screen
    vector<vector<PartPtr>> res;
    using std::vector;
    auto rows = (size_t)std::ceil(dim.at(1) / (double)gridSpace);
    auto cols = (size_t)std::ceil(dim.at(0) / (double)gridSpace);
    // cout << std::format("Rows: {}, Cols: {}\n", rows, cols);
    vector<vector<vector<PartPtr>>> grid(rows, vector<vector<PartPtr>>(cols, vector<PartPtr>{}));

    // Assign particles to cells
    for (const auto& particle : particles) {
        const auto& pos = particle->gPos();
        auto row = std::min((size_t)pos.at(1) / gridSpace, rows - 1);
        auto col = std::min((size_t)pos.at(0) / gridSpace, cols - 1);
        grid[row][col].push_back(particle);
    }

    // Only particles in the same cells should be checked for collisions
    res.reserve(rows * cols);
    for (size_t i{}; i < rows; i++) {
        for (size_t j{}; j < cols; j++) {
            if (grid[i][j].size() > 1) res.push_back(grid[i][j]);
        }
    }
    return res;
}

void Screen::buildKdTree(ParticleGroups& groups, std::vector<PartPtr>& particles, bool xAxis) {
    if (particles.size() <= 20) {
        if (particles.size() > 1) groups.push_back(particles);
        return;
    }

    // Find the median index
    size_t median = particles.size() / 2;

    std::nth_element(
        particles.begin(), particles.begin() + median, particles.end(), [xAxis](const PartPtr& a, const PartPtr& b) {
            return (xAxis ? a->gPos().at(0) : a->gPos().at(1)) < (xAxis ? b->gPos().at(0) : b->gPos().at(1));
        });

    // Split into left and right subgroups
    std::vector<PartPtr> left(particles.begin(), particles.begin() + median);
    std::vector<PartPtr> right(particles.begin() + median + 1, particles.end());

    // Recurse on left and right groups along the next axis
    buildKdTree(groups, left, !xAxis);
    buildKdTree(groups, right, !xAxis);
}

ParticleGroups Screen::kdTree(bool xAxis) {
    std::vector<std::vector<PartPtr>> res;
    buildKdTree(res, particles, false);
    return res;
}

void Screen::handleParticleCollision() {
    // auto particleGroups = sweepAndPrune();
    auto particleGroups = uniSpacePartitioning();
    // auto particleGroups = kdTree();

    for (const auto& particles : particleGroups) {
        handleParticleCollision(particles.at(0), particles);
    }
}

void Screen::handleParticleCollision(const PartPtr& particle, const std::vector<PartPtr>& particles) {
    for (const auto& otherParticle : particles) {
        if (otherParticle == particle) continue;
        const auto epsilon{1e-5};
        const auto epsilonVector{epsilon * Vector2<double>{1, 1}};

        const auto& pos = particle->gPos();
        const auto& otherPos = otherParticle->gPos();
        const auto& radius = particle->gRadius();
        const auto& otherRadius = otherParticle->gRadius();

        const auto& vel = particle->gVel();
        const auto& otherVel = otherParticle->gVel();
        const auto& mass = particle->gMass();
        const auto& otherMass = otherParticle->gMass();

        if (pos.dist(otherPos) <= radius + otherRadius) {
            // Collision
            // auto posDiff1{otherPos - pos};
            // auto posDiff2{pos - otherPos};
            // auto n1 = posDiff1 / posDiff1.length();
            // auto n2 = posDiff2 / posDiff2.length();
            // auto dV = otherVel - vel;
            // auto vn1 = n1 * (dV * n1).at(0);
            // auto vn2 = n2 * (dV * n2).at(0);
            // auto vt1 = vel - vn1;
            // auto vt2 = otherVel - vn2;
            // auto d1 = 2 * radius - posDiff1.length();
            // auto d2 = 2 * otherRadius - posDiff2.length();
            //
            // auto newVel = vel + vn1 * (1 + e) / 2 + frictionCoef * vt1 * dt / 2;
            // auto p1 = pos - n1 * d1 / 2;
            //
            // auto newVelOther = otherVel - vn2 * (1 + e) / 2 - frictionCoef * vt2 * dt / 2;
            // auto p2 = otherPos - n2 * d2 / 2;
            //
            // particle->sVel(newVel);
            // particle->sPos(p1);
            // otherParticle->sVel(newVelOther);
            // otherParticle->sPos(p2);

            auto posDiff1{otherPos - pos};
            auto posDiff2{pos - otherPos};
            auto n1 = posDiff1 / (posDiff1.length() + epsilon);
            auto n2 = posDiff2 / (posDiff2.length() + epsilon);
            auto d1 = 2 * radius - posDiff1.length();
            auto d2 = 2 * otherRadius - posDiff2.length();

            auto massComponent = (2 * otherMass) / ((mass + otherMass) + epsilon);
            auto centerDiff = pos - otherPos;
            auto vCInner = (vel - otherVel) * centerDiff;
            auto centerDiffNorm = centerDiff.length();
            auto newVel = vel - static_cast<double>(massComponent) *
                                    (vCInner / ((centerDiffNorm * centerDiffNorm) + epsilon)).at(0) * centerDiff;
            auto p1 = pos - n1 * d1 / 2;

            auto massComponentOther = (2 * mass) / ((mass + otherMass) + epsilon);
            auto centerDiffOther = otherPos - pos;
            auto vCInnerOther = (otherVel - vel) * centerDiffOther;
            auto centerDiffNormOther = centerDiffOther.length();
            auto newVelOther =
                otherVel - static_cast<double>(massComponentOther) *
                               (vCInnerOther / ((centerDiffNormOther * centerDiffNormOther) + epsilon)).at(0) *
                               centerDiffOther;
            auto p2 = otherPos - n2 * d2 / 2;

            auto velDiff1 = newVel - newVelOther;
            auto normComp1 = (n1 * (velDiff1)).at(0) * n1;
            auto tanComp1 = vel - normComp1;
            auto tanVelWF1 = tanComp1 * (1 - frictionCoef);
            auto newVelWF = normComp1 + tanVelWF1;

            auto velDiff2 = newVelOther - newVel;
            auto normComp2 = (n2 * (velDiff2)).at(0) * n2;
            auto tanComp2 = vel - normComp2;
            auto tanVelWF2 = tanComp2 * (1 - frictionCoef);
            auto newVelOtherWF = normComp2 + tanVelWF2;

            // particle->sVel(newVel);
            particle->sVel(newVelWF);
            particle->sPos(p1);
            otherParticle->sVel(newVelOtherWF);
            // otherParticle->sVel(newVelOther);
            otherParticle->sPos(p2);
        }
    }
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
        handleBoxCollision(part);

        part->update(dt);
    }
    handleParticleCollision();

    for (auto& spring : springs) spring->update();

    cullOutOfBoundsParticles();

    const auto& [x, y] = win->getSize();
    dim.at(0) = (int)x - 10;
    dim.at(1) = (int)y - 10;
    if (borderMatchDimens) {
        borderXMax = dim.at(0);
        borderYMax = dim.at(1);
        border.setSize({borderXMax, borderYMax});
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
    ImGui::Text("dt: %f", dt);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::InputDouble("dt", &dt, dt, 0.1, "%.2f");

    ImGui::Checkbox("Cull Out of Bounds particles", &cull);

    if (ImGui::Checkbox("Place Mode", &placeMode)) {
        springMode = false;
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
    ImGui::InputDouble("Friction Coefficient", &frictionCoef, 0.1, 10.0, "%.2f");

    // Change gravity
    ImGui::InputDouble("Gravity X", &gravity.at(0), 0.1, 1.0, "%.2f");
    ImGui::InputDouble("Gravity Y", &gravity.at(1), 0.1, 1.0, "%.2f");

    // Change elasticity
    ImGui::InputDouble("Elasticity", &e, 0.1, 1.0, "%.2f");

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

void Screen::clean() {
    if (win) {
        // Close the window before shutdown
        win->close();

        // Shutdown ImGui for this specific window
        ImGui::SFML::Shutdown(*win);

        // Let the unique_ptr handle deletion
        win.reset();

        cout << "Cleaned Screen\n";
    }
}
