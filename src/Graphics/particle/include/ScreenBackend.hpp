#pragma once
#include <SDL_pixels.h>

#include <cmath>
#include <iostream>
#include <random>

#include "Backend.hpp"
#include "InputHandler.hpp"
#include "Particle.hpp"
// #include "SDL_Backend.hpp"
// #include "SFML_Backend.hpp"
#include "SFML_utils.hpp"

using std::cout;
using namespace SDL_Utils;
using namespace SFML_utils;

class Screen {
   public:
    bool init(const std::string& title, int xpos, int ypos, Vector2<int> dim, bool fullscreen, double dt = 1);
    void update();
    void render();
    void handleEvents();
    void clean();
    void quit();
    void addParticle(const Vector2<double>& pos, const Vector2<double>& vel, int mass, SDL_Color color);
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
    // [[nodiscard]] const RendererPtr& getRenderer() const { return backend->getRenderer(); }
    // [[nodiscard]] const WindowPtr& getWindow() const { return backend->getWindow(); }
    [[nodiscard]] const RenderWindowPtr& getRenderer() const { return backend->getRenderer(); }
    [[nodiscard]] const RenderWindowPtr& getWindow() const { return backend->getWindow(); }
    [[nodiscard]] size_t getNumParticles() const { return numParticles; }
    ~Screen() { clean(); }

   private:
    void addRandomParticle();
    Screen() = default;
    // Screen(BackendPtr<RenderWindowPtr, RenderWindowPtr>&& backend) : backend{std::move(backend)} {}
    // Screen(BackendPtr<RendererPtr, WindowPtr>&& backend) : backend{std::move(backend)} {}
    bool running{};
    std::vector<PartPtr> particles;
    size_t numParticles{};
    double dt{};
    // BackendPtr<RendererPtr, WindowPtr> backend{std::make_unique<SDL_Backend>()};
    // BackendPtr<RenderWindowPtr, RenderWindowPtr> backend{std::make_unique<SFML_Backend>()};
    static BackendPtr<RenderWindowPtr, RenderWindowPtr> backend;
};

using ScreenPtr = std::unique_ptr<Screen>;
using ParticleScreen = Screen;
