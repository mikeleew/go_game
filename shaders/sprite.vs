#version 330 core
layout (location = 0) in vec4 vertex; // vec2 vertex; vec2 texCoord
//layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

  
//out vec3 vertexColor; // specify a color output to the fragment shader
out vec2 TexCoord; // specify a texture coordinate output to the fragment shader

uniform mat4 model; // model matrix
uniform mat4 projection; // projection matrix
uniform mat2 texMatrix; // texture matrix
uniform vec2 texOffset; // texture offset

void main()
{
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0); // see how we directly give a vec3 to vec4's constructor
    //vertexColor = aColor; // set the output variable to a dark-red color
    TexCoord = texMatrix*vertex.zw + texOffset; // pass the texture coordinate to the fragment shader
    
}