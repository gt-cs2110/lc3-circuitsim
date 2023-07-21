#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>

typedef unsigned int guint;
typedef unsigned char guint8;
#include "minecraft-font.c"

static const unsigned chars_per_row = 16;
static const unsigned rows = 6;
static const unsigned char_width = 7;
static const unsigned char_height = 8;
// To make things cleaner, we insert an extra blank column
static const unsigned effective_char_width = 8;
static const unsigned horiz_interchar_padding = 1;
static const unsigned image_width = chars_per_row*char_width + (chars_per_row-1)*horiz_interchar_padding;

static inline void write_zeros(FILE *fp, int count) {
    for (int i = 0; i < count; i++) {
        char sep = (i % chars_per_row == chars_per_row-1)? '\n' : ' ';
        fprintf(fp, "00000000%c", sep);
    }
}

static inline const unsigned char *get_pixel(unsigned char_row, unsigned char_col, unsigned r, unsigned c) {
    unsigned off = (char_row*char_height + r)*image_width + char_col*(char_width + horiz_interchar_padding) + c;
    return gimp_image.pixel_data + (4 * off);
}

int main(void) {
    assert(gimp_image.bytes_per_pixel == 4); // RGBA
    assert(gimp_image.width == image_width);
    assert(gimp_image.height == rows*char_height);

    FILE *fp1, *fp2;
    if (!(fp1 = fopen("minecraft_font1.dat", "w"))
            || !(fp2 = fopen("minecraft_font2.dat", "w"))) {
        perror("fopen");
        if (fp1) {
            fclose(fp1);
        }
        return 1;
    }

    write_zeros(fp1, ' ');
    write_zeros(fp2, ' ');
    
    for (unsigned char_idx = 0; char_idx < chars_per_row*rows; char_idx++) {
        uint64_t pixel_wires = 0;
        for (unsigned row = 0; row < char_height; row++) {
            for (unsigned col = 0; col < char_width; col++) {
                const unsigned char *pixels = get_pixel(char_idx / chars_per_row, char_idx % chars_per_row, row, col);
                assert(!pixels[3] || (pixels[3] == 0xff && !pixels[0] && !pixels[1] && !pixels[2]));
                if (pixels[3]) {
                    unsigned idx = row*effective_char_width + col;
                    pixel_wires |= 1ULL << (char_height*effective_char_width - idx - 1);
                }
            }
        }

        // Roi doesn't allow more than 32 bits in a ROM
        uint32_t pixel_wires1 = pixel_wires >> 32;
        uint32_t pixel_wires2 = pixel_wires & ((1ULL << 32)-1ULL);
        char sep = (char_idx % chars_per_row == chars_per_row-1)? '\n' : ' ';
        fprintf(fp1, "%08" PRIx32 "%c", pixel_wires1, sep);
        fprintf(fp2, "%08" PRIx32 "%c", pixel_wires2, sep);
    }

    write_zeros(fp1, 128);
    write_zeros(fp2, 128);

    fclose(fp1);
    fclose(fp2);
    return 0;
}
