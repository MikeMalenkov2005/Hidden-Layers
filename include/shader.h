#ifndef SHADER_H
#define SHADER_H

#include <framework.h>

typedef GLuint Shader, ShaderProgram;

Shader CompileShader(SDL_IOStream *src, GLenum type, bool closeio);

void DeleteShaders(const Shader *shaders, size_t count);

ShaderProgram LinkShaderProgram(const Shader *shaders, size_t count);

void DeleteShaderPrograms(const ShaderProgram *programs, size_t count);

#endif
