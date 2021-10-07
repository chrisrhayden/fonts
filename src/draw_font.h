#ifndef DRAW_FONT
#define DRAW_FONT

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

#include <stdbool.h>
#include <stdint.h>

#include "color.h"
#include "font_cache.h"

typedef struct {
    size_t x;
    size_t y;
    size_t width;
    size_t height;
    size_t c_width;
    size_t c_height;
    size_t character_box;
    size_t buffer_len;
    uint32_t *buffer;
} SourceData;

bool draw_character(FT_Bitmap *bitmap, SourceData *source_data, Color *fg,
                    Color *bg);

bool draw_characters(BitMapCache *bitmap_cache, SourceData *data, Color *fg,
                     Color *bg, Characters *characters);

uint32_t pack_color(Color *color, uint8_t alpha);

#endif
