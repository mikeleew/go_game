#version 330 core
out vec4 FragColor;
  
//in vec3 vertexColor; // the input variable from the vertex shader (same name and same type)  

uniform vec4 vertexColor;  //color of triangle with alpha channel

void main()
{
    //FragColor = vec4(vertexColor, 1.0); // set the output color to the vertex color
    FragColor = vertexColor;
} 