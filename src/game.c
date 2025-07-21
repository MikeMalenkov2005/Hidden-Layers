#include <game.h>

bool LoadGameTexture(GameState *state, GameTexture id, const char *name, GLsizei levels, GLenum wrapping, bool interpolatePixels, bool interpolateLevels)
{
  if (id < 0 || id >= TEXTURE_COUNT) return SDL_SetError("Invalid texture id (%d)", id);
  if (!(state->textures[id] = LoadTexture2D(OpenResource(state, name, "rb"), levels, true))) return false;
  SetTextureInterpolation(state->textures[id], interpolatePixels, interpolateLevels);
  SetTextureWrapping(state->textures[id], wrapping);
  glGenerateTextureMipmap(state->textures[id]);
  return true;
}

bool LoadGameShader(GameState *state, GameShader id, const char *vsh, const char *fsh)
{
  Shader shaders[2];
  if (!(shaders[0] = CompileShader(OpenResource(state, vsh, "r"), GL_VERTEX_SHADER, true))) return 0;
  if (!(shaders[1] = CompileShader(OpenResource(state, fsh, "r"), GL_FRAGMENT_SHADER, true))) return (glDeleteShader(shaders[0]), 0);
  ShaderProgram program = (state->shaders[id] = LinkShaderProgram(shaders, SDL_arraysize(shaders)));
  DeleteShaders(shaders, SDL_arraysize(shaders));
  return (bool)program;
}

GameState *CreateGameState(const char *config, bool resource)
{
  GameState *state = SDL_malloc(sizeof(*state));
  if (!state) return (void*)SDL_SetError("Out of memory");
  SDL_zerop(state);

  if (!(state->basePath = SDL_GetBasePath())) return (DestroyGameState(state), NULL);

  /* TODO: LOAD AND USE CONFIG FILES */

  SDL_WindowFlags flags = SDL_WINDOW_OPENGL;
  int width = 640, height = 480;

  if (!(state->window = SDL_CreateWindow("Hidden Layers", width, height, flags))) return (DestroyGameState(state), NULL);
  if (!(state->context = SDL_GL_CreateContext(state->window))) return (DestroyGameState(state), NULL);
  if (!SDL_GL_MakeCurrent(state->window, state->context)) return (DestroyGameState(state), NULL);
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) return (DestroyGameState(state), (void*)SDL_SetError("GLAD failed to load OpenGL 4.6"));

  if (!LoadGameTexture(state, TEXTURE_TILEMAP, "tilemap.png", 4, GL_REPEAT, true, true)) return (DestroyGameState(state), NULL);
  if (!LoadGameShader(state, SHADER_TILE, "tile.vsh", "tile.fsh")) return (DestroyGameState(state), NULL);

  return state;
}

void DestroyGameState(GameState *state)
{
  if (state)
  {
    if (state->context)
    {
      DeleteShaderPrograms(state->shaders, SHADER_COUNT);
      glDeleteTextures(TEXTURE_COUNT, state->textures);
      SDL_GL_DestroyContext(state->context);
    }
    if (state->window) SDL_DestroyWindow(state->window);
    SDL_free(state);
  }
}

SDL_IOStream *OpenResource(const GameState *state, const char *name, const char *mode)
{
  if (!state || !name || !state->basePath) return NULL;
  size_t size = SDL_strlen(state->basePath) + SDL_strlen(name) + 1;
  char *path = SDL_malloc(size);
  if (!path) return (void*)SDL_SetError("Out of memory");
  size_t offset = SDL_utf8strlcpy(path, state->basePath, size);
  SDL_utf8strlcpy(path + offset, name, size - offset);
  return SDL_IOFromFile(path, mode);
}
