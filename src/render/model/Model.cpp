#include "Model.h"

#include <regex>

namespace Render
{
    Model::Model(const char *modelPath)
    {
        // Create Model's vao
        glGenVertexArrays(1, &vaoId);
        
        modelInit = loadModel(modelPath) == 0;
        if (!modelInit)
        {
            printf("Failed to load model: %s", modelPath);
        }


        // Bind to blank vao to protect it from external tampering
        glBindVertexArray(BLANK_VAO_ID);
    }

    // This exists for loading the default cube from internal memory
    // You should not be using this for loading regular models
    Model::Model(const std::vector<GLuint> &indexBuffer, 
                 const std::vector<glm::vec3> &vertBuffer,
                 const std::vector<glm::vec2> &uvBuffer,
                 const std::vector<glm::vec3> &normBuffer,
                 const Texture &texture)
    {
        glGenVertexArrays(1, &vaoId);
        glBindVertexArray(vaoId);
        glGenBuffers((GLuint)(BufferType::BUFFER_NUM), bufferIds); // Generate the Ids for the vertex, uv, index, and normal buffer
        
        meshes.emplace_back(MeshEntry(indexBuffer.size(), 0, 0, 0));
        textures.emplace_back(std::unique_ptr<Texture>(new Texture(texture)));

        gl_index_buffer = indexBuffer;
        gl_vert_buffer = vertBuffer;
        gl_uv_buffer = uvBuffer;
        gl_norm_buffer = normBuffer;

        populateOglBuffers();

        gl_index_buffer.clear();
        gl_vert_buffer.clear();
        gl_uv_buffer.clear();
        gl_norm_buffer.clear();

        modelInit = true;

        glBindVertexArray(BLANK_VAO_ID);
    }

    int Model::loadModel(const char* modelPath)
    {
        glBindVertexArray(vaoId);
        glGenBuffers((GLuint)(BufferType::BUFFER_NUM), bufferIds); // Generate the Ids for the vertex, uv, index, and normal buffer

        
        const aiScene *modelScenePtr = aiImportFile(modelPath, ASSIMP_LOAD_FLAGS);

        if (modelScenePtr != nullptr)
        {
            meshes.reserve(modelScenePtr->mNumMeshes);
            textures.reserve(modelScenePtr->mNumMaterials);

            uint64_t vertCnt, indiceCnt;
            initVertIndiceMesh(modelScenePtr, vertCnt, indiceCnt);
            
            // Reserve enough space to fit all of these meshes
            gl_index_buffer.reserve(indiceCnt);
            gl_vert_buffer.reserve(vertCnt);
            gl_uv_buffer.reserve(vertCnt);
            gl_norm_buffer.reserve(vertCnt);

            // Put all of the mesh data into the the ram buffers
            for (uint32_t i = 0; i < meshes.size(); ++i)
            {
                const aiVector3D ZERO_3D(0.0f, 0.0f, 0.0f); // Used if the mesh does not have UV coordinates

                const aiMesh *curMesh = modelScenePtr->mMeshes[i];
                for (uint64_t i = 0; i < curMesh->mNumVertices; ++i)
                {
                    const aiVector3D &curVert = curMesh->mVertices[i];
                    const aiVector3D &curUV   = curMesh->HasTextureCoords(0) ? curMesh->mTextureCoords[0][i] : ZERO_3D;
                    const aiVector3D &curNorm = curMesh->mNormals[i];

                    gl_vert_buffer.emplace_back(glm::vec3(curVert.x, curVert.y, curVert.z));
                    gl_uv_buffer.emplace_back(glm::vec2(curUV.x, curUV.y));
                    gl_norm_buffer.emplace_back(glm::vec3(curNorm.x, curNorm.y, curNorm.z));
                }

                for (uint64_t i = 0; i < curMesh->mNumFaces; ++i)
                {
                    const aiFace &curFace = curMesh->mFaces[i];
                    for (uint64_t j = 0; j < curFace.mNumIndices; ++j)
                    {
                        const uint32_t &curIndex  = curFace.mIndices[j];
                        gl_index_buffer.push_back(curIndex);
                    }
                }
            }

            // Populate VRAM with the information now in the RAM buffers
            populateOglBuffers();

            // Grab the model directory
            std::string modelDir = modelPath;
            std::regex modelDirRgx(R"(^[.\w/]*\/)");
            std::smatch searchMatch;

            if(std::regex_search(modelDir, searchMatch, modelDirRgx))
            {
                modelDir = searchMatch.str(0);
            }
            else
            {
                printf("Failed to grab model directory: %s\n", modelPath);
                return -1;
            }

            // Load textures
            for (uint16_t i = 0; i < modelScenePtr->mNumMaterials; ++i)
            {
                // For now we only handle diffuse textures
                const aiMaterial *curMaterial = modelScenePtr->mMaterials[i];

                // Only grab the texture if it is diffuse
                if (curMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
                {
                    aiString textureRelPath;
                    // Now we try to grab the texture path and load the new texture if successful
                    if (curMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &textureRelPath) == AI_SUCCESS)
                    {
                        std::string texturePath(textureRelPath.data);
                        texturePath = modelDir + texturePath;

                        textures.emplace_back(std::unique_ptr<Texture>(new Texture(texturePath.c_str())));
                        if (!textures.back()->isInit())
                        {
                            printf("Error while initializing texture: %s\n", texturePath);
                            textures.back().reset();
                        }

                    }
                }
                else
                {
                    textures.emplace_back(std::unique_ptr<Texture>(nullptr));
                }
            }

            // The model is now loaded. Free up space
            aiReleaseImport(modelScenePtr);
            gl_index_buffer.clear();
            gl_vert_buffer.clear();
            gl_uv_buffer.clear();
            gl_norm_buffer.clear();
        }
        else
        {
            printf("Error occurred while loading %s:\n\t%s", modelPath, aiGetErrorString());
            glBindVertexArray(BLANK_VAO_ID);
            return -1;
        }
        glBindVertexArray(BLANK_VAO_ID);
        
        return 0;
    }

