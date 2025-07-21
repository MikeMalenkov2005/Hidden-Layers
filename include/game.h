#ifndef GAME_H
#define GAME_H

#include <framework.h>
#include <texture.h>
#include <shader.h>

typedef enum GameTexture
{
  TEXTURE_TILEMAP,
  TEXTURE_COUNT
} GameTexture;

typedef enum GameShader
{
  SHADER_TILE,
  SHADER_COUNT
} GameShader;

typedef struct GameState
{
  SDL_Window *window;
  SDL_GLContext context;
  const char *basePath;
  Uint64 ticks;
  Texture textures[TEXTURE_COUNT];
  ShaderProgram shaders[SHADER_COUNT];

  float scale;
  float xmove;
  float ymove;

} GameState;

typedef enum GameInput
{
  INPUT_
} GameInput;

GameState *CreateGameState(const char *config, bool resource);

void DestroyGameState(GameState *state);

SDL_IOStream *OpenResource(const GameState *state, const char *name, const char *mode);

#endif
