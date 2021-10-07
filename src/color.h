#ifndef COLOR
#define COLOR

#include <stdint.h>

// represent a color
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} Color;

// blend two colors in to one and return the struct
Color *blend_color(Color *source, Color *destination);

// pack a color in to a 32bit integer
uint32_t pack_color(Color *color, uint8_t alpha);

// bland two colors and pack the result in to a 32bit integer
uint32_t blend_and_pack(Color *source, Color *destination);

#endif
