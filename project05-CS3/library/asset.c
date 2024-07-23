#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

#include "asset.h"
#include "asset_cache.h"
#include "color.h"
#include "sdl_wrapper.h"

typedef struct asset {
  asset_type_t type;
  SDL_Rect bounding_box;
} asset_t;

typedef struct text_asset {
  asset_t base;
  TTF_Font *font;
  const char *text;
  rgb_color_t color;
} text_asset_t;

typedef struct image_asset {
  asset_t base;
  SDL_Texture *texture;
} image_asset_t;

typedef struct button_asset {
  asset_t base;
  image_asset_t *image_asset;
  text_asset_t *text_asset;
  button_handler_t handler;
  bool is_rendered;
} button_asset_t;

const size_t FONT_SIZE1 = 18;

static asset_t *asset_init(asset_type_t ty, SDL_Rect bounding_box) {
  asset_t *new;
  switch (ty) {
  case ASSET_IMAGE: {
    new = malloc(sizeof(image_asset_t));
    break;
  }
  case ASSET_FONT: {
    new = malloc(sizeof(text_asset_t));
    break;
  }
  case ASSET_BUTTON: {
    new = malloc(sizeof(button_asset_t));
    break;
  }
  default: {
    assert(false && "Unknown asset type");
  }
  }
  assert(new);
  new->type = ty;
  new->bounding_box = bounding_box;
  return new;
}

asset_type_t asset_get_type(asset_t *asset) { return asset->type; }

asset_t *asset_make_image(const char *filepath, SDL_Rect bounding_box) {
  image_asset_t *image_asset =
      (image_asset_t *)asset_init(ASSET_IMAGE, bounding_box);
  image_asset->texture =
      (SDL_Texture *)asset_cache_obj_get_or_create(ASSET_IMAGE, filepath);

  return (asset_t *)image_asset;
}

asset_t *asset_make_text(const char *filepath, SDL_Rect bounding_box,
                         const char *text, rgb_color_t color) {
  text_asset_t *text_asset =
      (text_asset_t *)asset_init(ASSET_FONT, bounding_box);
  text_asset->font = asset_cache_obj_get_or_create(ASSET_FONT, filepath);
  assert(text_asset->font);
  text_asset->text = strdup(text);
  text_asset->color = color;
  return (asset_t *)text_asset;
}

asset_t *asset_make_button(SDL_Rect bounding_box, asset_t *image_asset,
                           asset_t *text_asset, button_handler_t handler) {
  assert(image_asset == NULL || image_asset->type == ASSET_IMAGE);
  assert(text_asset == NULL || text_asset->type == ASSET_FONT);
  button_asset_t *button_asset = asset_init(ASSET_BUTTON, bounding_box);
  button_asset->image_asset = (image_asset_t *)image_asset;
  button_asset->handler = handler;
  button_asset->text_asset = (text_asset_t *)text_asset;
  button_asset->is_rendered = false;
  asset_cache_register_button(button_asset);
  return button_asset;
}

void asset_on_button_click(asset_t *button, state_t *state, double x,
                           double y) {
  button_asset_t *button_asset = (button_asset_t *)button;
  if (button_asset->is_rendered == false) {
    return;
  }
  if (click_in_bounds(button, x, y)) {
    button_asset->is_rendered = false;
    button_asset->handler(state);
  }
}

bool click_in_bounds(asset_t *button, double x, double y) {
  SDL_Rect box = button->bounding_box;
  if (x > box.x && x < box.x + box.w && y > box.y && y < box.y + box.h) {
    return true;
  }
  return false;
}

void asset_render(asset_t *asset) {
  assert(asset != NULL);
  if (asset->type == ASSET_IMAGE) {
    image_asset_t *image_asset = (image_asset_t *)asset;
    vector_t location = {asset->bounding_box.x, asset->bounding_box.y};
    vector_t size = {asset->bounding_box.w, asset->bounding_box.h};
    sdl_render(image_asset->texture, location, size);
  } else if (asset->type == ASSET_FONT) {
    text_asset_t *text_asset = (text_asset_t *)asset;
    vector_t location = {asset->bounding_box.x, asset->bounding_box.y};
    render_text(text_asset->font, text_asset->text, FONT_SIZE1, location);
  } else if (asset->type == ASSET_BUTTON) {
    button_asset_t *button_asset = (button_asset_t *)asset;
    vector_t location = {asset->bounding_box.x, asset->bounding_box.y};
    vector_t size = {asset->bounding_box.w, asset->bounding_box.h};
    sdl_render(button_asset->image_asset->texture, location, size);
    if (button_asset->text_asset != NULL) {
      asset_render((asset_t *)button_asset->text_asset);
    }
    button_asset->is_rendered = true;
  }
}

void asset_destroy(asset_t *asset) { free(asset); }
