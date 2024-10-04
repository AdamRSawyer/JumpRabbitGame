#ifndef MODEL_DEFS_H
#define MODEL_DEFS_H

#include <stdint.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GL/glew.h>

//#include "cubeTexture.h"
#include "chungusTexture.h"

namespace Render
{

enum class MODEL_ID : int32_t
{
    CUBE = 0,
    MAN,

};

enum class SHADER_LOCATIONS : GLuint
{
    VERTEX = 0,
    UV     = 1,
    NORM   = 2
};

static const std::string MODEL_FOLDER_PATH = "./resources/models/";
static const std::string MODEL_CONFIG_FILE = "modelConfig.ini";

static const uint32_t INVALID_TEXTURE   = 0xFFFF;
static const GLuint   BLANK_VAO_ID      =      0;
static const auto     ASSIMP_LOAD_FLAGS = aiProcess_FlipUVs | aiProcess_FixInfacingNormals | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices;

enum class BufferType : GLuint // Makes our attribute array numbering consistent and readable
{
    INDEX_BUFF = 0,
    VERTEX_BUFF,
       UV_BUFF,
    NORMAL_BUFF,
    BUFFER_NUM // Keep this at the bottom. It tells us how many buffers are above
};

struct MeshEntry // Helps us tell OpenGL what indices to look at in order to draw a mesh contained in a large array
{
    MeshEntry(GLuint numIndices, uint32_t startIndexIdx, uint32_t startVertexIdx, uint32_t textureIdx = INVALID_TEXTURE) : 
              numIndices(numIndices), startIndexIdx(startIndexIdx), startVertexIdx(startVertexIdx), textureIdx(textureIdx) {}

    GLuint numIndices;
    uint32_t startIndexIdx;
    uint32_t startVertexIdx;
    uint32_t textureIdx;
};

// Default Cube Model Variables:
// Used when an asset fails to load 

static const std::vector<GLuint> cubeIndices = {
                0,
                1,
                2,
                3,
                4,
                5,
                6,
                7,
                8,
                9,
                10,
                11,
                12,
                13,
                14,
                15,
                16,
                17,
                18,
                19,
                20,
                21,
                22,
                23,
                24,
                25,
                26,
                27,
                28,
                29,
                30,
                31,
                32,
                33,
                34,
                35
};

static const std::vector<glm::vec3> cubeVerts = {    

            glm::vec3(-1.0f, 1.0f, -1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, -1.0f),
            
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(-1.0f, -1.0f, 1.0f),
            glm::vec3(1.0f, -1.0f, 1.0f),

            glm::vec3(-1.0f, 1.0f, 1.0f),
            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3(-1.0f, -1.0f, 1.0f),

            glm::vec3(1.0f, -1.0f, -1.0f),    
            glm::vec3(-1.0f, -1.0f, 1.0f),     
            glm::vec3(-1.0f, -1.0f, -1.0f),

            glm::vec3(1.0f, 1.0f, -1.0f),
            glm::vec3(1.0f, -1.0f, 1.0f),
            glm::vec3(1.0f, -1.0f, -1.0f),

            glm::vec3(-1.0f, 1.0f, -1.0f),
            glm::vec3(1.0f, -1.0f, -1.0f),
            glm::vec3(-1.0f, -1.0f, -1.0f),

            glm::vec3(-1.0f, 1.0f, -1.0f),
            glm::vec3(-1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),

            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(-1.0f, 1.0f, 1.0f),
            glm::vec3(-1.0f, -1.0f, 1.0f),

            glm::vec3(-1.0f, 1.0f, 1.0f),
            glm::vec3(-1.0f, 1.0f, -1.0f),
            glm::vec3(-1.0f, -1.0f, -1.0f),

            glm::vec3(1.0f, -1.0f, -1.0f),
            glm::vec3(1.0f, -1.0f, 1.0f),
            glm::vec3(-1.0f, -1.0f, 1.0f),

            glm::vec3(1.0f, 1.0f, -1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, -1.0f, 1.0f),

            glm::vec3(-1.0f, 1.0f, -1.0f),
            glm::vec3(1.0f, 1.0f, -1.0f),
            glm::vec3(1.0f, -1.0f, -1.0f),

            // 1.    glm::vec3(1.0f, 1.0f, -1.0f),
            // 2.    glm::vec3(1.0f, -1.0f, -1.0f),
            // 3.    glm::vec3(1.0f, 1.0f, 1.0f),
            // 4.    glm::vec3(1.0f, -1.0f, 1.0f),
            // 5.    glm::vec3(-1.0f, 1.0f, -1.0f),
            // 6.    glm::vec3(-1.0f, -1.0f, -1.0f),
            // 7.    glm::vec3(-1.0f, 1.0f, 1.0f),
            // 8.    glm::vec3(-1.0f, -1.0f, 1.0f),
};

static const std::vector<glm::vec3> cubeNorms = 
{
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f),
                
