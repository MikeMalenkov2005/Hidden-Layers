#include <shader.h>

Shader CompileShader(SDL_IOStream *src, GLenum type, bool closeio)
{
  if (!src) return 0;
  Shader shader = glCreateShader(type);
  if (!shader) return (Shader)SDL_SetError("Failed to create a shader (OpenGL %u)", glGetError());
  char *source = SDL_LoadFile_IO(src, NULL, closeio);

  if (!source) return (glDeleteShader(shader), (Shader)SDL_SetError("Failed to load a shader source file (ID %u)", shader));
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  SDL_free(source);

  int status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status) return shader;
  glDeleteShader(shader);
  return (Shader)SDL_SetError("Failed to compile a shader (ID %u)", shader);
}

void DeleteShaders(const Shader *shaders, size_t count)
{
  for (size_t i = 0; i < count; ++i) if (shaders[i]) glDeleteShader(shaders[i]);
}

ShaderProgram LinkShaderProgram(const Shader *shaders, size_t count)
{
  if (!shaders || !count) return (ShaderProgram)SDL_SetError("Invalid parameter");
  ShaderProgram program = glCreateProgram();
  if (!program) return (ShaderProgram)SDL_SetError("Failed to create a shader program (OpenGL %u)", glGetError());

  for (size_t i = 0; i < count; ++i) if (shaders[i]) glAttachShader(program, shaders[i]);
  glLinkProgram(program);

  int status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status) return program;
  glDeleteProgram(program);
  return (ShaderProgram)SDL_SetError("Failed to link a shader program (ID %u)", program);
}

void DeleteShaderPrograms(const ShaderProgram *programs, size_t count)
{
  for (size_t i = 0; i < count; ++i) if (programs[i]) glDeleteProgram(programs[i]);
}
