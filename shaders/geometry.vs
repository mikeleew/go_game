#version 330 core
layout (location = 0) in vec2 vertex; // the position variable has attribute position 0

//out vec3 vertexColor;

uniform mat4 projection;
uniform mat4 model;

void main()
{
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0); // see how we directly give a vec3 to vec4's constructor
    //vertexColor = aColor; // set the output variable to a dark-red color
}