#ifndef COLOR
#define COLOR

#include <stdint.h>

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} Color;

uint32_t pack_color(Color *color, uint8_t alpha);
Color *blend_color(Color *source, Color *destination);
uint32_t blend_and_pack(Color *source, Color *destination);

#endif
