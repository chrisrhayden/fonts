#include <locale.h>
#include <math.h>

#include "hashmap.h"

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "color.h"
#include "draw_font.h"
#include "font_cache.h"

bool test_font() {
    printf("testing fonts\n");
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

    printf("font cache initialized\n");

    FT_Bitmap *bitmap;

    wchar_t key = 33;

    printf("getting value\n");
    get_value_hashmap(font_cache->bitmap_cache, &key, bitmap);

    if (bitmap == NULL) {
        return false;
    }
    printf("got value\n");

    // int character_box = (CHARACTER_LEN / 2) + 2;
    size_t character_box = 18;
    if (character_box * character_box <= CHARACTER_LEN) {
        printf("cant hold enough characters characters %ld - len %ld\n",
               character_box * character_box, CHARACTER_LEN);

        drop_font_cache(font_cache);

        return false;
    }

    // max character size by the amount of characters per row/column
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

    for (int i = 0; i < (width * height); ++i) {
        data.buffer[i] = color;
    }

    printf("drawing characters\n");
    bool success_bool = draw_characters(font_cache->bitmap_cache, &data, &red,
                                        &green, &characters);
    if (success_bool == false) {
        printf("failed to draw font\n");
    }

    drop_font_cache(font_cache);

    printf("drew font to buffer\n");
    char *filename = "./out/ascii_test.png";

    int success = stbi_write_png(filename, data.width, data.height, 4,
                                 data.buffer, data.width * sizeof(uint32_t));

    printf("created png\n");
    free(buffer);

    if (success) {
        return true;
    } else {
        return false;
    }
}

int main() {
    setlocale(LC_CTYPE, "");

    if (test_font()) {
        return 0;
    } else {
        return 1;
    }
}
