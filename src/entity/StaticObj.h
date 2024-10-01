#ifndef STATIC_OBJ_H
#define STATIC_OBJ_H

#include <glm/glm.hpp>
#include "entity.h"

namespace gameEntities
{
    // A static game object is a renderable object that lacks the ability to interact (collide) with other game objects
    // It can be moved using the basic position, rotation, and scale setter functions but it does not have the 
    // breadth of support that a dynamic object has.
    class StaticObj : public Entity 
    {
        public:

            StaticObj(Render::MODEL_ID modelId, Render::SHADER_ID shaderId);

            int render();

            void setPosition(const glm::vec3& position);
            void offsetPosition(const glm::vec3& offsetVec);

            void setRotation(const glm::vec3& rotationVecXYZ_rad);
            void offsetRotation(const glm::vec3& rotationVecXYZ_rad);

            void setScale(const glm::vec3& absolute_scale);
            void offsetScale(const glm::vec3& relative_scale);


        protected:

            Render::MODEL_ID   modelId;
            Render::SHADER_ID shaderId;
            Render::Renderer *renderer;

            glm::mat4 position;
            glm::mat4 rotationMat; // Multiplied in the order Z, Y, X. Units in degrees
            glm::vec3 rotationVec; // In the order X, Y, Z and in units of degrees
            glm::mat4 scale;

            void buildRotationMat();
    };
    
}

#endif // STATIC_OBJ_H