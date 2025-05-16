#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#define SDL_WINDOW_WIDTH 640
#define SDL_WINDOW_HEIGHT 480

typedef struct {
  SDL_Window *window;
  SDL_Surface *surface;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  if (!SDL_SetAppMetadata("Sample SDL3 Program", "1.0",
                          "com.davisraym.sample")) {
    return SDL_APP_FAILURE;
  }

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
  if (!as) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Couldn't initialize application state");
    return SDL_APP_FAILURE;
  }

  as->window = SDL_CreateWindow("Sample Program", SDL_WINDOW_WIDTH,
                                SDL_WINDOW_HEIGHT, 0);
  if (!as->window) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }

  as->surface = SDL_GetWindowSurface(as->window);
  if (!as->window) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't retrieve window surface: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_FillSurfaceRect(as->surface, NULL,
                           SDL_MapSurfaceRGB(as->surface, 0xFF, 0xFF, 0xFF))) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't fill window surface: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_UpdateWindowSurface(as->window)) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't update window surface: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }

  *appstate = as;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) { return SDL_APP_CONTINUE; }

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  default:
    break;
  }

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  if (appstate != NULL) {
    AppState *as = (AppState *)appstate;
    SDL_DestroyWindow(as->window);
    SDL_free(as);
  }
}
