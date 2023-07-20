#include <stdint.h>
#include <stdio.h>
#include "LED-Segment-ASCII/7-Segment/7-Segment-ASCII_HEX.txt"

static void write_zeros(int count) {
    for (int i = 0; i < count; i++) {
        printf("00");
        if (i % 16 == 15) {
            putchar('\n');
        } else {
            putchar(' ');
        }
    }
}

int main(void) {
    write_zeros(' ');

    for (unsigned int i = 0; i < sizeof SevenSegmentASCII / sizeof *SevenSegmentASCII; i++) {
        int bits = SevenSegmentASCII[i];
        int g = (bits >> 6) & 1;
        int f = (bits >> 5) & 1;
        int e = (bits >> 4) & 1;
        int d = (bits >> 3) & 1;
        int c = (bits >> 2) & 1;
        int b = (bits >> 1) & 1;
        int a = bits & 1;

        int roi = (e << 6) | (c << 5) | (f << 4) | (b << 3) | (d << 2) | (g << 1) | a;
        printf("%02x", roi);
        if (i % 16 == 15) {
            putchar('\n');
        } else {
            putchar(' ');
        }
    }

    // Upper ASCI bit is unused
    write_zeros(128);
}
