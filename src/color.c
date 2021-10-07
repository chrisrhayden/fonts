#include <endian.h>
#include <stdio.h>
#include <stdlib.h>

#include "color.h"

Color *blend_color(Color *source, Color *destination) {
    uint8_t alpha =
        source->alpha + (destination->alpha * (255 - source->alpha) / 255);

    uint8_t red = ((source->red * source->alpha) +
                   (destination->red * destination->alpha) *
                       (255 - source->alpha) / 255) /
                  alpha;

    uint8_t green = ((source->green * source->alpha) +
                     (destination->green * destination->alpha) *
                         (255 - source->alpha) / 255) /
                    alpha;

    uint8_t blue = ((source->blue * source->alpha) +
                    (destination->blue * destination->alpha) *
                        (255 - source->alpha) / 255) /
                   alpha;

    Color *color = malloc(sizeof(*color));

    color->alpha = alpha;
    color->red = red;
    color->green = green;
    color->blue = blue;

    return color;
}

uint32_t pack_color(Color *color, uint8_t alpha) {
    if (alpha == 0) {
        alpha = color->alpha;
    }

#if __BYTE_ORDER == __BIG_ENDIAN
    uint32_t packed_color =
        (color->red << 24) + (color->green << 16) + (color->blue << 8) + alpha;
#else
    uint32_t packed_color =
        (alpha << 24) + (color->blue << 16) + (color->green << 8) + color->red;
#endif

    return packed_color;
}

uint32_t blend_and_pack(Color *source, Color *destination) {
    Color *color = blend_color(source, destination);

    uint32_t packed = pack_color(color, 0);

    free(color);

    return packed;
}
