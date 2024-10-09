#include "Game.h"

Game::Game(int frameRate) : frameRate(frameRate)
{
    // Initialize game with test entity
    //entities.push_back(new gameEntities::StaticObj(Render::MODEL_ID::CUBE, Render::SHADER_ID::SIMPLE_VERTEX_AND_FRAGMENT_SHADER));
    entities.push_back(new gameEntities::StaticObj(Render::MODEL_ID::MAN, Render::SHADER_ID::SIMPLE_VERTEX_AND_FRAGMENT_SHADER));
    entities.back()->offsetPosition(glm::vec3(5.0f, -5.0f, 5.0f));
    entities.back()->offsetScale(glm::vec3(0.75f, 0.75f, 0.75f));

    renderer = Render::Renderer::getRenderer();

    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    Render::CameraMoveElmnt camMove(glm::vec3(4.0f, 30.0f, 4.0f), glm::vec3(glm::radians(-45.0f), glm::radians(0.0f), 0.0f), glm::radians(90.0f), deadline);
    renderer->addCameraMove(camMove);
    camMove = Render::CameraMoveElmnt(Render::DEFAULT_INIT_POS, glm::vec3(Render::DEFAULT_INIT_VERT_ANGL_RAD, Render::DEFAULT_INIT_HORIZNTL_ANGL_RAD, 0.0f), glm::radians(90.0f), deadline + std::chrono::seconds(10));
    renderer->addCameraMove(camMove);
    camMove = Render::CameraMoveElmnt(glm::vec3(16.0f, 4.0f, 4.0f), glm::vec3(Render::DEFAULT_INIT_VERT_ANGL_RAD, glm::radians(-90.0f), 0.0f), glm::radians(90.0f), deadline + std::chrono::seconds(15));
    renderer->addCameraMove(camMove);
    camMove = Render::CameraMoveElmnt(glm::vec3(24.0f, 4.0f, 24.0f), glm::vec3(Render::DEFAULT_INIT_VERT_ANGL_RAD, glm::radians(-180.0f), 0.0f), glm::radians(90.0f), deadline + std::chrono::seconds(20));
    renderer->addCameraMove(camMove);


    msRenderDelta = std::chrono::milliseconds(1000 / frameRate);

    lastRenderTime = std::chrono::steady_clock::now();

    debugMode = false;

}

int Game::loop(const std::vector<GameEvent>& gameEvents)
{
    std::chrono::steady_clock::time_point curTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point timeOfRender = max(curTime, lastRenderTime + msRenderDelta);

    int returnValue = 0;
    for (auto event: gameEvents)
    {
        switch (event.eventType)
        {
        case GameEventType::GAME_EXIT:
            returnValue = -1;
            break;

        case GameEventType::TOGGLE_DEBUG_MODE:
            debugMode = !debugMode;
            break;

        case GameEventType::MOVE_FORWARD:
        case GameEventType::MOVE_BACKWARD:
        case GameEventType::MOVE_LEFT:
        case GameEventType::MOVE_RIGHT:
        case GameEventType::MOVE_UP:
        case GameEventType::MOVE_DOWN:
        case GameEventType::LOOK_ADJUSTMENT:
        case GameEventType::FOV_ADJUSTMENT:
            if (debugMode)
            {
                debugCameraMove(event);  
            }
            break;

        default:
            break;
        }

    }
    
    if (!debugMode)
    {
        renderer->updateCameraPosition(timeOfRender);
    }

    // Find all renderable game entities and call their render method
    std::vector<gameEntities::Entity *> renderableEntities;
    findEntities(gameEntities::EntityAttributes::CAN_RENDER, renderableEntities);
    // Append all renderable entities to the draw queue
    for (auto entPtr : renderableEntities)
    {
        entPtr->offsetRotation(glm::vec3(glm::radians(0.0f), glm::radians(0.2f), 0.0f));
        entPtr->render();
    }

    renderer->render();

    // Tell the renderer to draw all of the entities in the queue       
    while (std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastRenderTime) < msRenderDelta)
    {
        curTime = std::chrono::steady_clock::now();
    }

    lastRenderTime = curTime;

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

void Game::debugCameraMove(const GameEvent &event)
{
    const float DEBUG_CAMERA_MOVE_SPEED_UNITS_PER_SECOND = 10.0f / frameRate;
    const float DEBUG_CAMERA_ROTATION_MULTIPLIER = 0.1f;
    const float DEBUG_CAMERA_FOV_MULTIPLIER = 1.0f;

    switch (event.eventType)
    {
    // let look adjustment occur first so that move forward, backward, left and right can use the update cameraLookDir and right vectors
    case GameEventType::LOOK_ADJUSTMENT:
        renderer->offsetCameraRot(glm::vec3(glm::radians(-event.lookAdjustmentY * DEBUG_CAMERA_ROTATION_MULTIPLIER), glm::radians(-event.lookAdjustmentX * DEBUG_CAMERA_ROTATION_MULTIPLIER), 0.0f));
        break;
    case GameEventType::FOV_ADJUSTMENT:
        renderer->offsetFov(glm::radians(-event.fovAdjustmnet * DEBUG_CAMERA_FOV_MULTIPLIER));
        break;
    case GameEventType::MOVE_FORWARD:
        renderer->moveCameraZAxis(DEBUG_CAMERA_MOVE_SPEED_UNITS_PER_SECOND);
        //renderer->offsetCamaraPos(glm::vec3(0.0f, 0.0f, DEBUG_CAMERA_MOVE_SPEED_UNITS_PER_SECOND));
        break;
    case GameEventType::MOVE_BACKWARD:
        renderer->moveCameraZAxis(-DEBUG_CAMERA_MOVE_SPEED_UNITS_PER_SECOND);
        //renderer->offsetCamaraPos(glm::vec3(0.0f, 0.0f, -DEBUG_CAMERA_MOVE_SPEED_UNITS_PER_SECOND));
        break;
    case GameEventType::MOVE_RIGHT:
        renderer->moveCameraXAxis(DEBUG_CAMERA_MOVE_SPEED_UNITS_PER_SECOND);
        //renderer->offsetCamaraPos(glm::vec3(-DEBUG_CAMERA_MOVE_SPEED_UNITS_PER_SECOND, 0.0f, 0.0f));
        break;
    case GameEventType::MOVE_LEFT:
        renderer->moveCameraXAxis(-DEBUG_CAMERA_MOVE_SPEED_UNITS_PER_SECOND);
        //renderer->offsetCamaraPos(glm::vec3(DEBUG_CAMERA_MOVE_SPEED_UNITS_PER_SECOND, 0.0f, 0.0f));
        break;
    case GameEventType::MOVE_UP:
        renderer->offsetCamaraPos(glm::vec3(0.0f, DEBUG_CAMERA_MOVE_SPEED_UNITS_PER_SECOND, 0.0f));
        break;
    case GameEventType::MOVE_DOWN:
        renderer->offsetCamaraPos(glm::vec3(0.0f, -DEBUG_CAMERA_MOVE_SPEED_UNITS_PER_SECOND, 0.0f));
        break;
    default:
        break;
    }
}