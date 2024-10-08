#include "SdlInputManagement.h"

namespace Input
{
    SdlInputManagement::SdlInputManagement() 
    {
        curMouseState.stateChanged = false;
    }



    bool SdlInputManagement::getKeyPressed(const SDL_Scancode &key)
    {
        return keyState[key] == true && prevKeyState[key] == false;
    }

    bool SdlInputManagement::getKeyHeld(const SDL_Scancode &key)
    {
        return keyState[key] == true;
    }

    void SdlInputManagement::updateInputState()
    {
        memcpy(prevKeyState, keyState, sizeof(uint32_t) * SDL_Scancode::SDL_NUM_SCANCODES);

        curMouseState.stateChanged = false;
        curMouseState.scrollWheelChange = false;
        curMouseState.scrollWheelChange = false;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EventType::SDL_KEYDOWN:
                keyState[(int)event.key.keysym.scancode] = 1;
                break;
            case SDL_EventType::SDL_KEYUP:
                keyState[(int)event.key.keysym.scancode] = 0;
                break;
            case SDL_EventType::SDL_MOUSEMOTION:
                curMouseState.stateChanged = true;
                curMouseState.mousePositionChange = true;
                curMouseState.cursor_x = event.motion.xrel;
                curMouseState.cursor_y = event.motion.yrel;
                break;
            case SDL_EventType::SDL_MOUSEWHEEL:
                curMouseState.stateChanged = true;
                curMouseState.scrollWheelChange = true;
                curMouseState.scrllWheel_y = event.wheel.y;

            default:
                break;
            }
        }
    }

    const MouseState* SdlInputManagement::getMouseUdpated()
    {
        return curMouseState.stateChanged ? &curMouseState : nullptr;
    }
}