#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <memory>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

#include "ModelDefs.h"
#include "Texture.h"

namespace Render
{


class Model
{
    public:
    
    Model(const char* modelPath);
    Model(const std::vector<GLuint> &indexBuffer, 
          const std::vector<glm::vec3> &vertBuffer,
          const std::vector<glm::vec2> &uvBuffer,
          const std::vector<glm::vec3> &normBuffer,
          const Texture &texture);

    int loadModel(const char* modelPath);
    int render();

    bool getModelInit();

    protected:
    int populateOglBuffers();
    int initVertIndiceMesh(const aiScene *modelScenePtr, uint64_t &vertCnt, uint64_t &indiceCnt);

    bool modelInit; // Lets you know if the model successfully loaded

    // The ID used to get the buffer to the model data in VRAM
    GLuint vaoId;
    GLuint bufferIds[(uint64_t)BufferType::BUFFER_NUM];

    // We will be using an array of structures approach. Therefore multiple meshes will be stored in the same array and we access them with offsets
    std::vector<MeshEntry> meshes;
    std::vector<std::unique_ptr<Texture>> textures; // Each mesh can have its own texture

    // Where model information is stored in RAM before being sent to VRAM
    std::vector<uint32_t>  gl_index_buffer; 
    std::vector<glm::vec3> gl_vert_buffer;
    std::vector<glm::vec2> gl_uv_buffer;
    std::vector<glm::vec3> gl_norm_buffer; 

};

}
#endif // MODEL_H