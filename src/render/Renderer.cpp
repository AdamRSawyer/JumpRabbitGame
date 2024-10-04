#include "Renderer.h"

#include <fstream>
#include <sstream>
#include <regex>

namespace Render
{
    Renderer* Renderer::rendererInstance = nullptr;

    Renderer::Renderer()
    {
        loadedModels[MODEL_ID::CUBE] = std::unique_ptr<Model>(new Model(Render::cubeIndices, Render::cubeVerts, 
                                                                        Render::cubeUVs, Render::cubeNorms, 
                                                                        Texture((unsigned char *)cubeTexturePng, sizeof(cubeTexturePng), GL_TEXTURE_2D)));

        // Set default projection and view matrices
        projection = glm::perspective(DEFAULT_CAMERA_FOV_RAD, DEFAULT_ASPECT_RATIO, DEFAULT_RENDER_MIN_DIST, DEFAULT_RENDER_MAX_DIST);
        glm::vec3 cameraLookDir(
            glm::cos(DEFAULT_INIT_VERT_ANGL_RAD) * glm::sin(DEFAULT_INIT_HORIZNTL_ANGL_RAD),
            glm::sin(DEFAULT_INIT_VERT_ANGL_RAD),
            glm::cos(DEFAULT_INIT_VERT_ANGL_RAD) * glm::cos(DEFAULT_INIT_HORIZNTL_ANGL_RAD)

        );

        glm::vec3 right(
            glm::sin(DEFAULT_INIT_HORIZNTL_ANGL_RAD - glm::radians(45.0f)),
            0,
            glm::cos(DEFAULT_INIT_HORIZNTL_ANGL_RAD - glm::radians(45.0f))
        );

        glm::vec3 upVec = glm::cross(right, cameraLookDir);

        view = glm::lookAt(
            DEFAULT_INIT_POS,
            DEFAULT_INIT_POS + cameraLookDir,
            upVec
        );

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        
        // Dark blue background
	    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        
        // Load shader config file
        loadShaderConfig();
        // Load default shader
        if (loadShader(SHADER_ID::SIMPLE_VERTEX_AND_FRAGMENT_SHADER) == -1)
        {
            printf("Failed to load default shader.\n");
        }

        // Load model config file
        loadModelConfig();
    }

    // modelMatrix: translation * rotation * scale * glm::mat4(1.0f)
    int Renderer::queueDraw(const MODEL_ID& modelId, const SHADER_ID& shaderId, 
                            const glm::mat4& translation, const glm::mat4& rotationZYX, 
                            const glm::mat4& scale)
    {
        drawQueue.emplace(DrawQueueElmnt(modelId, shaderId, translation, rotationZYX, scale, projection, view));     
        return 0;  
    }

    // Checks to see if the given model is loaded, draws it if it is, loads then draws it if not, and if it fails to load it draws the default cube model
    int Renderer::renderModel(const MODEL_ID& modelId) 
    {

        if (loadedModels.count(modelId))
        {
            loadedModels[modelId]->render();
            return 0;
        }
        else if (loadModel(modelId) == 0)
        {
            // Needs implemented: Load model if not loaded
            loadedModels[modelId]->render();
            return 0;
        }
        else // Render default cube model
        {
            loadedModels[MODEL_ID::CUBE]->render();
            printf("Failed to load MODEL_ID %i", modelId);
            return -1;
        }
    }

    void Renderer::getShader(const SHADER_ID& shaderId)
    {
        if (shaderMap.count(shaderId))
        {
            glUseProgram(shaderMap[shaderId].programId);
        }
        else if (loadShader(shaderId) == 0)
        {
            glUseProgram(shaderMap[shaderId].programId);
        }
        else // Use default shader
        {
            glUseProgram(shaderMap[SHADER_ID::SIMPLE_VERTEX_AND_FRAGMENT_SHADER].programId);
        }
    }

    int Renderer::loadModel(const MODEL_ID& modelId)
    {
        if (modelFNameMap.count(modelId))
        {
            loadedModels[modelId] = std::unique_ptr<Model>( new Model((MODEL_FOLDER_PATH + modelFNameMap[modelId]).c_str()));
            if (loadedModels[modelId]->getModelInit())
            {
                return 0;
            }
            else
            {
                loadedModels[modelId].reset();
                loadedModels.erase(modelId);
            }
        }
        return -1;
    }

