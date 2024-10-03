#ifndef TEXTURE_H
#define TEXTURE_H

#include <algorithm>
#include <stdint.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

namespace Render
{

    class Texture
    {
        public:

        Texture(const char *texturePath, GLenum textureTarg = GL_TEXTURE_2D);
        Texture(unsigned char *textureBuff, uint32_t dataSize, GLenum textureTarg = GL_TEXTURE_2D);
        int load(unsigned char* textureData);
        int bind(GLenum textureUnit);
        
        bool isInit();

        private:
            GLuint textureId;  
            GLenum textureTarget;   
            int textureWidth;
            int textureHeight;
            int textureBPP;   

            bool textureInit;
    };
}


#endif // TEXTURE_H