#pragma once
#include <SDL_pixels.h>

#include <SFML/Window/Event.hpp>
#include <cmath>
#include <iostream>
#include <random>

#include "InputHandler.hpp"
#include "Particle.hpp"
#include "SFML_utils.hpp"
#include "Structures.hpp"

using std::cout;
using namespace SDL_Utils;
using namespace SFML_utils;

class Screen {
   public:
    bool init(const std::string& title, Vector2<int> dim, bool fullscreen, double dt = 1);
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
    static std::unique_ptr<Screen> instance;
    static std::unique_ptr<Screen>& Instance();
    [[nodiscard]] bool isRunning() const { return running; }
    [[nodiscard]] const RenderWindowPtr& getWindow() const { return win; }
    [[nodiscard]] size_t getNumParticles() const { return numParticles; }
    ~Screen() { clean(); }

   private:
    void create();
    void destroy();
    void toggleFullscreen();
    void renderUI();
    void purgeParticles(int targetNumParticles);
    [[nodiscard]] Vector2<double> collisionResponse(const Vector2<double>& v, const Vector2<double>& n) const;

    Vector2<double> nL{1, 0};
    Vector2<double> nR{-1, 0};
    Vector2<double> nB{0, -1};
    Vector2<double> nT{0, 1};

    std::string title;
    void addRandomParticle();
    Screen() = default;
    RenderWindowPtr win{new sf::RenderWindow{}};
    bool running{};
    std::vector<PartPtr> particles;
    size_t numParticles{};
    double dt{};
    sf::Clock dT;
    double e{0.5};
    bool fullscreen{};
    bool mousePressed{false};
    int targetNumParticles{};
    Vector2<int> dim;
    Vector2<double> gravity{0, 0};
    Vector2<double> newParticleVel{0, 0};
    float floatArr[4] = {0.0F, 0.0F, 0.0F, 1.0F};
    sf::Color newParticleColor{0x00, 0x00, 0x00, 0xFF};
    int newParticleMass{5};
};

using ScreenPtr = std::unique_ptr<Screen>;
using ParticleScreen = Screen;
