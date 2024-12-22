#pragma once

#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <cmath>
#include <functional>
#include <iostream>
#include <mlinalg/MLinalg.hpp>
#include <random>

#include "InputHandler.hpp"
#include "Particle.hpp"
#include "SFML_utils.hpp"
#include "Spring.hpp"
#include "imgui.h"

using std::cout;
using namespace SDL_Utils;
using namespace SFML_utils;

using ParticleGroups = std::vector<std::vector<PartPtr>>;
using Callback = std::function<void(PartPtr&)>;

class Screen {
   public:
    bool init(const std::string& title, Vector2<int> dim, bool fullscreen, double dt = 1, uint framerate = 60);
    void update();
    void render();
    void handleEvents();
    void clean();
    void quit();
    void addParticle(const Vector2<double>& pos, const Vector2<double>& vel, int mass, sf::Color color);
    void removeParticle(const PartPtr& particle);

    template <typename Itr>
    void removeParticle(Itr itr) {
        particles.erase(itr);
        numParticles--;
    }

    void addParticle(const Vector2<double>& pos, const Vector2<double>& vel, int mass);
    void addSpringBetweenParticles(const PartPtr& p1, const PartPtr& p2);
    static std::unique_ptr<Screen> instance;
    static std::unique_ptr<Screen>& Instance();
    [[nodiscard]] bool isRunning() const { return running; }
    [[nodiscard]] const RenderWindowPtr& getWindow() const { return win; }
    [[nodiscard]] size_t getNumParticles() const { return numParticles; }
    sf::Time getElapsed() { return elapsed; }
    void restartClock() { elapsed = clock.restart(); }
    ~Screen() { clean(); }

   private:
    // Class methods
    Screen() = default;
    void create();
    void destroy();
    void toggleFullscreen();
    void renderUI();

    // Class members
    sf::Clock clock;
    sf::Time elapsed;
    std::string title;
    RenderWindowPtr win{new sf::RenderWindow{}};
    bool fullscreen{};
    Vector2<int> dim;
    bool running{};
    sf::RectangleShape border;

    // Particle methods
    void addRandomParticle();
    void purgeParticles(int targetNumParticles);
    [[nodiscard]] Vector2<double> collisionResponse(const Vector2<double>& v, const Vector2<double>& n) const;
    void handleBoxCollision(const PartPtr& particle);
    void handleParticleCollision();
    void handleParticleCollision(const PartPtr& particle, const std::vector<PartPtr>& particles);
    void cullOutOfBoundsParticles();
    void selectParticle(
        PartPtr& selection, const Callback& endCallback = [](PartPtr& particle) {
            if (particle != nullptr) particle->outlineThickness = 0;
            particle = nullptr;
        });

    // Broad-phase collision detection methods
    std::vector<PartPtr> sweepAndPrune(const PartPtr& particle, bool xAxis = true);
    ParticleGroups sweepAndPrune(bool xAxis = true);

    ParticleGroups uniSpacePartitioning(size_t gridSpace = 10);
    ParticleGroups kdTree(bool xAxis = false);
    void buildKdTree(ParticleGroups& groups, std::vector<PartPtr>& particles, bool xAxis);

    // Particle members
    std::vector<PartPtr> particles;
    std::vector<SpringPtr> springs;
    Vector2<double> nL{1, 0};   // Normal vector for left wall
    Vector2<double> nR{-1, 0};  // Normal vector for right wall
    Vector2<double> nB{0, -1};  // Normal vector for bottom wall
    Vector2<double> nT{0, 1};   // Normal vector for top wall
    size_t numParticles{};

    sf::CircleShape particleCircle;

    PartPtr selectedParticle{nullptr};

    PartPtr firstSpringParticle{nullptr};
    PartPtr secondSpringParticle{nullptr};

    double dt{};    // Time step
    sf::Clock dT;   // Clock for measuring time step
    double e{0.5};  // Elasticity
    bool mousePressed{false};
    int targetNumParticles{};
    Vector2<double> gravity{0, 0};  // Global gravity

    // ImGui members
    Vector2<double> newParticleVel{0, 0};
    float floatArr[4] = {0.0F, 0.0F, 0.0F, 1.0F};  // RGBA
    sf::Color newParticleColor{0x00, 0x00, 0x00, 0xFF};
    bool placeMode{true};
    int newParticleMass{5};
    double frictionCoef{0.3};
    float borderXMin;
    float borderYMin;
    float borderXMax;
    float borderYMax;
    bool cull{};
    int borderThickness{10};
    bool borderMatchDimens{true};
    bool precisionPlacement{};
    bool springMode{};
};

using ScreenPtr = std::unique_ptr<Screen>;
using ParticleScreen = Screen;
