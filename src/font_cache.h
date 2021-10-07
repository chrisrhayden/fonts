#ifndef C_MECH_FONT_CACHE
#define C_MECH_FONT_CACHE

#include <stdint.h>
#include <wchar.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

#include "hashmap.h"

typedef struct {
    FT_Bitmap *bitmap;
    size_t bitmap_left;
    size_t bitmap_top;
} GlyphMap;

HASHMAP(BitMapCache, wchar_t, GlyphMap);

typedef struct {
    int max_width;
    int max_height;
    FT_FaceRec *face;
    FT_Library library;
    BitMapCache *bitmap_cache;
} FontCache;

typedef struct {
    size_t len;
    wchar_t *char_array;
} Characters;

extern size_t CHARACTER_LEN;
extern wchar_t CHARACTER_ARRAY[254];

FontCache *init_font_cache(char font_path[], int point_size,
                           Characters *characters);

void drop_font_cache(FontCache *cache);

#endif
