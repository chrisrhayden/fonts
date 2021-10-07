#include "log.h"

#include "draw_font.h"
#include "font_cache.h"
#include "hashmap.h"

bool draw_character(FT_Bitmap *bitmap, SourceData *source_data, Color *fg,
                    Color *bg) {
    bool success = true;

    int bm_height = bitmap->rows;
    int bm_width = bitmap->width;
    int s_width = source_data->width;

    size_t s_x = source_data->x;
    size_t s_y = source_data->y;

    for (size_t y = 0; y < bm_height && success; ++y) {
        for (size_t x = 0; x < bm_width && success; ++x) {

            size_t index_bitmap = x + (y * bm_width);
            size_t index_source = s_x + (s_y * s_width);

            if (index_source >= source_data->buffer_len) {
                log_error("index out of bounds");
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

    int count = 1;
    GlyphMap *glyph_map = NULL;

    // increment the x/y by one to put a one pixel border around the texter map
    ++data->x;
    ++data->y;

    for (int i = 0; i < characters->len && success; ++i) {
        get_value_hashmap(bitmap_cache, &characters->char_array[i], glyph_map);

        if (glyph_map == NULL) {
            log_error("could not get bitmap form hashmap");
            success = false;
        } else {
            // set the left padding to correctly draw it to the buffer
            data->x += glyph_map->bitmap_left;

            // keep the current y value as we need to reset the y to the same
            // place so it can draw from the top of the row each time
            int temp_y = data->y;
            // set the glyph to the correct hight from the bottom
            data->y += data->c_height - glyph_map->bitmap_top;

            // draw the character to the buffer
            success = draw_character(glyph_map->bitmap, data, fg, bg);

            // reset the y
            data->y = temp_y;

            // when the count reaches 10 move to the next row
            if (count == data->character_box) {
                data->x = 1;
                // move the data->y by the glyph max hight plus 2 to set a 2
                // pixel border
                data->y += data->c_height + 2;

                // reset the count
                count = 1;
            } else {
                // we are in the same row so move the x value by the glyph width
                // minus the left padding plus 2 for a 2 pixel border arouns
                // each glyph
                data->x += (data->c_width - glyph_map->bitmap_left) + 2;

                // increment the count
                ++count;
            }
        }
    }

    return success;
}
