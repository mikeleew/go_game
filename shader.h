#pragma once
#include <glad/glad.h>
//#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION

#include <glm-1.0.1/glm/glm.hpp>
#include <glm-1.0.1/glm/gtc/matrix_transform.hpp>
#include <glm-1.0.1/glm/gtc/type_ptr.hpp>
#include <glm-1.0.1/glm/gtc/matrix_inverse.hpp>
#include <fstream>
#include <iostream>
#include <random>
#include <string>


int loadShaderSourceCode(const char* vertexPath, const char* fragmentPath, std::string& vertexCode, std::string& fragmentCode)
{
    // Load the vertex and fragment shaders from the specified paths
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);

    if (!vertexFile.is_open() || !fragmentFile.is_open())
    {
        std::cerr << "Error: Could not open shader files." << std::endl;
        return -1;
    }

    // Read the vertex shader code
    std::string line;
    while (std::getline(vertexFile, line))
    {
        vertexCode += line + "\n";
    }
    // Read the fragment shader code
    while (std::getline(fragmentFile, line))
    {
        fragmentCode += line + "\n";
    }
    // Close the files

    vertexFile.close();
    fragmentFile.close();

    return 0;

    // Compile shaders and create shader program (not implemented in this snippet)
}
class Shader
{
    public:
        unsigned int ID;
        Shader() {}
        Shader(const char* vertexPath, const char* fragmentPath)
        {
            // Load and compile shaders  
            if (loadShaderSourceCode(vertexPath, fragmentPath, vertexCode, fragmentCode) != 0)
            {
                std::cerr << "Error loading shader source code." << std::endl;
                return;
            }
            // Compile shaders and create shader program (not implemented in this snippet)
            vertexShader = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
            fragmentShader = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            int success;
            char infoLog[512];
            // Check for linking errors
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                std::cerr << "Error linking shader program: " << infoLog << std::endl;
            }
            // Delete the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);        
            ID = shaderProgram;
        }
        int init(const char* vertexPath, const char* fragmentPath)
        {
            // Load and compile shaders
            if (loadShaderSourceCode(vertexPath, fragmentPath, vertexCode, fragmentCode) != 0)
            {
                std::cout << "Error loading shader source code." << std::endl;
                return - 1;
            }
            // Compile shaders and create shader program (not implemented in this snippet)
            vertexShader = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
            fragmentShader = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            int success;
            char infoLog[512];
            // Check for linking errors
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                std::cout << "Error linking shader program: " << infoLog << std::endl;
                return -1;
            }
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            ID = shaderProgram;
            return 0;
        }
        void use()
        {
            glUseProgram(shaderProgram);
        }
        void SetFloat(const char *name, float value, bool useShader)
        {
            if (useShader)
                this->use();
            glUniform1f(glGetUniformLocation(this->ID, name), value);
        }
        void SetInteger(const char *name, int value, bool useShader)
        {
            if (useShader)
                this->use();
            glUniform1i(glGetUniformLocation(this->ID, name), value);
        }
        void SetVector2f(const char *name, float x, float y, bool useShader)
        {
            if (useShader)
                this->use();
            glUniform2f(glGetUniformLocation(this->ID, name), x, y);
        }
        void SetVector2f(const char *name, const glm::vec2 &value, bool useShader = false)
        {
            if (useShader)
                this->use();
            glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
        }
        void SetVector3f(const char *name, float x, float y, float z, bool useShader = false)
        {
            if (useShader)
                this->use();
            glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
        }
        void SetVector3f(const char *name, const glm::vec3 &value, bool useShader = false)
        {
            if (useShader)
                this->use();
            glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
        }
        void SetVector4f(const char *name, float x, float y, float z, float w, bool useShader = false)
        {
            if (useShader)
                this->use();
            glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
        }
        void SetVector4f(const char *name, const glm::vec4 &value, bool useShader = false)
        {
            if (useShader)
                this->use();
            glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
        }
        void SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader = false)
        {
            if (useShader)
                this->use();
            glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
        }
        void SetMatrix2(const char *name, const glm::mat2 &matrix, bool useShader = false)
        {
            if (useShader)
                this->use();
            glUniformMatrix2fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
        }
    private:
        unsigned int compileShader(const char* shaderCode, GLenum shaderType)
        {
            unsigned int shader;
            shader = glCreateShader(shaderType);
            glShaderSource(shader, 1, &shaderCode, NULL);
            glCompileShader(shader);

            // Check for compilation errors
            int success;
            char infoLog[512];
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                std::cerr << "Error compiling shader: " << infoLog << std::endl;
            }
            return shader;
        }
        std::string vertexCode;
        std::string fragmentCode;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;
};