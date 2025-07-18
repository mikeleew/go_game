#pragma once
#include "board.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

class GameResources
{
    public:
        GameResources()
        {
            init_window();
            xpos= 0.0;
            ypos= 0.0;
            _mouse_clicked = false;
            _mouse_button_down = false;
        }
        GameResources(unsigned int size, std::string texture_path, std::string sprite_vertexShader_path, std::string sprite_fragmentShader_path,
                       std::string geometry_vertexShader_path, std::string geometry_fragmentShader_path)
        {
            // Load resources here
            _mouse_clicked = false;
            _mouse_button_down = false;
            xpos= 0.0;
            ypos= 0.0;
            init_window();
            spriteRenderer.init(texture_path, sprite_vertexShader_path, sprite_fragmentShader_path);
            geometryRenderer.init(geometry_vertexShader_path, geometry_fragmentShader_path);
            board.init(size);
            
        }
        void init(unsigned int size, std::string texture_path, std::string sprite_vertexShader_path, std::string sprite_fragmentShader_path,
                       std::string geometry_vertexShader_path, std::string geometry_fragmentShader_path)
        {
            // Load resources here
            
            //init_window();
            spriteRenderer.init(texture_path, sprite_vertexShader_path, sprite_fragmentShader_path);
            geometryRenderer.init(geometry_vertexShader_path, geometry_fragmentShader_path);
            board.init(size);
        }
        ~GameResources()
        {
            // Unload resources here
        }
        void deallocate()
        {
            spriteRenderer.deallocate();
            geometryRenderer.deallocate();
            glfwDestroyWindow(window); // Do I need to destroy the window here?
            glfwTerminate();
        }
        void init_window()
        {
            // Load resources here
             //std::cout << "hello in init_window" << std::endl;
            if (!glfwInit())
            {
                    std::cout << "Could not initialize GLFW" << std::endl;
                    return;
            }
           
            window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Go game", NULL, NULL);
           // std::cout << window << std::endl;
            if (!window)
            {
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                return;
            }
            glfwMakeContextCurrent(window);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                std::cout << "Failed to initialize GLAD" << std::endl;
                glfwTerminate();
                return;
            }
            glViewport(0, 0, 800, 600);
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
            //glfwSetMouseButtonCallback(window, mouse_button_callback);     
        }
        bool LoadSpriteRenderer(std::string texture_path, std::string vertexShader_path, std::string fragmentShader_path)
        {
            spriteRenderer.init(texture_path, vertexShader_path, fragmentShader_path);
            return true;
        }
        bool LoadGeometryRenderer(std::string vertexShader_path, std::string fragmentShader_path)
        {
            geometryRenderer.init(vertexShader_path, fragmentShader_path);
            return true;
        }
        void processInput()
        {
            if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
            //if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
             //   std::cout << "Space key pressed" << std::endl;
            int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
            if(state == GLFW_PRESS && _mouse_button_down == false)
            {
                glfwGetCursorPos(window, &xpos, &ypos);
                _mouse_button_down = true;
                _mouse_clicked = true;
            }
            else if(state == GLFW_RELEASE)
            {
                _mouse_button_down = false;
                _mouse_clicked = false;
               
            }
            else
                _mouse_clicked = false;
        }
        void draw()
        {
            board.drawBoard(spriteRenderer);
            board.drawGrid(geometryRenderer);
            board.drawDots(spriteRenderer);
            board.draw(spriteRenderer);
          
        }
        void drawLastMove(unsigned int x, unsigned int y)
        {
            float fx = float(x) + 1.0f;
            float fy = float(y) + 1.0f;
            geometryRenderer.drawLine(glm::vec2((fx-0.2) * grid_width, fy * grid_height), glm::vec2((fx+0.2)* grid_width, fy*grid_height), 2.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            geometryRenderer.drawLine(glm::vec2(fx * grid_width, (fy-0.2) * grid_height), glm::vec2(fx * grid_width, (fy+0.2)*grid_height), 2.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        bool window_should_close()
        {
            return glfwWindowShouldClose(window);
        }


        void clear_screen()
        {
            glClearColor(0.0, 0.0, 0.0, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

        }
        void swap_buffers()

        {
            glfwSwapBuffers(window);
        }
        void poll_events()
        {
            glfwPollEvents();
        }
        void setBoardState(Tile** tiles)
        {
           board.setBoardState(tiles);
        }
        Tile ** getBoardState()
        {
            return board.getBoardState();
        }
        void get_window_size(int &width, int &height)
        {
            glfwGetWindowSize(window, &width, &height);
        }
        SpriteRenderer spriteRenderer;
        GeometryRenderer geometryRenderer;
        GLFWwindow* window;
        Board board;
        bool _mouse_button_down;
        bool _mouse_clicked;
        bool _draw_last_move = false;
        float grid_width = float(GRID_WIDTH);
        float grid_height = float(GRID_HEIGHT);
        double xpos, ypos;
           
};


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << "Mouse clicked at: (" << xpos << ", " << ypos << ")" << std::endl;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   glViewport(0, 0, width, height);
}  