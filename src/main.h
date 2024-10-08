#ifndef MAIN_H
#define MAIN_H

#define WINDOWS

#include <cstdio>
#include <stdint.h>
#include <vector>

#ifdef WINDOWS
#define SDL_MAIN_HANDLED
#include <windows.h>
#endif

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "render/Renderer.h"

#include "game/GameDefs.h"
#include "game/Game.h"

#include "input/SdlInputManagement.h"

const GameEvent sdlEvent_to_gameEvent(const SDL_Event &sdlEvent);
void sdlKeyboardScancodeToEventMap(std::vector<GameEvent> &gameEvents, Input::SdlInputManagement &inputMngr);
void grabEvents(std::vector<GameEvent> &gameEvents);
int close_game(const int& returnStatus);

int close_game(const int& returnStatus)
{
    SDL_Quit();

    return returnStatus;
}



// Clears any previously logged events and populates vector iwth new ones
void grabEvents(std::vector<GameEvent> &gameEvents, Input::SdlInputManagement &inputMngr) // Take SDL_Events and transform them into GameEvents
{
    gameEvents.clear();
    inputMngr.updateInputState();
    sdlKeyboardScancodeToEventMap(gameEvents, inputMngr);
    
}

void sdlKeyboardScancodeToEventMap(std::vector<GameEvent> &gameEvents, Input::SdlInputManagement &inputMngr)
{
    if (inputMngr.getKeyPressed(SDL_Scancode::SDL_SCANCODE_ESCAPE))
    {
        gameEvents.push_back(GameEvent(GameEventType::GAME_EXIT));
    }

    // Movement
    //======================================================//
    if (inputMngr.getKeyHeld(SDL_Scancode::SDL_SCANCODE_W))
    {
        gameEvents.push_back(GameEvent(GameEventType::MOVE_FORWARD));
    }

    if (inputMngr.getKeyHeld(SDL_Scancode::SDL_SCANCODE_S))
    {
        gameEvents.push_back(GameEvent(GameEventType::MOVE_BACKWARD));
    }

    if (inputMngr.getKeyHeld(SDL_Scancode::SDL_SCANCODE_A))
    {
        gameEvents.push_back(GameEvent(GameEventType::MOVE_LEFT));
    }

    if (inputMngr.getKeyHeld(SDL_Scancode::SDL_SCANCODE_D))
    {
        gameEvents.push_back(GameEvent(GameEventType::MOVE_RIGHT));
    }

    if (inputMngr.getKeyHeld(SDL_Scancode::SDL_SCANCODE_LCTRL))
    {
        gameEvents.push_back(GameEvent(GameEventType::MOVE_DOWN));
    }

    if (inputMngr.getKeyHeld(SDL_Scancode::SDL_SCANCODE_SPACE))
    {
        gameEvents.push_back(GameEvent(GameEventType::MOVE_UP));
    }

    const Input::MouseState *mouseState = inputMngr.getMouseUdpated();
    if (mouseState != nullptr)
    {
        if (mouseState->mousePositionChange)
        {
            gameEvents.push_back(GameEvent(GameEventType::LOOK_ADJUSTMENT, mouseState->cursor_x, mouseState->cursor_y));
        }
        if (mouseState->scrollWheelChange)
        {
            GameEvent scrllWhlEvent(GameEventType::FOV_ADJUSTMENT);
            scrllWhlEvent.fovAdjustmnet = mouseState->scrllWheel_y;
            gameEvents.push_back(scrllWhlEvent);
        }
        
    }
    //======================================================//

    // Mode Toggling
    //======================================================//
    if (inputMngr.getKeyHeld(SDL_Scancode::SDL_SCANCODE_GRAVE)) // This is the TILDE scancode
    {
        gameEvents.push_back(GameEvent(GameEventType::TOGGLE_DEBUG_MODE));
    }


}


#endif // MAIN_H