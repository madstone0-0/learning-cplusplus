#include <SFML/System/Sleep.hpp>
#include <mlinalg/MLinalg.hpp>

#include "Screen.hpp"
#include "imgui-SFML.h"
#define SMOOTHING_ON 1
using namespace SDL_Utils;
using namespace mlinalg::structures;

const int WINX{1366};
const int WINY{768};

const int FPS = 60;
const int DELAY_TIME = static_cast<int>(1000 / FPS);

int main() {
    try {
        if (!ParticleScreen::Instance()->init("Simulation", {WINX, WINY}, false, 1. / FPS, FPS)) return 1;

        while (ParticleScreen::Instance()->isRunning()) {
            ParticleScreen::Instance()->restartClock();
            ParticleScreen::Instance()->handleEvents();
            ParticleScreen::Instance()->update();
            ParticleScreen::Instance()->render();

            if (ParticleScreen::Instance()->getElapsed().asMilliseconds() < DELAY_TIME) {
                sf::sleep(sf::milliseconds(DELAY_TIME - ParticleScreen::Instance()->getElapsed().asMilliseconds()));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
