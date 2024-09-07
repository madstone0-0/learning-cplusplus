#pragma once
#include <iostream>

#include "Backend.hpp"
#include "InputHandler.hpp"
#include "SDL_utils.hpp"
#include "Screen.hpp"

using namespace SDL_Utils;
using std::cout;

class SDL_Backend : public Backend<RendererPtr, WindowPtr> {
   public:
    bool init(const std::string& title, int xpos, int ypos, mlinalg::Vector2<int> dim, bool fullscreen) override {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) >= 0) {
            cout << "SDL Init success" << '\n';
            int flags = 0;
            if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;
            win = makeWindow(title.c_str(), xpos, ypos, dim.at(0), dim.at(1), flags);

            if (win != nullptr) {
                this->dim.at(0) = (int)dim.at(0);
                this->dim.at(1) = (int)dim.at(1);
                cout << "Window Creation success" << '\n';
                renderer = makeRenderer(win.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                if (renderer) {
                    cout << "Renderer Creation success" << '\n';

                } else {
                    cout << "Renderer Creation failed" << '\n';
                    return false;
                }
            } else {
                cout << "Window Creation failed" << '\n';
                return false;
            }
        } else {
            cout << "SDL Init failed" << '\n';
            return false;
        }
        cout << "Init success" << '\n';
        return true;
    }

    [[nodiscard]] const RendererPtr& getRenderer() const override { return renderer; }

    [[nodiscard]] const WindowPtr& getWindow() const override { return win; }

    mlinalg::Vector2<double> getDimens() override {
        int winx{};
        int winy{};
        SDL_GetRendererOutputSize(renderer.get(), &winx, &winy);
        dim.at(0) = winx;
        dim.at(1) = winy;
        return dim;
    }

    void beginRender() override {
        SDL_RenderClear(renderer.get());
        SDL_SetRenderDrawColor(renderer.get(), 0xE4, 0xE4, 0xE4, 255);
    }

    void render(const PartPtr& part) override { part->draw(renderer); }

    void endRender() override {
        SDL_SetRenderDrawColor(renderer.get(), 0xE4, 0xE4, 0xE4, 255);
        SDL_RenderPresent(renderer.get());
    }

    void handleEvent() override {
        if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE)) ParticleScreen::Instance()->quit();
    }

    void clean() override {
        renderer.reset();
        win.reset();
        SDL_Quit();
        std::cout << "Cleaned SDL Screen\n";
    }

    ~SDL_Backend() override { clean(); }

   private:
    WindowPtr win{nullptr, SDL_DestroyWindow};
    RendererPtr renderer{nullptr, SDL_DestroyRenderer};
    Vector2<double> dim;
};
