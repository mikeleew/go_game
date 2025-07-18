#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm-1.0.1/glm/glm.hpp>
#include <glm-1.0.1/glm/gtc/matrix_transform.hpp>
#include <glm-1.0.1/glm/gtc/type_ptr.hpp>
#include <glm-1.0.1/glm/gtc/matrix_inverse.hpp>
#include "texture.h"
#include "shader.h"
#include "sprite.h"





class SpriteRenderer
{
    public:
     
        SpriteRenderer(std::string texture_path, std::string vertexShader_path, std::string fragmentShader_path)
        {
            // Initialize the sprite renderer with the specified texture and shaders
            shader.init(vertexShader_path.c_str(), fragmentShader_path.c_str());
            texture.init(texture_path.c_str());
            initRenderData();
            projection = glm::mat4(1.0f);
            projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
            shader.SetMatrix4("projection", projection, true);
            shader.SetInteger("image", 0, true);
            texWidth = texture.getWidth();
            texHeight = texture.getHeight();
        }

        SpriteRenderer()
        {
            quadVAO = 0;
            quadVBO = 0;
            quadEBO = 0;
            shader.ID = 0;
            projection = glm::mat4(1.0f);

        }

        int init(std::string texture_path, std::string vertexShader_path, std::string fragmentShader_path)
        {
            // Initialize the sprite renderer with the specified texture and shaders
            shader.init(vertexShader_path.c_str(), fragmentShader_path.c_str());
            texture.init(texture_path.c_str());
            initRenderData();
            projection = glm::mat4(1.0f);
            projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
            shader.SetMatrix4("projection", projection, true);
            shader.SetInteger("image", 0, true);
            texWidth = texture.getWidth();
            texHeight = texture.getHeight();
            return 1;
        }
        
  
        void deallocate()
        {
            glDeleteVertexArrays(1, &quadVAO);
            glDeleteBuffers(1, &quadVBO);
            glDeleteBuffers(1, &quadEBO);
            GLboolean enabled;
            glGetBooleanv(GL_BLEND, &enabled);
            if(enabled)
                glDisable(GL_BLEND);  // turn off blending for transparency
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            glDeleteProgram(shader.ID);
            texture.deallocate();
        }

        ~SpriteRenderer()
        {
            // Destructor
       
          
         

        }

        void initRenderData()
        {
            float vertices [] = {
                // Positions         // Texture Coords
                0.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                0.0f, 1.0f, 0.0f, 1.0f
            };
            unsigned int indices[] = {
                0, 1, 2,
                0, 2, 3
            };
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glGenBuffers(1, &quadEBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glEnable(GL_BLEND);  // turn on blending for transparency
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // set blending function
        }
        
        void DrawSprite(glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color, glm::vec2 texCoords, glm::vec2 texSize)
        {
                // prepare transformations
                this->shader.use();
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(position, 0.0f));  
            
                model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
                model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
                model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
            
                model = glm::scale(model, glm::vec3(size, 1.0f)); 
                
                this->shader.SetMatrix4("model", model);
                this->shader.SetVector3f("spriteColor", color);

                glm::mat2 texTransform = glm::mat2(texSize.x/texWidth, 0.0f,
                                                   0.0f, texSize.y/texHeight);
              

                glm::vec2 texOffset = glm::vec2(texCoords.x/texWidth, texCoords.y/texHeight);

                this->shader.SetMatrix2("texMatrix", texTransform);
                this->shader.SetVector2f("texOffset", texOffset);
                
                glActiveTexture(GL_TEXTURE0);
                texture.bind();
            
                glBindVertexArray(this->quadVAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
          }
  
          void DrawSprite(Sprite& sprite)
          {
              this->DrawSprite(sprite.position, sprite.size, sprite.rotate, sprite.color, sprite.texCoords, sprite.texSize);
          }

    private:
        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int quadEBO;
        Shader shader;
        Texture texture;
        glm::mat4 projection;
        unsigned int texWidth;
        unsigned int texHeight;
      
};


class GeometryRenderer
{
    public:
        GeometryRenderer(std::string vertexShader_path, std::string fragmentShader_path)
        {
            // Initialize the sprite renderer with the specified texture and shaders
            shader.init(vertexShader_path.c_str(), fragmentShader_path.c_str());
            initRenderData();
            projection = glm::mat4(1.0f);
            projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
            shader.SetMatrix4("projection", projection, true);
        }

        GeometryRenderer()
        {
            quadVAO = 0;
            quadVBO = 0;
            quadEBO = 0;
            shader.ID = 0;
            projection = glm::mat4(1.0f);

        }

