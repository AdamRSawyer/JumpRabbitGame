#ifndef SDL_INPUT_MANAGEMENT_H
#define SDL_INPUT_MANAGEMENT_H

#ifdef WINDOWS
#define SDL_MAIN_HANDLED
#include <windows.h>
#endif

#include <SDL2/SDL.h>

#include <vector>
#include <memory>

namespace Input
{

struct MouseState
{
    bool stateChanged;
    bool mousePositionChange;
    bool scrollWheelChange;
    int cursor_x, cursor_y;
    int scrllWheel_y;
};

// Manages grabbing SDL input events and the current state of all input devices
class SdlInputManagement
{
    public:
    SdlInputManagement();
    bool getKeyPressed(const SDL_Scancode &key);
    bool getKeyHeld(const SDL_Scancode &key);
    
    // Returns a mouse state struct if updated. nullptr otherwise
    const MouseState* getMouseUdpated();
    void updateInputState();

    private:
    int initKeyboardState();
    
    // Index with the integer value of the SDL_Scancode enum
    uint32_t keyState[SDL_Scancode::SDL_NUM_SCANCODES] = {0}; // Holds a 0 if the key is not being pressed and a positive number indicating how long the key has been pressed otherwise
    uint32_t prevKeyState[SDL_Scancode::SDL_NUM_SCANCODES] = {0};
    int numKeys;

    MouseState curMouseState;
};

}
#endif