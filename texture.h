#pragma once
#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image/stb_image.h"

class Texture
{
    public:
        Texture() {
            // Default constructor
            texture = 0;
            _width = 0;
            _height = 0;

        }
        Texture(const char* texturePath)
        {
            // Load the texture
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            // Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Load the image
            int width, height, nrChannels;
            unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cerr << "Failed to load texture" << std::endl;
            }
            stbi_image_free(data);
        }
        void deallocate()
        {
            // Deallocate the texture
            glDeleteTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        ~Texture()
        {
            // Destructor
        }
        int init(const char* texturePath)
        {
            // Load the texture
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            // Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Load the image
            int width, height, nrChannels;
            unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cerr << "Failed to load texture" << std::endl;
                return -1;
            }
            stbi_image_free(data);
            _width = width;
            _height = height;
            return 0;
        }
        void bind()
        {
            //glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
        }

        int getWidth() const
        {
            return _width;
        }
        int getHeight() const
        {
            return _height;
        }
    private:
        unsigned int texture;
        unsigned int _width;
        unsigned int _height;
};
