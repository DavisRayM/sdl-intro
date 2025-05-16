#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_surface.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#define SDL_WINDOW_WIDTH 640
#define SDL_WINDOW_HEIGHT 480

enum KeyPressSurface {
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};

typedef struct {
  SDL_Window *window;
  SDL_Surface *windowSurface;
  SDL_Surface *keyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
  SDL_Surface *currentSurface;
} AppState;

static const struct {
  const int key;
  const char *path;
} keypress_surface_bmp[] = {
    {KEY_PRESS_SURFACE_DEFAULT, "fixtures/press.bmp"},
    {KEY_PRESS_SURFACE_UP, "fixtures/up.bmp"},
    {KEY_PRESS_SURFACE_DOWN, "fixtures/down.bmp"},
    {KEY_PRESS_SURFACE_LEFT, "fixtures/left.bmp"},
    {KEY_PRESS_SURFACE_RIGHT, "fixtures/right.bmp"},
};

SDL_Surface *load_resource(AppState *as, const char *path) {
  SDL_Surface *optimizedResource = NULL;
  SDL_Surface *resource = SDL_LoadBMP(path);

  if (!resource) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to load resource %s: %s", path,
                 SDL_GetError());
  } else {
    optimizedResource = SDL_ConvertSurface(resource, as->windowSurface->format);
    if (!optimizedResource) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                   "Couldn't convert convert loaded surface: %s",
                   SDL_GetError());
    }
    SDL_DestroySurface(resource);
  }

  return optimizedResource;
}

void handle_keypress(AppState *as, SDL_Scancode scancode) {
  switch (scancode) {
  case SDL_SCANCODE_UP:
    as->currentSurface = as->keyPressSurfaces[KEY_PRESS_SURFACE_UP];
    break;
  case SDL_SCANCODE_DOWN:
    as->currentSurface = as->keyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
    break;
  case SDL_SCANCODE_LEFT:
    as->currentSurface = as->keyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
    break;
  case SDL_SCANCODE_RIGHT:
    as->currentSurface = as->keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
    break;
  default:
    as->currentSurface = as->keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
    break;
  }
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  if (!SDL_SetAppMetadata("Keypress SDL3", "0.1",
                          "com.davisraym.sdl.keypress")) {
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
  *appstate = as;

  as->window =
      SDL_CreateWindow("Keypress", SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT, 0);
  if (!as->window) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO,
                 "Couldn't create application window: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  as->windowSurface = SDL_GetWindowSurface(as->window);
  if (!as->windowSurface) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't retrieve window surface: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }

  for (size_t i = 0; i < SDL_arraysize(keypress_surface_bmp); i++) {
    SDL_Surface *surface = load_resource(as, keypress_surface_bmp[i].path);
    if (!surface) {
      return SDL_APP_FAILURE;
    }

    as->keyPressSurfaces[keypress_surface_bmp[i].key] = surface;
  }

  as->currentSurface = as->keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  AppState *as = (AppState *)appstate;

  if (!SDL_BlitSurface(as->currentSurface, NULL, as->windowSurface, NULL)) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO,
                 "Couldn't blit current surface onto window surface: %s",
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
  AppState *as = (AppState *)appstate;
  switch (event->type) {
  case SDL_EVENT_KEY_DOWN:
    handle_keypress(as, event->key.scancode);
    break;
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
    for (size_t i = 0; i < SDL_arraysize(keypress_surface_bmp); i++) {
      SDL_DestroySurface(as->keyPressSurfaces[keypress_surface_bmp[i].key]);
    }

    SDL_DestroyWindow(as->window);
    SDL_free(as);
  }
}
