#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb/stb_image.h"

namespace Render
{
    Texture::Texture(const char *texturePath, GLenum textureTarg)
    {
        textureTarget = textureTarg;
        textureInit = false;

        unsigned char *textureData = stbi_load(texturePath, &textureWidth, &textureHeight, &textureBPP, 0);
        
        if (!textureData)
        {
            printf("Could not load texture at path: %s\n", texturePath);
        }
        else
        {
            if (load(textureData))
            {
                textureInit = true;
            }
            stbi_image_free(textureData);
            
        }
    }

    Texture::Texture(unsigned char *textureBuff, uint32_t dataSize, GLenum textureTarg)
    {
        textureTarget = textureTarg;

        unsigned char *textureData = stbi_load_from_memory(textureBuff, dataSize, &textureWidth, &textureHeight, &textureBPP, 0);
        
        if (!textureData)
        {
            printf("Could not load texture from memory\n");
        }
        else
        {
            if (load(textureData))
            {
                textureInit = true;
            }
            stbi_image_free(textureData);
        }
    }

    int Texture::load(unsigned char* textureData)
    {
        const int MIN_TEXTURE_LEVELS = 5;

        if (textureTarget == GL_TEXTURE_2D)
        {
            glCreateTextures(textureTarget, 1, &textureId);

            int textureLevels = std::min(MIN_TEXTURE_LEVELS, (int)glm::log2((float)std::max(textureWidth, textureHeight)));

            switch (textureBPP)
            {
            case 1:
                glTextureStorage2D(textureId, textureLevels, GL_R8, textureWidth, textureHeight);
                glTextureSubImage2D(textureId, 0, 0, 0, textureWidth, textureHeight, GL_RED, GL_UNSIGNED_BYTE, textureData);
                break;
            case 2:
                glTextureStorage2D(textureId, textureLevels, GL_RG8, textureWidth, textureHeight);
                glTextureSubImage2D(textureId, 0, 0, 0, textureWidth, textureHeight, GL_RG, GL_UNSIGNED_BYTE, textureData);
                break;
            case 3:
                glTextureStorage2D(textureId, textureLevels, GL_RGB8, textureWidth, textureHeight);
                glTextureSubImage2D(textureId, 0, 0, 0, textureWidth, textureHeight, GL_RGB, GL_UNSIGNED_BYTE, textureData);
                break;
            case 4:
                glTextureStorage2D(textureId, textureLevels, GL_RGBA8, textureWidth, textureHeight);
                glTextureSubImage2D(textureId, 0, 0, 0, textureWidth, textureHeight, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
                break;
            default:
                printf("Texture BPP is not supported");
                return -1;
                break;
            }

            glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // This is how we pick which mip map to use when minifying
            glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // How we get a texture value when magnification is required. With linear we average texture elements
            glTextureParameteri(textureId, GL_TEXTURE_BASE_LEVEL, 0); // Set the base index of the lowest mip map
            glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_REPEAT); // How we handle values that go over the edge of the texture for coordinate s
            glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_REPEAT); // How we handle values that go over the edge of the texture for coordinate t

            glGenerateTextureMipmap(textureId);
            
            return 0;
        }
        return -1;
        
    }

    int Texture::bind(GLenum textureUnit)
    {
        glBindTextureUnit(textureUnit - GL_TEXTURE0, textureId);
        return 0;
    }

    bool Texture::isInit()
    {
        return textureInit;
    }
}