#include <framework.h>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  GameState *state = appstate;
  switch (event->type)
  {
  case SDL_EVENT_KEY_DOWN:
    if (event->key.key == SDLK_ESCAPE && !PushQuitEvent()) return SDL_APP_FAILURE;
    break;
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
  GameState *state = appstate;
  Uint64 delta = SDL_GetTicks() - state->ticks;
  state->ticks += delta;

  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(state->window);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) || !NET_Init()) return SDL_APP_FAILURE;

  GameState *state = SDL_malloc(sizeof(*state));
  if (!state) return (SDL_SetError("Out of memory"), SDL_APP_FAILURE);
  *appstate = SDL_zerop(state);

  if (!(state->basePath = SDL_GetBasePath())) return SDL_APP_FAILURE;



  SDL_WindowFlags flags = SDL_WINDOW_OPENGL;
  int width = 640, height = 480;

  if (!(state->window = SDL_CreateWindow("Hidden Layers", width, height, flags))) return SDL_APP_FAILURE;
  if (!(state->context = SDL_GL_CreateContext(state->window))) return SDL_APP_FAILURE;
  if (!SDL_GL_MakeCurrent(state->window, state->context)) return SDL_APP_FAILURE;
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) return (SDL_SetError("GLAD failed to load OpenGL 4.6"), SDL_APP_FAILURE);

  glClearColor(1, 0, 1, 1);

  state->ticks = SDL_GetTicks();
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  if (result == SDL_APP_FAILURE) SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Fatal Error: %s", SDL_GetError());
  for (GameState *state = appstate; state; state = NULL)
  {
    if (state->context) SDL_GL_DestroyContext(state->context);
    if (state->window) SDL_DestroyWindow(state->window);
    SDL_free(state);
  }
  NET_Quit();
  SDL_Quit();
}

char *GetResourcePath(const GameState *state, const char *name)
{
  if (!state || !name || !state->basePath) return NULL;
  size_t size = SDL_strlen(state->basePath) + SDL_strlen(name) + 1;
  char *result = SDL_malloc(size);
  if (result)
  {
    size_t offset = SDL_utf8strlcpy(result, state->basePath, size);
    SDL_utf8strlcpy(result + offset, name, size - offset);
  }
  return result;
}

bool PushQuitEvent()
{
  SDL_Event event = { .quit = { .type = SDL_EVENT_QUIT, .timestamp = SDL_GetTicks() } };
  return SDL_PushEvent(&event);
}
