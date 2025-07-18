#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Triangle
{
    public:
        Triangle()
        {
            // Define the vertices of the triangle
            float vertices[] = {
                // Positions         // Colors
                0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
               -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
            };
            // Generate and bind a Vertex Array Object (VAO)
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            // Generate and bind a Vertex Buffer Object (VBO)
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // Define the vertex attributes
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // Unbind the VAO and VBO
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        void draw()
        {
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    private:
        unsigned int VAO;
        unsigned int VBO;
};
