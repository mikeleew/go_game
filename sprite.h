#pragma once
#include <glm-1.0.1/glm/glm.hpp>


class Sprite
{
    public:
        Sprite() {}
        Sprite(glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color, glm::vec2 texCoords, glm::vec2 texSize)
            : position(position), size(size), rotate(rotate), color(color), texCoords(texCoords), texSize(texSize) {}
        void operator=(const Sprite& other) // assignment operator
        {
            if (this == &other) return; // self-assignment check
            // Copy the values from the other object
        
            position = other.position;
            size = other.size;
            rotate = other.rotate;
            color = other.color;
            texCoords = other.texCoords;
            texSize = other.texSize;
        }
        glm::vec2 position;
        glm::vec2 size;
        float rotate;
        glm::vec3 color;
        glm::vec2 texCoords;
        glm::vec2 texSize;
};