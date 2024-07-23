#ifndef __ASSET_H__
#define __ASSET_H__

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <color.h>
#include <stddef.h>

typedef enum { ASSET_IMAGE, ASSET_FONT } asset_type_t;

typedef struct asset asset_t;

/**
 * Allocates memory for an image asset with the given parameters.
 *
 * @param filepath the filepath to the image file
 * @param bounding_box the bounding box containing the location and dimensions
 * of the text when it is rendered
 * @return a pointer to the newly allocated image asset
 */
asset_t *asset_make_image(const char *filepath, SDL_Rect bounding_box);

/**
 * Allocates memory for a text asset with the given parameters.
 *
 * @param filepath the filepath to the .ttf file
 * @param bounding_box the bounding box containing the location and dimensions
 * of the text when it is rendered
 * @param text the text to render
 * @param color the color of the text
 * @return a pointer to the newly allocated text asset
 */
asset_t *asset_make_text(const char *filepath, SDL_Rect bounding_box,
                         const char *text, rgb_color_t color);

/**
 * Renders the asset to the screen.
 * @param asset the asset to render
 */
void asset_render(asset_t *asset);

/**
 * Frees the memory allocated for the asset.
 * @param asset the asset to free
 */
void asset_destroy(asset_t *asset);

#endif // #ifndef __ASSET_H__
