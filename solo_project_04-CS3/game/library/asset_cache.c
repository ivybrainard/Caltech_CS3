#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

#include "asset.h"
#include "asset_cache.h"
#include "list.h"
#include "sdl_wrapper.h"

static list_t *ASSET_CACHE;

const size_t FONT_SIZE = 18;
const size_t INITIAL_CAPACITY = 5;

typedef struct {
  asset_type_t type;
  const char *filepath;
  void *obj;
} entry_t;

static void asset_cache_free_entry(entry_t *entry) {
  if (entry->type == ASSET_IMAGE) {
    SDL_DestroyTexture(entry->obj);
  } else if (entry->type == ASSET_FONT) {
    TTF_CloseFont(entry->obj);
  } else if (entry->type == ASSET_BUTTON) {
    SDL_DestroyTexture(entry->obj);
    TTF_CloseFont(entry->obj);
  } else {
    return;
  }
  free(entry);
}

void asset_cache_init() {
  ASSET_CACHE =
      list_init(INITIAL_CAPACITY, (free_func_t)asset_cache_free_entry);
}

void asset_cache_destroy() { list_free(ASSET_CACHE); }

void *asset_cache_obj_get_or_create(asset_type_t ty, const char *filepath) {
  void *obj = asset_cache_check_entry(ty, filepath);
  if (obj != NULL) {
    return obj;
  } else {
    entry_t *new_entry = malloc(sizeof(entry_t));
    assert(new_entry != NULL);
    new_entry->type = ty;
    new_entry->filepath = filepath;
    if (ty == ASSET_IMAGE) {
      new_entry->obj = sdl_display(filepath);
    } else if (ty == ASSET_FONT) {
      new_entry->obj = load_font(filepath, FONT_SIZE);
    } else {
      return NULL;
    }
    list_add(ASSET_CACHE, new_entry);
    return new_entry->obj;
  }
}

void asset_cache_register_button(asset_t *button) {
  if (button == NULL) {
    return;
  }
  entry_t *new_entry = malloc(sizeof(entry_t));
  assert(new_entry != NULL);
  new_entry->type = ASSET_BUTTON;
  new_entry->obj = button;
  new_entry->filepath = NULL;
  list_add(ASSET_CACHE, new_entry);
}

void asset_cache_handle_buttons(state_t *state, double x, double y) {
  size_t size = list_size(ASSET_CACHE);
  for (size_t i = 0; i < size; i++) {
    entry_t *entry = list_get(ASSET_CACHE, i);
    if (entry->type == ASSET_BUTTON) {
      asset_on_button_click(entry->obj, state, x, y);
    }
  }
}

void *asset_cache_check_entry(asset_type_t ty, const char *filepath) {
  size_t size = list_size(ASSET_CACHE);
  for (size_t i = 0; i < size; i++) {
    entry_t *entry = list_get(ASSET_CACHE, i);
    if (entry->type == ty) {
      if (entry && strcmp(entry->filepath, filepath) == 0) {
        return entry->obj;
      }
    }
  }
  return NULL;
}
