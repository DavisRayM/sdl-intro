#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#define SDL_WINDOW_WIDTH 640
#define SDL_WINDOW_HEIGHT 480

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  return SDL_APP_SUCCESS;
}

SDL_AppResult SDL_AppIterate(void *appstate) { return SDL_APP_CONTINUE; }

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  switch (event->type) {
  default:
    break;
  }

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  if (appstate != NULL) {
    AppState *as = (AppState *)appstate;
    SDL_free(as);
  }
}
