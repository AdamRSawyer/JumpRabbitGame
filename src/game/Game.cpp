#include "Game.h"

Game::Game(int frameRate) : frameRate(frameRate)
{
    // Initialize game with test entity
    //entities.push_back(new gameEntities::StaticObj(Render::MODEL_ID::CUBE, Render::SHADER_ID::SIMPLE_VERTEX_AND_FRAGMENT_SHADER));
    entities.push_back(new gameEntities::StaticObj(Render::MODEL_ID::MAN, Render::SHADER_ID::SIMPLE_VERTEX_AND_FRAGMENT_SHADER));
    entities.back()->offsetPosition(glm::vec3(5.0f, -5.0f, 5.0f));
    entities.back()->offsetScale(glm::vec3(0.75f, 0.75f, 0.75f));

    renderer = Render::Renderer::getRenderer();

    msRenderDelta = std::chrono::milliseconds(1000 / frameRate);

    lastRenderTime = std::chrono::steady_clock::now();

}

int Game::loop(const std::vector<GameEvent>& gameEvents)
{
    std::chrono::steady_clock::time_point curTime = std::chrono::steady_clock::now();

    int returnValue = 0;
    for (auto event: gameEvents)
    {
        switch (event)
        {
        case GameEvent::GAME_EXIT:
            returnValue = -1;
            break;
        
        default:
            break;
        }

    }

    if (std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastRenderTime) > msRenderDelta)
    {
        lastRenderTime = curTime;
        // Find all renderable game entities and call their render method
        std::vector<gameEntities::Entity *> renderableEntities;
        findEntities(gameEntities::EntityAttributes::CAN_RENDER, renderableEntities);
        // Append all renderable entities to the draw queue
        for (auto entPtr : renderableEntities)
        {
            entPtr->offsetRotation(glm::vec3(glm::radians(0.0f), glm::radians(0.2f), 0.0f));
            entPtr->render();
            
        }

        // Tell the renderer to draw all of the entities in the queue
        renderer->render();
    }

    return returnValue;
}

void Game::findEntities(gameEntities::EntityAttributes attr, std::vector<gameEntities::Entity*> &returnVec)
{
    for (int i = 0; i < entities.size(); ++i)
    {
        if (entities[i]->hasAttr(attr))
        {
            returnVec.push_back(entities[i]);
        }
    }
}

int Game::setFrameRate(const int frame_rate)
{
    frameRate = frame_rate;
    return 0;
}
