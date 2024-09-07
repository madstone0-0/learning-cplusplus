#pragma once
#include <SDL2/SDL.h>

#include <algorithm>
#include <boost/container/vector.hpp>
#include <format>
#include <iostream>
#include <memory>
#include <ranges>
#include <vector>

#include "SDL_utils.hpp"

class InputHandler;

using V2Ptr = std::unique_ptr<Vector2<double>>;
using InputHandlerPtr = std::unique_ptr<InputHandler>;
namespace rg = std::ranges;

enum MOUSE_BUTTONS { LEFT = 0, MIDDLE, RIGHT };

using namespace SDL_Utils;
class InputHandler {
   public:
    static InputHandlerPtr& Instance();
    void update();
    void clean();
    ~InputHandler() = default;
    [[nodiscard]] bool mBState(MOUSE_BUTTONS mouseButton) const;
    [[nodiscard]] const V2Ptr& mPos() const;
    bool isKeyDown(SDL_Scancode key);

   private:
    InputHandler();
    void onKeyUp();
    void onKeyDown();

    void onMouseMove(SDL_Event& e);
    void onMouseButtonDown(SDL_Event& e);
    void onMouseButtonUp(SDL_Event& e);

    void handleMouse(SDL_Event& e);

    static InputHandlerPtr instance;
    std::vector<boost::container::vector<bool>> buttonStates;
    boost::container::vector<bool> mouseButtonStates;
    V2Ptr mousePos{new Vector2<double>{0, 0}};
    const Uint8* keyState;
};

using TheInputHandler = InputHandler;
