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

typedef struct {
  asset_t base;
  TTF_Font *font;
  char *text;
  rgb_color_t color;
} text_asset_t;

typedef struct {
  asset_t base;
  SDL_Texture *texture;
} image_asset_t;

const size_t FONT_SIZE1 = 18;

/**
 * Allocates memory for an asset with the given parameters.
 *
 * @param ty the type of the asset
 * @param bounding_box the bounding box containing the location and dimensions
 * of the asset when it is rendered
 * @return a pointer to the newly allocated asset
 */
static asset_t *asset_init(asset_type_t ty, SDL_Rect bounding_box) {
  asset_t *new =
      malloc(ty == ASSET_IMAGE ? sizeof(image_asset_t) : sizeof(text_asset_t));
  assert(new);
  new->type = ty;
  new->bounding_box = bounding_box;
  return new;
}

asset_t *asset_make_image(const char *filepath, SDL_Rect bounding_box) {
  image_asset_t *image_asset = (image_asset_t *)asset_init(ASSET_IMAGE, bounding_box);
  image_asset->texture = (SDL_Texture*)asset_cache_obj_get_or_create(ASSET_IMAGE, filepath);
  return (asset_t *)image_asset;
}

asset_t *asset_make_text(const char *filepath, SDL_Rect bounding_box,
                         const char *text, rgb_color_t color) {
  text_asset_t *text_asset = (text_asset_t *)asset_init(ASSET_FONT, bounding_box);
  text_asset->font = asset_cache_obj_get_or_create(ASSET_FONT, filepath);
  assert(text_asset->font);
  text_asset->text = strdup(text);
  text_asset->color = color;  
  return (asset_t *)text_asset;
}

void asset_render(asset_t *asset) {
  assert(asset!=NULL);
  if (asset->type == ASSET_IMAGE) {
    image_asset_t *image_asset = (image_asset_t *)asset;
    vector_t location = {asset->bounding_box.x, asset->bounding_box.y};
    vector_t size = {asset->bounding_box.w, asset->bounding_box.h};
    sdl_render(image_asset->texture, location, size);
  } 
  else if (asset->type == ASSET_FONT) {
    text_asset_t *text_asset = (text_asset_t *)asset;
    vector_t location = {asset->bounding_box.x, asset->bounding_box.y};
    render_text(text_asset->font, text_asset->text, FONT_SIZE1, location);
  }
}

void asset_destroy(asset_t *asset) { free(asset);}