        int init(std::string vertexShader_path, std::string fragmentShader_path)
        {
            // Initialize the sprite renderer with the specified texture and shaders
            shader.init(vertexShader_path.c_str(), fragmentShader_path.c_str());
            initRenderData();
            projection = glm::mat4(1.0f);
            projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
            shader.SetMatrix4("projection", projection, true);
            return 1;
        }
    
        void deallocate()
        {
            glDeleteVertexArrays(1, &quadVAO);
            glDeleteBuffers(1, &quadVBO);
            glDeleteBuffers(1, &quadEBO);
            glDeleteVertexArrays(1, &lineVAO);
            glDeleteBuffers(1, &lineVBO);
            glDeleteBuffers(1, &lineEBO);
            GLboolean enabled;
            glGetBooleanv(GL_BLEND, &enabled);
            if(enabled)
                glDisable(GL_BLEND);  // turn off blending for transparency
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            glDeleteProgram(shader.ID);
        }

        ~GeometryRenderer()
        {
            // Destructor
         
          
         

        }
        void drawBox(glm::vec2 start, glm::vec2 end, float width, glm::vec4 color)
        {
            this->shader.use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(start, 0.0f));  
            //glm::vec2 size = glm::vec2(end.x - start.x, end.y - start.y);
            float length = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
            glm::vec2 size = glm::vec2(width, length);
            glm::vec2 delta = glm::vec2(end.x - start.x, end.y - start.y);
            float rotate = 0.f;
            if(delta.x != 0)
                rotate = std::atan(-double(delta.y)/ double(delta.x)) * 180.0f / M_PI;
            //std::cout<< rotate << std::endl;
            //model = glm::translate(model, glm::vec3(0.5*size.x, 0.5*size.y, 0.0f)); 
            model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
            //model = glm::translate(model, glm::vec3(-0.5*size.x, -0.5*size.y, 0.0f));
            model = glm::scale(model, glm::vec3(size, 1.0f));
            this->shader.SetMatrix4("model", model);
            this->shader.SetVector4f("vertexColor", color);
            
            glBindVertexArray(this->quadVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        
        void drawLine(glm::vec2 start, glm::vec2 end, float width, glm::vec4 color)
        {
            if(start.x > end.x)
            {
                std::swap(start, end);
            }
            this->shader.use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(start, 0.0f));  
            ///glm::vec2 size = glm::vec2(end.x - start.x, end.y - start.y);
            float length = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
            //glm::vec2 size = glm::vec2(width, length);
            glm::vec2 delta = glm::vec2(end.x - start.x, end.y - start.y);
            float rotate = 0.f;
            if(delta.x != 0)
                rotate = std::atan(double(delta.y)/ double(delta.x)) * 180.0f / M_PI;
            else if(delta.y != 0)
                rotate = 90.0f;
            //std::cout<< rotate << std::endl;
            //model = glm::translate(model, glm::vec3(0.5*size.x, 0.5*size.y, 0.0f)); 
            model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
            //delta = glm::vec2(delta.x, delta.y);
            glm::vec2 size = glm::vec2(length, length);
            model = glm::scale(model, glm::vec3(size, 1.0f));
            //model = glm::translate(model, glm::vec3(-0.5*size.x, -0.5*size.y, 0.0f));
            //model = glm::scale(model, glm::vec3(size, 1.0f));
            this->shader.SetMatrix4("model", model);
            this->shader.SetVector4f("vertexColor", color);
            
            glBindVertexArray(this->lineVAO);
            glLineWidth(width);
            glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    
    private:
        void initRenderData()
        {
          float vertices [] = {
                // Positions         // Texture Coords
                0.0f, 0.0f, 
                1.0f, 0.0f,
                1.0f, 1.0f, 
                0.0f, 1.0f, 
            };
            unsigned int indices[] = {
                0, 1, 2,
                0, 2, 3
            };
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glGenBuffers(1, &quadEBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
         
        
            float line_vertices[] = {
                // Positions         // Texture Coords
                0.0f, 0.0f, 
                1.0f, 0.0f,
            };
            unsigned int line_indices[] = {
                0, 1
            };
            glGenVertexArrays(1, &lineVAO);
            glGenBuffers(1, &lineVBO);
            glGenBuffers(1, &lineEBO);
            glBindVertexArray(lineVAO);
            glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices), line_vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(line_indices), line_indices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            glEnable(GL_BLEND);  // turn on blending for transparency
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // set blending funct

        }

        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int quadEBO;
        Shader shader;
        glm::mat4 projection;

        unsigned int lineVAO;
        unsigned int lineVBO;
        unsigned int lineEBO;
      

};