    int Model::populateOglBuffers()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIds[(int)BufferType::INDEX_BUFF]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer.size() * sizeof(gl_index_buffer[0]), &gl_index_buffer[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, bufferIds[(int)BufferType::VERTEX_BUFF]);
        glBufferData(GL_ARRAY_BUFFER, gl_vert_buffer.size() * sizeof(gl_vert_buffer[0]), &gl_vert_buffer[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)SHADER_LOCATIONS::VERTEX);
        #define VERTEX_SIZE 3
        glVertexAttribPointer(
            (GLuint)SHADER_LOCATIONS::VERTEX, // Which Vertex Attrib
            VERTEX_SIZE,                      // Array size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalize?
            0,                                // stride
            (void *)0                     
        );

        glBindBuffer(GL_ARRAY_BUFFER, bufferIds[(int)BufferType::UV_BUFF]);
        glBufferData(GL_ARRAY_BUFFER, gl_uv_buffer.size() * sizeof(gl_uv_buffer[0]), &gl_uv_buffer[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)SHADER_LOCATIONS::UV);
        #define UV_SIZE 2
        glVertexAttribPointer(
            (GLuint)SHADER_LOCATIONS::UV,     // Which Vertex Attrib
            UV_SIZE,                          // Array size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalize?
            0,                                // stride
            (void *)0                     
        );

        glBindBuffer(GL_ARRAY_BUFFER, bufferIds[(int)BufferType::NORMAL_BUFF]);
        glBufferData(GL_ARRAY_BUFFER, gl_norm_buffer.size() * sizeof(gl_norm_buffer[0]), &gl_norm_buffer[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)SHADER_LOCATIONS::NORM);
        #define NORM_SIZE 3
        glVertexAttribPointer(
            (GLuint)SHADER_LOCATIONS::NORM,   // Which Vertex Attrib
            NORM_SIZE,                        // Array size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalize?
            0,                                // stride
            (void *)0                     
        );
        return 0;

    }

    int Model::initVertIndiceMesh(const aiScene *modelScenePtr, uint64_t &vertCnt, uint64_t &indiceCnt)
    {
        vertCnt = 0, indiceCnt = 0;
        // Determine the number of verts and indices by stepping through each mesh in the scene
        // Also update the mesh entries with the proper information
        for (uint32_t i = 0; i < modelScenePtr->mNumMeshes; ++i)
        {
            const aiMesh *curMesh = modelScenePtr->mMeshes[i];

            #define TRIANGLE_VERT_CNT 3
            meshes.emplace_back(MeshEntry(curMesh->mNumFaces * TRIANGLE_VERT_CNT, 
                                            indiceCnt, vertCnt, curMesh->mMaterialIndex));
            
            vertCnt += curMesh->mNumVertices;
            indiceCnt += meshes.back().numIndices;
        }

        return 0;
    }

    int Model::render()
    {
        glBindVertexArray(vaoId);

        // Step through each mesh
        for (uint32_t i = 0; i < meshes.size(); ++i)
        {
            MeshEntry *curMesh = &meshes[i];

            // Bind the texture if it exists
            if (textures[curMesh->textureIdx] != nullptr)
            {
                textures[curMesh->textureIdx]->bind(GL_TEXTURE0);
            }
            
            glDrawElementsBaseVertex(GL_TRIANGLES,                                      // What we are drawing 
                                     curMesh->numIndices,                               // How many indices to describe the verts, norms, and UVs
                                     GL_UNSIGNED_INT,                                   // The data type of the indices 
                                     (void *)(sizeof(GLuint) * curMesh->startIndexIdx), // Offset for the first index
                                     curMesh->startVertexIdx);                          // Offset for the first vertex

        }

        glBindVertexArray(BLANK_VAO_ID);
        return 0;
    }

    bool Model::getModelInit()
    {
        return modelInit;
    }

}

