#pragma once
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_joystick.h>
#include <SDL_mixer.h>

#include <MLinalg.hpp>
#include <iostream>
#include <memory>
#include <string_view>
#include <utility>

using namespace mlinalg::structures;
namespace SDL_Utils {
    using std::shared_ptr, std::unique_ptr;
    using WindowPtr = unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
    using RendererPtr = unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;
    using TexturePtr = unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
    using SurfacePtr = unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;
    using JoystickPtr = unique_ptr<SDL_Joystick, decltype(&SDL_JoystickClose)>;
    using ChunkPtr = unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>;
    using MusicPtr = unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>;

    template <typename... Args>
    WindowPtr makeWindow(Args&&... args) {
        auto window = WindowPtr{SDL_CreateWindow(std::forward<Args>(args)...), SDL_DestroyWindow};
        return window;
    }

    template <typename... Args>
    RendererPtr makeRenderer(Args&&... args) {
        auto renderer = RendererPtr{SDL_CreateRenderer(std::forward<Args>(args)...), SDL_DestroyRenderer};
        return renderer;
    }

    template <typename... Args>
    TexturePtr makeTexture(Args&&... args) {
        return TexturePtr{SDL_CreateTexture(std::forward<Args>(args)...), SDL_DestroyTexture};
    }

    template <typename... Args>
    SurfacePtr loadBMPAsSurface(Args&&... args) {
        return SurfacePtr{SDL_LoadBMP(std::forward<Args>(args)...), SDL_FreeSurface};
    }

    template <typename... Args>
    SurfacePtr loadImageAsSurface(Args&&... args) {
        return SurfacePtr{IMG_Load(std::forward<Args>(args)...), SDL_FreeSurface};
    }

    template <typename... Args>
    TexturePtr makeBMPTextureFromSurface(const RendererPtr& renderer, const std::string& path, Args&&... args) {
        auto tempSur = loadBMPAsSurface(path.c_str());
        return TexturePtr{SDL_CreateTextureFromSurface(renderer.get(), tempSur.get(), std::forward<Args>(args)...),
                          SDL_DestroyTexture};
    }

    template <typename... Args>
    TexturePtr makeImgTextureFromSurface(const RendererPtr& renderer, const std::string& path, Args&&... args) {
        auto tempSur = loadImageAsSurface(path.c_str());
        return TexturePtr{SDL_CreateTextureFromSurface(renderer.get(), tempSur.get(), std::forward<Args>(args)...),
                          SDL_DestroyTexture};
    }

    inline JoystickPtr openJoystick(int index) { return JoystickPtr{SDL_JoystickOpen(index), SDL_JoystickClose}; }

    template <typename... Args>
    MusicPtr loadMusic(Args&&... args) {
        return MusicPtr{Mix_LoadMUS(std::forward<Args>(args)...), Mix_FreeMusic};
    }

    template <typename... Args>
    ChunkPtr loadChunk(Args&&... args) {
        return ChunkPtr{Mix_LoadWAV(std::forward<Args>(args)...), Mix_FreeChunk};
    }
}  // namespace SDL_Utils

namespace SDL_Utils::draw {
    struct Circle {
        Circle(const Vector2<int>& pos, int radius) : pos{pos}, radius{radius}, diameter{radius * 2} {}

        void draw(const RendererPtr& renderer) {
            auto x{radius - 1};
            auto y{0};
            auto tx{1};
            auto ty{1};
            auto error{tx - diameter};
            while (x >= y) {
                SDL_RenderDrawPoint(renderer.get(), pos.at(0) + x, pos.at(1) - y);
                SDL_RenderDrawPoint(renderer.get(), pos.at(0) + x, pos.at(1) + y);
                SDL_RenderDrawPoint(renderer.get(), pos.at(0) - x, pos.at(1) - y);
                SDL_RenderDrawPoint(renderer.get(), pos.at(0) - x, pos.at(1) + y);
                SDL_RenderDrawPoint(renderer.get(), pos.at(0) + y, pos.at(1) - x);
                SDL_RenderDrawPoint(renderer.get(), pos.at(0) + y, pos.at(1) + x);
                SDL_RenderDrawPoint(renderer.get(), pos.at(0) - y, pos.at(1) - x);
                SDL_RenderDrawPoint(renderer.get(), pos.at(0) - y, pos.at(1) + x);

                if (error <= 0) {
                    --y;
                    error += ty;
                    ty += 2;
                }

                if (error > 0) {
                    --x;
                    tx += 2;
                    error += (tx - diameter);
                }
            }
        }

        ~Circle() = default;
        Vector2<int> pos;
        int radius{};
        int diameter{};
    };
}  // namespace SDL_Utils::draw
