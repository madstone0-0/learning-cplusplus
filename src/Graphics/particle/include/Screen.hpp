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
#include "Physics.hpp"
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
    static void cleanupImGui();
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
    Screen(const Screen&) = delete;
    Screen& operator=(const Screen&) = delete;

   private:
    // Class methods
    bool isInitialized{false};
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

    // Physics
    PhysicsType physics{new SimplePhysics(0.0, particles, dim)};

    // Particle methods
    void addRandomParticle();
    void purgeParticles(int targetNumParticles);
    void cullOutOfBoundsParticles();
    void selectParticle(
        PartPtr& selection, const Callback& endCallback = [](PartPtr& particle) {
            if (particle != nullptr) particle->outlineThickness = 0;
            particle = nullptr;
        });

    // Particle members
    std::vector<PartPtr> particles;
    std::vector<SpringPtr> springs;
    size_t numParticles{};

    sf::CircleShape particleCircle;

    PartPtr selectedParticle{nullptr};

    PartPtr firstSpringParticle{nullptr};
    PartPtr secondSpringParticle{nullptr};

    sf::Clock dT;  // Clock for measuring time step
    bool mousePressed{false};
    int targetNumParticles{};

    // ImGui members
    Vector2<double> newParticleVel{0, 0};
    float floatArr[4] = {0.0F, 0.0F, 0.0F, 1.0F};  // RGBA
    sf::Color newParticleColor{0x00, 0x00, 0x00, 0xFF};
    bool placeMode{true};
    int newParticleMass{5};
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
