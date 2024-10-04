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

const GameEvent sdlEvent_to_gameEvent(const SDL_Event &sdlEvent);
const GameEvent sdlKeyboardEventMap(const SDL_Event &sdlEvent);
void grabEvents(std::vector<GameEvent> &gameEvents);
int close_game(const int& returnStatus);

int close_game(const int& returnStatus)
{
    SDL_Quit();

    return returnStatus;
}

// Clears any previously logged events and populates vector iwth new ones
void grabEvents(std::vector<GameEvent> &gameEvents) // Take SDL_Events and transform them into GameEvents
{

    gameEvents.clear();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        GameEvent gameEvent = sdlEvent_to_gameEvent(event);
        if (gameEvent != GameEvent::NO_EVENT)
        {
            gameEvents.push_back(gameEvent);
        }
    }
}

const GameEvent sdlEvent_to_gameEvent(const SDL_Event &sdlEvent)
{
    switch (sdlEvent.type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        return sdlKeyboardEventMap(sdlEvent);
        break;
    
    default:
        return GameEvent::NO_EVENT;
        break;
    }
}

const GameEvent sdlKeyboardEventMap(const SDL_Event &sdlEvent)
{
    switch (sdlEvent.key.keysym.sym)
    {
    case SDLK_ESCAPE:
        return GameEvent::GAME_EXIT;
        break;
    default:
        return GameEvent::NO_EVENT;
        break;
    }
}


#endif // MAIN_H