#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <queue>

#include "RendererDefs.h"

namespace Render
{

class Renderer // Singleton Class that handles rendering of all game objects
{

    public:
    
    Renderer(const Renderer& renderer) = delete;
    
    // A gl context should already exist before initializing the renderer
    static Renderer* getRenderer()
    {
        if (rendererInstance == nullptr)
        {
            rendererInstance = new Renderer();
        }
        return rendererInstance;
    }
    
    // Returns 0 if success and -1 if error occurred
    int queueDraw(const MODEL_ID& modelId, const SHADER_ID& shaderId, 
                  const glm::mat4& translation, const glm::mat4& rotationZYX, 
                  const glm::mat4& scale);

    // This should be called in the main loop to initialize the singleton renderer
    void setSDLWindow(SDL_Window *windowPtr);

    // Render all of the models in the drawQueue
    int render();

    private:
    Renderer();
    static Renderer* rendererInstance;

    // Load model verticies into gl buffer
    // Returns the number of verticies in the model
    int getModelVerts(const MODEL_ID& modelId);
    // Set given shader as current gl program
    void getShader(const SHADER_ID& shaderId);
    
    // Returns 0 if success and -1 if error occurred
    int renderModel(const MODEL_ID& modelId);
    // Returns 0 if success and -1 if error occurred
    int loadModel(const MODEL_ID& modelId);
    // Returns 0 if success and -1 if error occurred
    int loadShader(const SHADER_ID& shaderId);
    // Returns 0 if success and -1 if error occurred
    int loadShaderConfig();

    std::map<MODEL_ID, std::unique_ptr<Model>> loadedModels;
    std::map<SHADER_ID, ShaderInfo> shaderMap;
    std::map<SHADER_ID, std::pair<std::string, std::string>> shaderFNameMap;
    std::priority_queue<DrawQueueElmnt, std::vector<DrawQueueElmnt>, std::greater<DrawQueueElmnt>> drawQueue;

    // Camera Variables
    glm::mat4 projection;
    glm::mat4 view;

    SDL_Window *window = nullptr;

};


}



#endif // RENDERER_H