                glm::vec3(0.0f, 0.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, -1.0f),

                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),

                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),

                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f),
                
                glm::vec3(0.0f, 0.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, -1.0f),

                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),

                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(-1.0f, 0.0f, 0.0f),
                
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
};

static const std::vector<glm::vec2> cubeUVs 
{
                glm::vec2(0.875f, 0.5f),
                glm::vec2(0.625f, 0.75f),
                glm::vec2(0.625f, 0.5f),
               
                glm::vec2(0.625f, 0.75f),
                glm::vec2(0.375f, 1.0f),
                glm::vec2(0.375f, 0.75f),

                glm::vec2(0.625f, 0.0f),    
                glm::vec2(0.375f, 0.25f),   
                glm::vec2(0.375f, 0.0f),   
                
                glm::vec2(0.375f, 0.5f),  
                glm::vec2(0.125f, 0.75f),   
                glm::vec2(0.125f, 0.5f),

                glm::vec2(0.625f, 0.5f),
                glm::vec2(0.375f, 0.75f),
                glm::vec2(0.375f, 0.5f),

                glm::vec2(0.625f, 0.25f),
                glm::vec2(0.375f, 0.5f),
                glm::vec2(0.375f, 0.25f),

                glm::vec2(0.875f, 0.5f),
                glm::vec2(0.875f, 0.75f),
                glm::vec2(0.625f, 0.75f),

                glm::vec2(0.625f, 0.75f),
                glm::vec2(0.625f, 1.0f),
                glm::vec2(0.375f, 1.0f),

                glm::vec2(0.625f, 0.0f),
                glm::vec2(0.625f, 0.25f),
                glm::vec2(0.375f, 0.25f),

                glm::vec2(0.375f, 0.5f),
                glm::vec2(0.375f, 0.75f),
                glm::vec2(0.125f, 0.75f),

                glm::vec2(0.625f, 0.5f),
                glm::vec2(0.625f, 0.75f),
                glm::vec2(0.375f, 0.75f),

                glm::vec2(0.625f, 0.25f),
                glm::vec2(0.625f, 0.5f),
                glm::vec2(0.375f, 0.5f)

            // 1.    glm::vec2(0.875f, 0.5f),
            // 2.    glm::vec2(0.625f, 0.75f),
            // 3.    glm::vec2(0.625f, 0.5f),
            // 4.    glm::vec2(0.375f, 1.0f),
            // 5.    glm::vec2(0.375f, 0.75f),
            // 6.    glm::vec2(0.625f, 0.0f),
            // 7.    glm::vec2(0.375f, 0.25f),
            // 8.    glm::vec2(0.375f, 0.0f),
            // 9.    glm::vec2(0.375f, 0.5f),
            // 10.   glm::vec2(0.125f, 0.75f),
            // 11.   glm::vec2(0.125f, 0.5f),
            // 12.   glm::vec2(0.625f, 0.25f),
            // 13.   glm::vec2(0.875f, 0.75f),
            // 14.   glm::vec2(0.625f, 1.0f),     
};


}
#endif // MODEL_DEFS_H             