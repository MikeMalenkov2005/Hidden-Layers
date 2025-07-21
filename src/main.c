#include <game.h>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  GameState *state = appstate;
  switch (event->type)
  {
  // case SDL_EVENT_KEY_UP:
  //   switch (event->key.key)
  //   {
  //   case SDLK_W:
  //     state->input &= ~(1 << 0);
  //     break;
  //   case SDLK_A:
  //     state->input &= ~(1 << 1);
  //     break;
  //   case SDLK_S:
  //     state->input &= ~(1 << 2);
  //     break;
  //   case SDLK_D:
  //     state->input &= ~(1 << 3);
  //     break;
  //   }
  //   break;

  case SDL_EVENT_KEY_DOWN:
    switch (event->key.key)
    {
    case SDLK_ESCAPE:
      if (!PushQuitEvent()) return SDL_APP_FAILURE;
      break;
    case SDLK_R:
      glUniform1f(0, state->scale = 1);
      break;
    // case SDLK_W:
    //   state->input |= (1 << 0);
    //   break;
    // case SDLK_A:
    //   state->input |= (1 << 1);
    //   break;
    // case SDLK_S:
    //   state->input |= (1 << 2);
    //   break;
    // case SDLK_D:
    //   state->input |= (1 << 3);
    //   break;
    }
    break;

  case SDL_EVENT_MOUSE_WHEEL:
    glUniform1f(0, state->scale *= 1 + event->wheel.y * 0.1f);
    break;

  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
  static Sint8 model[8] = { -1, -1, 1, -1, 1, 1, -1, 1 };
  GameState *state = appstate;
  Uint64 delta = SDL_GetTicks() - state->ticks;
  state->ticks += delta;

  glClear(GL_COLOR_BUFFER_BIT);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_BYTE, GL_FALSE, 2, model);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableVertexAttribArray(0);
  SDL_GL_SwapWindow(state->window);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) || !NET_Init() || !TTF_Init()) return SDL_APP_FAILURE;
  GameState *state = CreateGameState("config.cfg", true);
  if (!(*appstate = state)) return SDL_APP_FAILURE;

  glClearColor(0, 0, 0, 1);
  glUseProgram(state->shaders[SHADER_TILE]);
  glBindTextureUnit(0, state->textures[TEXTURE_TILEMAP]);
  glUniform1f(0, state->scale = 1);

  state->ticks = SDL_GetTicks();
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  GameState *state = appstate;
  if (result == SDL_APP_SUCCESS)
  {
    glBindTextureUnit(0, 0);
    glUseProgram(0);
  }
  else SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Fatal Error: %s", SDL_GetError());
  DestroyGameState(state);
  TTF_Quit();
  NET_Quit();
  SDL_Quit();
}

bool PushQuitEvent()
{
  SDL_Event event = { .quit = { .type = SDL_EVENT_QUIT, .timestamp = SDL_GetTicks() } };
  return SDL_PushEvent(&event);
}
