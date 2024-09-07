#include "InputHandler.hpp"

#include <SDL_mouse.h>
#include"Screen.hpp"

#include <algorithm>
#include <iterator>

#include "SDL_utils.hpp"

InputHandlerPtr InputHandler::instance{};

InputHandlerPtr& InputHandler::Instance() {
    if (!instance) instance = InputHandlerPtr{new InputHandler{}};
    return instance;
}

void InputHandler::clean() { std::cout << "Cleaned InputHandler\n"; }

void InputHandler::onKeyUp() { keyState = SDL_GetKeyboardState(nullptr); }

void InputHandler::onKeyDown() { keyState = SDL_GetKeyboardState(nullptr); }

void InputHandler::onMouseMove(SDL_Event& e) {
    mousePos->at(0) = e.motion.x;
    mousePos->at(1) = e.motion.y;
}

void InputHandler::onMouseButtonDown(SDL_Event& e) {
    switch (e.button.button) {
        case SDL_BUTTON_LEFT:
            mouseButtonStates.at(MOUSE_BUTTONS::LEFT) = true;
            break;
        case SDL_BUTTON_MIDDLE:
            mouseButtonStates.at(MOUSE_BUTTONS::MIDDLE) = true;
            break;
        case SDL_BUTTON_RIGHT:
            mouseButtonStates.at(MOUSE_BUTTONS::RIGHT) = true;
            break;
    }
}

void InputHandler::onMouseButtonUp(SDL_Event& e) {
    switch (e.button.button) {
        case SDL_BUTTON_LEFT:
            mouseButtonStates.at(MOUSE_BUTTONS::LEFT) = false;
            break;
        case SDL_BUTTON_MIDDLE:
            mouseButtonStates.at(MOUSE_BUTTONS::MIDDLE) = false;
            break;
        case SDL_BUTTON_RIGHT:
            mouseButtonStates.at(MOUSE_BUTTONS::RIGHT) = false;
            break;
    }
}

bool InputHandler::isKeyDown(SDL_Scancode key) {
    if (keyState != nullptr) {
        return keyState[key] == 1;
    }
    return false;
}

[[nodiscard]] bool InputHandler::mBState(MOUSE_BUTTONS mouseButton) const { return mouseButtonStates.at(mouseButton); }

void InputHandler::handleMouse(SDL_Event& e) {
    switch (e.type) {
        case SDL_MOUSEBUTTONDOWN:
            onMouseButtonDown(e);
            break;
        case SDL_MOUSEBUTTONUP:
            onMouseButtonUp(e);
            break;
        case SDL_MOUSEMOTION:
            onMouseMove(e);
            break;
        default:
            break;
    }
}

[[nodiscard]] const V2Ptr& InputHandler::mPos() const { return mousePos; }

InputHandler::InputHandler() : mouseButtonStates(3, false), keyState{SDL_GetKeyboardState(nullptr)} {}

void InputHandler::update() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type) {
            case SDL_QUIT:
                ParticleScreen::Instance()->quit();
                break;
            case SDL_KEYDOWN:
                onKeyDown();
                break;
            case SDL_KEYUP:
                onKeyUp();
                break;
            default:
                break;
        }
        handleMouse(e);
    }
}
