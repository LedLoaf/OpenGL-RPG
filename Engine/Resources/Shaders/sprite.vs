#version 460 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 coords;

out vec2 TexCoords;
uniform mat4 projection;      // Screen coordinates to normalized

void main()
{
    TexCoords = coords;
    gl_Position = projection * vec4(position, 0.0, 1.0);
}