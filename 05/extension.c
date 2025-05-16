#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#define SDL_WINDOW_WIDTH 640
#define SDL_WINDOW_HEIGHT 480

typedef struct {
  SDL_Window *window;
  SDL_Surface *surface;
  SDL_Surface *imageSurface;
} AppState;

SDL_Surface *load_image_surface(AppState *as, const char *path) {
  SDL_Surface *optimizedSurface = NULL;
  SDL_Surface *surface = IMG_Load(path);

  if (!surface) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't load image at %s: %s", path,
                 SDL_GetError());
  } else {
    optimizedSurface = SDL_ConvertSurface(surface, as->surface->format);
    if (!optimizedSurface) {
      SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't convert image surface: %s",
                   SDL_GetError());
    }

    SDL_DestroySurface(surface);
  }

  return optimizedSurface;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  if (!SDL_SetAppMetadata("SDL Extension Sample", "1.0",
                          "com.davisraym.extension.sdl3.image")) {
    SDL_Log("Couldn't set application metadata: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
  if (!as) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Couldn't initialize application state: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_Window *window = SDL_CreateWindow("SDL Image Extension", SDL_WINDOW_WIDTH,
                                        SDL_WINDOW_HEIGHT, 0);
  if (!window) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO,
                 "Couldn't create application window: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  as->window = window;
  as->surface = SDL_GetWindowSurface(window);
  SDL_Surface *imageSurface = load_image_surface(as, "fixtures/loaded.png");

  if (!imageSurface) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't load image: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }
  as->imageSurface = imageSurface;

  *appstate = as;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  AppState *as = (AppState *)appstate;

  if (!SDL_BlitSurfaceScaled(as->imageSurface, NULL, as->surface, NULL,
                             SDL_SCALEMODE_LINEAR)) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't blit image to surface: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_UpdateWindowSurface(as->window)) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't update window surface: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }

  return SDL_APP_CONTINUE;
}

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
