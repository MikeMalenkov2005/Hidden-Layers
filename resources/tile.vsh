#version 460 core

in vec2 position;

out vec2 uv;

layout (location = 0) uniform float scale;

void main()
{
  uv = position * vec2(0.5, -0.5) + 0.5;
  gl_Position = vec4(position * scale, 0.0, 1.0);
}
