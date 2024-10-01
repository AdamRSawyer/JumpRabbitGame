#ifndef ENTITY_H
#define ENTITY_H

#include "entityDefs.h"
#include <map>
#include <cstdio>

// CAN_RENDER Includes
// =======================================================================================
#include "../render/RendererDefs.h"
#include "../render/Renderer.h"
// =======================================================================================

namespace gameEntities
{
    class Entity
    {
        public:

        bool hasAttr(gameEntities::EntityAttributes attr)
        {
            return entityAttr.count(attr) && entityAttr[attr] == true;
        };

        // CAN_RENDER Public
        // =======================================================================================
        virtual int render()
        {
            if (hasAttr(gameEntities::EntityAttributes::CAN_RENDER))
            {
                fprintf(stderr, "Subclass requires implementation of render() member function.");
            }
            else
            {
                fprintf(stderr, "This subclass cannot be rendered.");
            }
            return -1;
        }

        virtual void setPosition(const glm::vec3& position)
        {
            if (hasAttr(gameEntities::EntityAttributes::CAN_RENDER))
            {
                fprintf(stderr, "Subclass requires implementation of setPosition() member function.");
            }
            else
            {
                fprintf(stderr, "This subclass does not have the CAN_RENDER attribute and therefore its position cannot be set.");
            }
        }

        virtual void offsetPosition(const glm::vec3& offsetVec)
        {
            if (hasAttr(gameEntities::EntityAttributes::CAN_RENDER))
            {
                fprintf(stderr, "Subclass requires implementation of offsetPosition() member function.");
            }
            else
            {
                fprintf(stderr, "This subclass does not have the CAN_RENDER attribute and therefore its position cannot be set.");
            }
        }

        virtual void setRotation(const glm::vec3& rotationVecXYZ_rad)
        {
            if (hasAttr(gameEntities::EntityAttributes::CAN_RENDER))
            {
                fprintf(stderr, "Subclass requires implementation of setRotation() member function.");
            }
            else
            {
                fprintf(stderr, "This subclass does not have the CAN_RENDER attribute and therefore its rotation cannot be set.");
            }
        }

        virtual void offsetRotation(const glm::vec3& rotationVecXYZ_rad)
        {
            if (hasAttr(gameEntities::EntityAttributes::CAN_RENDER))
            {
                fprintf(stderr, "Subclass requires implementation of offsetRotation() member function.");
            }
            else
            {
                fprintf(stderr, "This subclass does not have the CAN_RENDER attribute and therefore its rotation cannot be set.");
            }
        }

        virtual void setScale(const glm::vec3& absolute_scale)
        {
            if (hasAttr(gameEntities::EntityAttributes::CAN_RENDER))
            {
                fprintf(stderr, "Subclass requires implementation of setScale() member function.");
            }
            else
            {
                fprintf(stderr, "This subclass does not have the CAN_RENDER attribute and therefore its scale cannot be set.");
            }
        }

        virtual void offsetScale(const glm::vec3& relative_scale)
        {
            if (hasAttr(gameEntities::EntityAttributes::CAN_RENDER))
            {
                fprintf(stderr, "Subclass requires implementation of relativeScale() member function.");
            }
            else
            {
                fprintf(stderr, "This subclass does not have the CAN_RENDER attribute and therefore its scale cannot be set.");
            }
        }
        // =======================================================================================

        virtual ~Entity() {};
        
        protected:

        std::map<EntityAttributes, bool> entityAttr; // Attributes determine the methods an entity can perform. You should not be executing a method without confirming that the appropriate attribute exists for that method

    };
}

#endif // ENTITY_H
