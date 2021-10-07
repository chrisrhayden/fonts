// #include <freetype2/ft2build.h>
// #include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

#include <SDL2/SDL_endian.h>

#include "draw_font.h"
#include "font_cache.h"
#include "hashmap.h"

bool draw_character(FT_Bitmap *bitmap, SourceData *source_data, Color *fg,
                    Color *bg) {
    bool success = true;
    int rows = bitmap->rows;
    int bm_width = bitmap->width;
    int s_width = source_data->width;

    size_t s_x = source_data->x;
    size_t s_y = source_data->y;

    for (size_t y = 0; y < rows && success; ++y) {
        for (size_t x = 0; x < bm_width && success; ++x) {

            size_t index_bitmap = x + (y * bm_width);
            size_t index_source = s_x + (s_y * s_width);

            if (index_source >= (source_data->width * source_data->height)) {
                printf("index out of bounds\n");
                success = false;
            } else {

                if (bitmap->buffer[index_bitmap] > 0) {
                    fg->alpha = bitmap->buffer[index_bitmap];

                    source_data->buffer[index_source] = blend_and_pack(fg, bg);
                }

                ++s_x;
            }
        }

        ++s_y;
        s_x = source_data->x;
    }

    return success;
}

bool draw_characters(BitMapCache *bitmap_cache, SourceData *data, Color *fg,
                     Color *bg, Characters *characters) {
    bool success = true;

    GlyphMap *glyph_map = NULL;
    int count = 1;

    ++data->x;
    ++data->y;

    printf("making characters\n");
    for (int i = 0; i < characters->len && success; ++i) {
        get_value_hashmap(bitmap_cache, &characters->char_array[i], glyph_map);

        if (glyph_map == NULL) {
            printf("could not get bitmap\n");
            success = false;
            break;
        }

        // set the character in the middle of the character box
        // int offset_w = (data->c_width - bitmap->width) / 2;
        // int offset_h = (data->c_height - bitmap->rows) / 2;

        data->x += glyph_map->bitmap_left;

        // keep the current y value as we need to reset the y to the same
        // place so it can draw from the top of the row each time
        int temp_y = data->y;
        data->y += data->c_height - glyph_map->bitmap_top;

        success = draw_character(glyph_map->bitmap, data, fg, bg);

        data->y = temp_y;

        // when the count reaches 10 move to the next row
        if (count == data->character_box) {
            data->x = 1;
            data->y += data->c_height + 2;

            count = 1;
        } else {
            // we are in the same row so move the x value and increment the
            // count
            data->x += (data->c_width - glyph_map->bitmap_left) + 2;

            ++count;
        }
    }

    return success;
}
