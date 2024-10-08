#ifndef GAME_H
#define GAME_H

#include "GameDefs.h"

#include <chrono>

// Entity Includes:
#include "../entity/Entity.h"
#include "../entity/StaticObj.h"

// Renderer Includes
#include "../render/Renderer.h"

class Game
{
    public:
    Game(int frameRate);

    // Returns -1 to indicate game closure
    // Returns 0 otherwise
    int loop(const std::vector<GameEvent>& gameEvents);
    
    // Returns -1 to indicate game closure
    // Returns 0 otherwise
    int setFrameRate(const int frame_rate);

    private:
        std::vector<gameEntities::Entity *> entities;

        // Return a vector of pointers to entities with the given attribute
        void findEntities(gameEntities::EntityAttributes attr, std::vector<gameEntities::Entity*> &returnVec);

        Render::Renderer *renderer = nullptr;

        int frameRate;
        std::chrono::milliseconds msRenderDelta;
        std::chrono::steady_clock::time_point lastRenderTime;

        void debugCameraMove(const GameEvent &event);

        bool debugMode;
};

#endif // GAME_H