#ifndef MODEL_DEFS_H
#define MODEL_DEFS_H

#include <stdint.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GL/glew.h>

namespace Render
{


enum class SHADER_LOCATIONS : GLuint
{
    VERTEX = 0,
    UV     = 1,
    NORM   = 2
};

static const uint32_t INVALID_TEXTURE   = 0xFFFF;
static const GLuint   BLANK_VAO_ID      =      0;
static const auto     ASSIMP_LOAD_FLAGS = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices;

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
    MeshEntry(uint32_t numIndices, uint32_t startIndexIdx, uint32_t startVertexIdx, uint32_t textureIdx = INVALID_TEXTURE) : 
              numIndices(numIndices), startIndexIdx(startIndexIdx), startVertexIdx(startVertexIdx), textureIdx(textureIdx) {}

    uint32_t numIndices;
    uint32_t startIndexIdx;
    uint32_t startVertexIdx;
    uint32_t textureIdx;
};

}
#endif // MODEL_DEFS_H