    int Renderer::loadShader(const SHADER_ID& shaderId)
    {
        GLuint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

        if (!shaderFNameMap.count(shaderId))
        {
            printf("The given shaderId, %i, does not have an associated set of filenames. Make sure that you are pointing to files for the shader in shaderConfig.ini\n", shaderId);
            return -1;
        }
        std::string vertexShaderPath = Render::SHADER_PATH + shaderFNameMap.at(shaderId).first;
        std::string fragmentShaderPath = Render::SHADER_PATH + shaderFNameMap.at(shaderId).second;
        
        // Read code for vertex shader
        std::ifstream vertexShaderFile(vertexShaderPath, std::ios::in);
        std::string vertexShaderCode;
        if (vertexShaderFile.is_open())
        {
            std::stringstream stream;
            stream << vertexShaderFile.rdbuf();
            vertexShaderCode = stream.str();
            vertexShaderFile.close();
        }
        else
        {
            printf("Failed to open vertex shader: %s", vertexShaderPath.c_str());
            return -1;
        }

        // Read code for fragment shader
        std::ifstream fragmentShaderFile(fragmentShaderPath, std::ios::in);
        std::string fragmentShaderCode;
        if (fragmentShaderFile.is_open())
        {
            std::stringstream stream;
            stream << fragmentShaderFile.rdbuf();
            fragmentShaderCode = stream.str();
            fragmentShaderFile.close();
        }
        else
        {
            printf("Failed to open vertex shader: %s", fragmentShaderPath.c_str());
            return -1;
        }

        // Compile Vertex Shader
        char const *vertexShaderCodePntr = vertexShaderCode.c_str();
        glShaderSource(vertShaderId, 1, &vertexShaderCodePntr, NULL);
        glCompileShader(vertShaderId);

        // Check that vertex shader compiled
        GLint result = GL_FALSE;
        int infoLogLen;

        glGetShaderiv(vertShaderId, GL_COMPILE_STATUS, &result);
        glGetShaderiv(vertShaderId, GL_INFO_LOG_LENGTH, &infoLogLen);
        
        if (infoLogLen > 0)
        {
            std::vector<char> vertShaderErrMsg(infoLogLen + 1);
            glGetShaderInfoLog(vertShaderId, infoLogLen, NULL, &vertShaderErrMsg[0]);
            printf("%s\n", &vertShaderErrMsg[0]);
        }

        // Compile Fragment Shader
        char const *fragmentShaderCodePntr = fragmentShaderCode.c_str();
        glShaderSource(fragShaderId, 1, &fragmentShaderCodePntr, NULL);
        glCompileShader(fragShaderId);

        // Check that vertex shader compiled
        glGetShaderiv(fragShaderId, GL_COMPILE_STATUS, &result);
        glGetShaderiv(fragShaderId, GL_INFO_LOG_LENGTH, &infoLogLen);
        
        if (infoLogLen > 0)
        {
            std::vector<char> fragShaderErrMsg(infoLogLen + 1);
            glGetShaderInfoLog(fragShaderId, infoLogLen, NULL, &fragShaderErrMsg[0]);
            printf("%s\n", &fragShaderErrMsg[0]);
        }

        shaderMap[shaderId].programId = glCreateProgram();
        glAttachShader(shaderMap[shaderId].programId, vertShaderId);
        glAttachShader(shaderMap[shaderId].programId, fragShaderId);
        glLinkProgram(shaderMap[shaderId].programId);

        glGetProgramiv(shaderMap[shaderId].programId, GL_LINK_STATUS, &result);
        glGetProgramiv(shaderMap[shaderId].programId, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0)
        {
            std::vector<char> programErrMsg(infoLogLen + 1);
            glGetShaderInfoLog(shaderMap[shaderId].programId, infoLogLen, NULL, &programErrMsg[0]);
            printf("%s\n", &programErrMsg[0]);
        }

        glDetachShader(shaderMap[shaderId].programId, vertShaderId);
        glDetachShader(shaderMap[shaderId].programId, fragShaderId);

        glDeleteShader(vertShaderId);
        glDeleteShader(fragShaderId);

        // Attach MVP uniform to program
        shaderMap[shaderId].mvpId = glGetUniformLocation(shaderMap[shaderId].programId, "MVP");
        shaderMap[shaderId].diffuseTextureUniformId = glGetUniformLocation(shaderMap[shaderId].programId, "diffuseTexture");
        glUniform1i(shaderMap[shaderId].diffuseTextureUniformId, GL_TEXTURE0);

        return 0;
    }

