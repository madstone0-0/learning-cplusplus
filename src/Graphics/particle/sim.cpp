#include <MLinalg.hpp>
#include <SFML/System/Sleep.hpp>

#include "Screen.hpp"
#include "imgui-SFML.h"
#define SMOOTHING_ON 1
using namespace SDL_Utils;
using namespace mlinalg::structures;

const int WINX{640};
const int WINY{480};

const int FPS = 60;
const int DELAY_TIME = static_cast<int>(1000.0F / FPS);

int main() {
    sf::Clock frameClock;

    try {
        if (!ParticleScreen::Instance()->init("Simulation", {WINX, WINY}, false, 1. / FPS)) return 1;

        while (ParticleScreen::Instance()->isRunning()) {
            frameClock.restart();
            ParticleScreen::Instance()->handleEvents();
            ParticleScreen::Instance()->update();
            ParticleScreen::Instance()->render();

            frameClock.restart();
            // cout << std::format("Particles: {}\n", ParticleScreen::Instance()->getNumParticles());

            if (frameClock.getElapsedTime().asMilliseconds() < DELAY_TIME) {
                sf::sleep(sf::milliseconds(DELAY_TIME - frameClock.getElapsedTime().asMilliseconds()));
            }
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
