#include "main.h"

int main()
{
    const int SDL_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    const int VIDEO_WIDTH = 1920, VIDEO_HEIGHT = 1080;
    const int EVENT_RESERVE_CNT = 10;
    const int FRAME_RATE = 60;
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

    std::vector<GameEvent> eventQueue; eventQueue.reserve(EVENT_RESERVE_CNT);

    if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS )== -1)
    {
        fprintf(stderr, "SDL Init Error: %s", SDL_GetError());   
        close_game(-1);
    }

    SDL_Window *window = SDL_CreateWindow("Jump Rabbit", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, VIDEO_WIDTH, VIDEO_HEIGHT, SDL_FLAGS);
    
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    // Initialize extension wrangler
    if(glewInit() != GLEW_OK)
    {
        printf("Failed to init GLEW. Closing Game.");
        return -1;
    }

    glViewport( 0, 0, VIDEO_WIDTH, VIDEO_HEIGHT );

    // Initializing the renderer and giving it the pointer to the window to draw to
    Render::Renderer* renderer = Render::Renderer::getRenderer();
    renderer->setSDLWindow(window);

    Input::SdlInputManagement inputManager;
    SDL_SetRelativeMouseMode(SDL_bool::SDL_TRUE);

    bool gameRunning = true;
    Game game(FRAME_RATE);

    while (gameRunning)
    {
        grabEvents(eventQueue, inputManager);
        if (game.loop(eventQueue) == -1)
        {
            gameRunning = false;
        }
    }
    return close_game(0);
}