    int Renderer::render()
    {
        if (window != nullptr)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            while (!drawQueue.empty())
            {
                getShader(drawQueue.top().shaderId);

                // Generate MVP
                glm::mat4 mvp = projection * view * drawQueue.top().modelMatrix;
                // Send MVP matrix to shader
                glUniformMatrix4fv(shaderMap[drawQueue.top().shaderId].mvpId, 1, GL_FALSE, &mvp[0][0]);
                renderModel(drawQueue.top().modelId);

                drawQueue.pop();
            }
            // Swap Buffers
            SDL_GL_SwapWindow(window);
        }
        else
        {
            printf("Call setSDLWindow(SDL_Window *windowPtr) so the renderer knows what to draw to\n");
        }
        return 0;
    }

    int Renderer::loadShaderConfig()
    {
        std::ifstream shaderConfig(SHADER_PATH + SHADER_CONFIG_FILE);

        const uint8_t BUFF_SIZE = 255;
        char buff[BUFF_SIZE];

        if (shaderConfig.is_open())
        {
            while(!shaderConfig.eof())
            {
                shaderConfig.getline(buff, BUFF_SIZE);
                std::string line(buff);
                std::string shaderName = "", vertShaderFName = "", fragmentShaderFName = "";

                // For understanding the regex use https://regex101.com/
                // C++ uses the ECMAScript flavor of regex
                std::regex shaderNameRgx(R"([\w]+(?=\s*=))");
                std::regex vertShaderNameRgx(R"([\w.]+.glsl(?=\s*[^"]))");
                std::regex fragShaderNameRgx(R"([\w.]+.glsl(?=\s*\"))");

                std::smatch searchMatch;

                if(std::regex_search(line, searchMatch, shaderNameRgx))
                {
                    shaderName = searchMatch.str(0);
                }
                if(std::regex_search(line, searchMatch, vertShaderNameRgx))
                {
                    vertShaderFName = searchMatch.str(0);
                }
                if(std::regex_search(line, searchMatch, fragShaderNameRgx))
                {
                    fragmentShaderFName = searchMatch.str(0);
                }

                if (shaderName.length() && vertShaderFName.length() && fragmentShaderFName.length())
                {
                    if (STRING_TO_SHADER_MAP.count(shaderName))
                    {
                        shaderFNameMap[STRING_TO_SHADER_MAP.at(shaderName)] = std::pair(vertShaderFName, fragmentShaderFName);
                    }
                    
                }
                
            }
            shaderConfig.close();
        }
        else
        {
            return -1;
        }
        return 0;
    }

    int Renderer::loadModelConfig()
    {
        std::ifstream modelConfig(MODEL_FOLDER_PATH + MODEL_CONFIG_FILE);

        const uint8_t BUFF_SIZE = 255;
        char buff[BUFF_SIZE];

        if (modelConfig.is_open())
        {
            while(!modelConfig.eof())
            {
                modelConfig.getline(buff, BUFF_SIZE);
                std::string line(buff);
                std::string modelName = "", modelFName = "";

                // For understanding the regex use https://regex101.com/
                // C++ uses the ECMAScript flavor of regex
                std::regex modelNameRgx(R"([\w]+(?=\s*=))");
                std::regex modelFNameRgx(R"([\w./]+.\w*[^=]\s*[^=](?=\s*"))");

                std::smatch searchMatch;

                if(std::regex_search(line, searchMatch, modelNameRgx))
                {
                    modelName = searchMatch.str(0);
                }
                if(std::regex_search(line, searchMatch, modelFNameRgx))
                {
                    modelFName = searchMatch.str(0);
                }

                if (modelName.length() && modelFName.length())
                {
                    if (STRING_TO_MODEL_MAP.count(modelName))
                    {
                        modelFNameMap[STRING_TO_MODEL_MAP.at(modelName)] = modelFName;
                    }
                    
                }
                
            }
            modelConfig.close();
        }
        else
        {
            return -1;
        }
        return 0;
    }

    void Renderer::setSDLWindow(SDL_Window *windowPtr)
    {
        window = windowPtr;
    }
}