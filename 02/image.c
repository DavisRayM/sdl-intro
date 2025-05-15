#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#define SDL_WINDOW_WIDTH 600
#define SDL_WINDOW_HEIGHT 480

typedef struct {
  SDL_Window *window;
  SDL_Surface *surface;
  SDL_Surface *image;
} AppState;

bool load_image(AppState *as) {
  bool res = true;
  as->image = SDL_LoadBMP("fixtures/hello_world.bmp");

  if (!as->image) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Unable to load image %s! SDL Error: %s\n",
                 "fixtures/hello_world.bmp", SDL_GetError());
    res = false;
  }

  return res;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  if (!SDL_SetAppMetadata("Image On Screen SDL3", "1.0",
                          "com.davisraym.image")) {
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
  *appstate = as;

  as->window = SDL_CreateWindow("SDL Image On Screen", SDL_WINDOW_WIDTH,
                                SDL_WINDOW_HEIGHT, 0);
  if (!as->window) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO,
                 "Couldn't create application window: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  as->surface = SDL_GetWindowSurface(as->window);
  if (!load_image(as)) {
    return SDL_APP_FAILURE;
  }

  SDL_BlitSurface(as->image, NULL, as->surface, NULL);
  SDL_UpdateWindowSurface(as->window);

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
    SDL_DestroySurface(as->image);
    SDL_DestroyWindow(as->window);
    SDL_free(as);
  }
}
