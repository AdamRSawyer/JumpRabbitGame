#include "StaticObj.h"

namespace gameEntities
{
    StaticObj::StaticObj(Render::MODEL_ID modelId, Render::SHADER_ID shaderId) : modelId(modelId), shaderId(shaderId)
    {
        // Initialize Attributes
        entityAttr[EntityAttributes::CAN_RENDER] = true;
    
        // Get reference to renderer singleton
        renderer = Render::Renderer::getRenderer();

        // Set to default worldspace position
        setPosition(glm::vec3(4.0, 3.0f, 10.0f));
        rotationVec = glm::vec3(0.0f);
        setRotation(rotationVec);
        setScale(glm::vec3(1.0f));
    }

    // Adds the entities model to the render queue
    int StaticObj::render()
    {
        renderer->queueDraw(modelId, shaderId, position, rotationMat, scale);
        return 0;
    }

    void StaticObj::setPosition(const glm::vec3& positionVec)
    {
        position = glm::translate(glm::mat4(1.0f), positionVec);
    }

    void StaticObj::offsetPosition(const glm::vec3& offsetVec)
    {
        position = glm::translate(position, offsetVec);
    }

    void StaticObj::setRotation(const glm::vec3& rotationVecXYZ_deg)
    {
        rotationVec = rotationVecXYZ_deg;

        buildRotationMat();
    }

    void StaticObj::offsetRotation(const glm::vec3& rotationVecXYZ_deg)
    {
            rotationVec += rotationVecXYZ_deg;
            buildRotationMat();
    }

    void StaticObj::buildRotationMat()
    {
        glm::mat4 rotateX = glm::rotate(rotationVec[0], glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotateY = glm::rotate(rotationVec[1], glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotateZ = glm::rotate(rotationVec[2], glm::vec3(0.0f, 0.0f, 1.0f));

        rotationMat = rotateZ * rotateY * rotateX;
    }

    void StaticObj::setScale(const glm::vec3& absolute_scale)
    {
        scale = glm::scale(glm::mat4(1.0f), absolute_scale);
    }

    void StaticObj::offsetScale(const glm::vec3& relative_scale)
    {
        scale = glm::scale(scale, relative_scale);
    }
}