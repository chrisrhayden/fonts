#include <locale.h>
#include <math.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "hashmap.h"

#include "color.h"
#include "draw_font.h"
#include "font_cache.h"

#include "log.h"

bool test_font() {
    log_trace("testing fonts");
    // size_t size = 50;
    size_t size = 16;

    // char path[] = "assets/FiraCode-Regular.ttf";
    // char path[] = "assets/Hack-Regular.ttf";
    // char path[] = "assets/Hack Regular Nerd Font Complete.ttf";
    char path[] = "assets/unscii-8.ttf";
    // char path[] = "assets/unscii-16.ttf";

    Characters characters;
    characters.char_array = CHARACTER_ARRAY;
    characters.len = CHARACTER_LEN;

    FontCache *font_cache = init_font_cache(path, size, &characters);

    if (font_cache == NULL) {
        return false;
    }

    log_trace("font cache initialized");

    // the amount of characters for the rows and width
    size_t character_box = 18;
    if (character_box * character_box <= CHARACTER_LEN) {
        log_trace("cant hold enough characters characters %ld - len %ld",
                  character_box * character_box, CHARACTER_LEN);

        drop_font_cache(font_cache);

        return false;
    }

    // max character size by the amount of characters per row/column
    // add 2 for each character for a 2 pixel border around each character
    size_t width = (font_cache->max_width + 2) * character_box;
    size_t height = (font_cache->max_height + 2) * character_box;

    size_t buffer_len = (width * height);
    uint32_t *buffer = malloc(sizeof(uint32_t) * buffer_len);

    SourceData data = {
        .x = 0,
        .y = 0,
        .width = width,
        .height = height,
        .c_width = font_cache->max_width,
        .c_height = font_cache->max_height,
        .character_box = character_box,
        .buffer_len = buffer_len,
        .buffer = buffer,
    };

    Color red = {.red = 178, .green = 34, .blue = 34, .alpha = 255};
    Color green = {.red = 10, .green = 255, .blue = 0, .alpha = 255};

    uint32_t color = pack_color(&green, 0);

    for (int i = 0; i < data.buffer_len; ++i) {
        data.buffer[i] = color;
    }

    log_trace("drawing characters");
    bool success_bool = draw_characters(font_cache->bitmap_cache, &data, &red,
                                        &green, &characters);
    if (success_bool == false) {
        log_error("failed to draw font");
    }

    drop_font_cache(font_cache);

    log_trace("drawing characters to buffer");
    char *filename = "./out/ascii_test.png";

    int success = stbi_write_png(filename, data.width, data.height, 4,
                                 data.buffer, data.width * sizeof(uint32_t));

    log_trace("created png");
    free(data.buffer);

    if (success) {
        return true;
    } else {
        return false;
    }
}

int main() {
    setlocale(LC_CTYPE, "");

    log_set_level(LOG_TRACE);

    if (test_font()) {
        return 0;
    } else {
        return 1;
    }
}
