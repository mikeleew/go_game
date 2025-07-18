#version 330 core
out vec4 color;
  
//in vec3 vertexColor; // the input variable from the vertex shader (same name and same type)  
in vec2 TexCoord; // the input variable from the vertex shader (same name and same type)


uniform sampler2D image; // the texture sampler
uniform vec3 spriteColor; // the color of the sprite

void main()
{
    color = texture(image, TexCoord) * vec4(spriteColor, 1.0); // combine texture color with vertex color
} 