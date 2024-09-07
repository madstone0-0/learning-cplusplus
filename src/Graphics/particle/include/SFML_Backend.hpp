#pragma once
#include <imgui-SFML.h>
#include <imgui.h>

#include <MLinalg.hpp>
#include <SFML/Graphics.hpp>
#include <string>

#include "Backend.hpp"
#include "SFML_utils.hpp"
#include "Screen.hpp"

using namespace mlinalg;
using namespace SFML_utils;

class SFML_Backend : public Backend<RenderWindowPtr, RenderWindowPtr> {
   public:
    bool init(const std::string& title, int xpos, int ypos, mlinalg::Vector2<int> dim, bool fullscreen) override {
        win->setFramerateLimit(60);
        this->title = title;
        this->fullscreen = fullscreen;
        this->dim.at(0) = dim.at(0);
        this->dim.at(1) = dim.at(1);
        create();
        ImGui::SFML::Init(*win);
        return true;
    }

    [[nodiscard]] const RenderWindowPtr& getRenderer() const override { return win; }

    [[nodiscard]] const RenderWindowPtr& getWindow() const override { return win; }

    mlinalg::Vector2<double> getDimens() override {
        const auto [x, y] = win->getSize();
        dim.at(0) = x;
        dim.at(1) = y;
        return dim;
    }

    void beginRender() override {
        ImGui::SFML::Update(*win, dt.restart());
        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();
        win->clear({0xE4, 0xE4, 0xE4, 255});
    }

    void render(const PartPtr& part) override { win->draw(part->draw()); }

    void endRender() override {
        ImGui::SFML::Render(*win);
        win->display();
    }

    void handleEvent() override {
        sf::Event e{};
        while (win->pollEvent(e)) {
            // ImGui::SFML::ProcessEvent(*win, e);
            if (e.type == sf::Event::Closed)
                ParticleScreen::Instance()->quit();
            else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F11)
                toggleFullscreen();
        }
    }

    void clean() override {
        if (win && win->isOpen()) {
            win->close();
            ImGui::SFML::Shutdown();
            win.reset();
            std::cout << "Cleaned SFML Screen\n";
        }
    }

    ~SFML_Backend() override {
        if (win && win->isOpen()) {
            clean();
        }
    }

   private:
    void create() {
        auto style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;
        win->create(
            {
                (uint)dim.at(0),
                (uint)dim.at(1),
                32,
            },
            title, style);
    }

    void destroy() { win->close(); }

    void toggleFullscreen() {
        fullscreen = !fullscreen;
        destroy();
        create();
    }

    bool fullscreen;
    std::string title;
    sf::Clock dt;
    RenderWindowPtr win{new sf::RenderWindow{}};
    Vector2<double> dim;
};
