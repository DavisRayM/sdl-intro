#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#define SDL_WINDOW_WIDTH 640
#define SDL_WINDOW_HEIGHT 480

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
} AppState;

/// Loads texture from a file
SDL_Texture *load_texture(AppState *as, const char *path);

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  if (!SDL_SetAppMetadata("SDL 2D Rendering", "1.0", "com.davisraym.sdl.2d")) {
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
                 "COuldn't initialize application state: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("SDL 2D Rendering", SDL_WINDOW_WIDTH,
                                   SDL_WINDOW_HEIGHT, 0, &as->window,
                                   &as->renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO,
                 "COuldn't create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_SetRenderDrawColor(as->renderer, 0xFF, 0xFF, 0xFF, 0xFF)) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't set render draw color: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_Texture *texture = load_texture(as, "fixtures/texture.png");
  if (!texture) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't load texture: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }
  as->texture = texture;
  *appstate = as;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  AppState *as = (AppState *)appstate;

  if (!SDL_RenderClear(as->renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't clear render: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_RenderTexture(as->renderer, as->texture, NULL, NULL)) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't render texture: %s",
                 SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_RenderPresent(as->renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO,
                 "Couldn't update application screen: %s", SDL_GetError());
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

    SDL_DestroyTexture(as->texture);
    SDL_DestroyWindow(as->window);
    SDL_DestroyRenderer(as->renderer);
    SDL_free(as);
  }
}

SDL_Texture *load_texture(AppState *as, const char *path) {
  SDL_Texture *texture = IMG_LoadTexture(as->renderer, path);
  if (!texture) {
    SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Couldn't load image %s: %s", path,
                 SDL_GetError());
  }

  return texture;
}
