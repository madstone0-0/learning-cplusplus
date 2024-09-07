#pragma once
#include <MLinalg.hpp>
#include <functional>
#include <memory>

#include "Particle.hpp"

using Callback = void();
template <typename RendererType, typename WindowType>
struct Backend {
    virtual bool init(const std::string& title, int xpos, int ypos, mlinalg::Vector2<int> dim, bool fullscreen) = 0;

    virtual const RendererType& getRenderer() const = 0;
    virtual const WindowType& getWindow() const = 0;
    virtual mlinalg::Vector2<double> getDimens() = 0;
    virtual void handleEvent() = 0;

    virtual void beginRender() = 0;
    virtual void render(const PartPtr&) = 0;
    virtual void endRender() = 0;
    virtual void clean() = 0;
    virtual ~Backend() = default;
};

template <typename RendererType, typename WindowType>
using BackendPtr = std::unique_ptr<Backend<RendererType, WindowType>>;
