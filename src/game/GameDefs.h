#ifndef GAME_DEFS_H
#define GAME_DEFS_H

#include <stdint.h>
#include <vector>

enum class GameEventType : int32_t 
{
    NO_EVENT = 0,

    // Movement
    MOVE_FORWARD,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,

    LOOK_ADJUSTMENT,
    FOV_ADJUSTMENT,
    
    
    // Mode Toggling
    TOGGLE_DEBUG_MODE,



    GAME_EXIT,

};

struct GameEvent
{
    GameEvent(GameEventType eventType) : eventType(eventType) {};
    GameEvent(GameEventType eventType, int lookAdjustmentX, int lookAdjustmentY) : eventType(eventType), 
                                                                                   lookAdjustmentX(lookAdjustmentX), 
                                                                                   lookAdjustmentY(lookAdjustmentY) {};
    
    GameEventType eventType;

    int lookAdjustmentX, lookAdjustmentY;
    int fovAdjustmnet;
};


#endif // GAME_DEFS_H