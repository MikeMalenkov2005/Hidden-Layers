#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3_net/SDL_net.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

struct GameState
{
  SDL_Window *window;
  SDL_GLContext context;
  const char *basePath;
  Uint64 ticks;
};

typedef struct GameState GameState;

char *GetResourcePath(const GameState *state, const char *name);

bool PushQuitEvent();

#endif
