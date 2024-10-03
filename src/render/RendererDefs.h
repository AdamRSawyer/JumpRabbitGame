#ifndef RENDERER_DEFS_H
#define RENDERER_DEFS_H

#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <string>
#include <vector>

#include "model/Model.h"

namespace Render
{

// Camera constants
static const float DEFAULT_CAMERA_FOV_RAD = glm::radians(70.0f);
static const float DEFAULT_ASPECT_RATIO = 16.0f/9.0f;
static const float DEFAULT_RENDER_MIN_DIST = 0.1f;
static const float DEFAULT_RENDER_MAX_DIST = 100.0f;
static const glm::vec3 DEFAULT_INIT_POS = glm::vec3(4.0f, 4.0f, 4.0f);
static const float DEFAULT_INIT_HORIZNTL_ANGL_RAD = glm::radians(0.0f);
static const float DEFAULT_INIT_VERT_ANGL_RAD = glm::radians(0.0f);

enum class MODEL_ID : int32_t
{
    CUBE = 0,

};

enum class SHADER_ID : int32_t
{
    SIMPLE_VERTEX_AND_FRAGMENT_SHADER = 0,
};

static std::string SHADER_PATH = "./src/shaders/";
static std::string SHADER_CONFIG_FILE = "shaderConfig.ini";

// Describes the strings associated with each enum entry for use with config file
const std::map<std::string, SHADER_ID> STRING_TO_SHADER_MAP = 
{
    {"SIMPLE_VERTEX_AND_FRAGMENT_SHADER", SHADER_ID::SIMPLE_VERTEX_AND_FRAGMENT_SHADER},
};

struct ShaderInfo
{
    GLuint programId;
    GLuint mvpId;
};

struct DrawQueueElmnt
{
    DrawQueueElmnt(MODEL_ID modelId, const SHADER_ID& shaderId, 
                   const glm::mat4& translation, const glm::mat4& rotationZYX, const glm::mat4& scale, 
                   const glm::mat4 &projection, const glm::mat4 &view) : modelId(modelId), shaderId(shaderId)
    {
        modelMatrix = translation * rotationZYX * scale;

        // Use translation matrix to determine distance from camera (forms z buffer)
        distFromCamera = glm::abs((projection * view * translation)[3][2]);
    }

    MODEL_ID modelId;
    SHADER_ID shaderId;
    glm::mat4 modelMatrix;
    float distFromCamera;

    bool operator>(const DrawQueueElmnt& queueElmnt)
    {
        return this->distFromCamera > queueElmnt.distFromCamera;   
    }

};

bool operator>(const DrawQueueElmnt& queueElmnt_1, const DrawQueueElmnt& queueElmnt_2);

const std::vector<glm::vec3> cubeVerts = {    
                glm::vec3(-1.0f,-1.0f,-1.0f),
                glm::vec3(-1.0f,-1.0f, 1.0f),
                glm::vec3(-1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f,-1.0f),
                glm::vec3(-1.0f,-1.0f,-1.0f),
                glm::vec3(-1.0f, 1.0f,-1.0f),
                glm::vec3(1.0f,-1.0f, 1.0f),
                glm::vec3(-1.0f,-1.0f,-1.0f),
                glm::vec3(1.0f,-1.0f,-1.0f),
                glm::vec3(1.0f, 1.0f,-1.0f),
                glm::vec3(1.0f,-1.0f,-1.0f),
                glm::vec3(-1.0f,-1.0f,-1.0f),
                glm::vec3(-1.0f,-1.0f,-1.0f),
                glm::vec3(-1.0f, 1.0f, 1.0f),
                glm::vec3(-1.0f, 1.0f,-1.0f),
                glm::vec3(1.0f,-1.0f, 1.0f),
                glm::vec3(-1.0f,-1.0f, 1.0f),
                glm::vec3(-1.0f,-1.0f,-1.0f),
                glm::vec3(-1.0f, 1.0f, 1.0f),
                glm::vec3(-1.0f,-1.0f, 1.0f),
                glm::vec3(1.0f,-1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f,-1.0f,-1.0f),
                glm::vec3(1.0f, 1.0f,-1.0f),
                glm::vec3(1.0f,-1.0f,-1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f,-1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f,-1.0f),
                glm::vec3(-1.0f, 1.0f,-1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(-1.0f, 1.0f,-1.0f),
                glm::vec3(-1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(-1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f,-1.0f, 1.0f)
};

}
#endif // RENDERER_DEFS_H