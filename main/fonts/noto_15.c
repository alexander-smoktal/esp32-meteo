/*******************************************************************************
 * Size: 15 px
 * Bpp: 1
 * Opts: --bpp 1 --size 15 --no-compress --font NotoSans-VariableFont_wdth,wght.ttf --range 32-128,176,700 --format lvgl -o noto_15.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef NOTO_15
#define NOTO_15 1
#endif

#if NOTO_15

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xaa, 0xa8, 0xf0,

    /* U+0022 "\"" */
    0x99, 0x99,

    /* U+0023 "#" */
    0x9, 0xc, 0x84, 0x8f, 0xf1, 0x20, 0x93, 0xfe,
    0x48, 0x24, 0x12, 0x0,

    /* U+0024 "$" */
    0x10, 0xfe, 0x44, 0x8f, 0x7, 0x85, 0x89, 0x13,
    0xf8, 0x40, 0x80,

    /* U+0025 "%" */
    0x60, 0x92, 0x22, 0x44, 0x49, 0x9, 0x4c, 0xca,
    0x42, 0x48, 0x89, 0x11, 0x24, 0x18,

    /* U+0026 "&" */
    0x38, 0x11, 0x4, 0x41, 0xb0, 0x30, 0x16, 0x28,
    0xda, 0x1c, 0xc7, 0x1f, 0x60,

    /* U+0027 "'" */
    0xf0,

    /* U+0028 "(" */
    0x29, 0x49, 0x24, 0x91, 0x22,

    /* U+0029 ")" */
    0x89, 0x12, 0x49, 0x25, 0x28,

    /* U+002A "*" */
    0x30, 0x23, 0x53, 0xc3, 0x89, 0x2, 0x0,

    /* U+002B "+" */
    0x10, 0x20, 0x47, 0xf1, 0x2, 0x4, 0x0,

    /* U+002C "," */
    0xe8,

    /* U+002D "-" */
    0xf0,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x8, 0x44, 0x23, 0x10, 0x88, 0x46, 0x0,

    /* U+0030 "0" */
    0x38, 0x8a, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x44,
    0x70,

    /* U+0031 "1" */
    0x37, 0x51, 0x11, 0x11, 0x11,

    /* U+0032 "2" */
    0x79, 0x8, 0x10, 0x20, 0xc1, 0x4, 0x10, 0x41,
    0xfc,

    /* U+0033 "3" */
    0x7c, 0x84, 0x8, 0x33, 0x80, 0xc0, 0x81, 0x86,
    0xf8,

    /* U+0034 "4" */
    0x4, 0xc, 0x14, 0x34, 0x24, 0x44, 0xc4, 0xff,
    0x4, 0x4,

    /* U+0035 "5" */
    0x7c, 0x81, 0x3, 0xe0, 0x60, 0x40, 0x81, 0x84,
    0xf0,

    /* U+0036 "6" */
    0x3c, 0xc2, 0x5, 0xec, 0x70, 0x60, 0xc1, 0x44,
    0x70,

    /* U+0037 "7" */
    0xfe, 0xc, 0x10, 0x60, 0x83, 0x4, 0x8, 0x20,
    0x40,

    /* U+0038 "8" */
    0x7d, 0x8e, 0xe, 0x37, 0x8d, 0xa0, 0xc1, 0xc6,
    0xf8,

    /* U+0039 "9" */
    0x79, 0x8a, 0xc, 0x1c, 0x6f, 0x40, 0x82, 0xc,
    0xf0,

    /* U+003A ":" */
    0xf0, 0xf,

    /* U+003B ";" */
    0x6c, 0x0, 0x3, 0x48,

    /* U+003C "<" */
    0x2, 0x18, 0xc6, 0x6, 0x3, 0x1, 0x80,

    /* U+003D "=" */
    0xfe, 0x0, 0x7, 0xf0,

    /* U+003E ">" */
    0x80, 0xc0, 0x60, 0x30, 0xc6, 0x30, 0x0,

    /* U+003F "?" */
    0x78, 0x10, 0x41, 0x18, 0x43, 0x0, 0x30, 0xc0,

    /* U+0040 "@" */
    0xf, 0x83, 0x6, 0x4f, 0x2d, 0x91, 0x91, 0x19,
    0x11, 0x91, 0x38, 0xee, 0x40, 0x6, 0x8, 0x1f,
    0x0,

    /* U+0041 "A" */
    0xc, 0x3, 0x1, 0x40, 0x48, 0x32, 0x8, 0xc7,
    0xf1, 0x6, 0x40, 0xb0, 0x20,

    /* U+0042 "B" */
    0xfd, 0xe, 0xc, 0x1f, 0xd0, 0xe0, 0xc1, 0x87,
    0xf8,

    /* U+0043 "C" */
    0x1f, 0x60, 0x40, 0x80, 0x80, 0x80, 0x80, 0xc0,
    0x60, 0x3f,

    /* U+0044 "D" */
    0xfc, 0x86, 0x83, 0x81, 0x81, 0x81, 0x81, 0x82,
    0x86, 0xf8,

    /* U+0045 "E" */
    0xfe, 0x8, 0x20, 0xfa, 0x8, 0x20, 0x83, 0xf0,

    /* U+0046 "F" */
    0xfe, 0x8, 0x20, 0xfa, 0x8, 0x20, 0x82, 0x0,

    /* U+0047 "G" */
    0x1f, 0xb0, 0x10, 0x10, 0x8, 0x7c, 0x6, 0x3,
    0x81, 0x60, 0x8f, 0xc0,

    /* U+0048 "H" */
    0x81, 0x81, 0x81, 0x81, 0xff, 0x81, 0x81, 0x81,
    0x81, 0x81,

    /* U+0049 "I" */
    0xe9, 0x24, 0x92, 0x5c,

    /* U+004A "J" */
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0xe0,

    /* U+004B "K" */
    0x87, 0x1a, 0x65, 0x8e, 0x1e, 0x26, 0x46, 0x85,
    0xc,

    /* U+004C "L" */
    0x82, 0x8, 0x20, 0x82, 0x8, 0x20, 0x83, 0xf0,

    /* U+004D "M" */
    0xc0, 0x7c, 0xe, 0x82, 0xd0, 0x59, 0x1b, 0x22,
    0x66, 0xcc, 0x51, 0x8e, 0x30, 0x84,

    /* U+004E "N" */
    0xc1, 0xc1, 0xa1, 0xb1, 0x91, 0x89, 0x8d, 0x85,
    0x83, 0x83,

    /* U+004F "O" */
    0x1f, 0x18, 0x6c, 0xa, 0x1, 0x80, 0x60, 0x18,
    0x7, 0x2, 0x61, 0x8f, 0x80,

    /* U+0050 "P" */
    0xfd, 0xe, 0xc, 0x18, 0x7f, 0x20, 0x40, 0x81,
    0x0,

    /* U+0051 "Q" */
    0x1f, 0x18, 0x6c, 0xa, 0x1, 0x80, 0x60, 0x18,
    0x7, 0x3, 0x61, 0x87, 0xc0, 0x30, 0x6, 0x0,
    0xc0,

    /* U+0052 "R" */
    0xfc, 0x86, 0x82, 0x82, 0x86, 0xf8, 0x8c, 0x84,
    0x86, 0x82,

    /* U+0053 "S" */
    0x7f, 0x6, 0x6, 0x7, 0x3, 0x80, 0x81, 0x7,
    0xf0,

    /* U+0054 "T" */
    0xff, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8,
    0x8, 0x8,

    /* U+0055 "U" */
    0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x42, 0x3c,

    /* U+0056 "V" */
    0xc1, 0xa0, 0x90, 0x4c, 0x62, 0x21, 0x30, 0xd8,
    0x28, 0x14, 0x4, 0x0,

    /* U+0057 "W" */
    0xc3, 0xd, 0xc, 0x24, 0x70, 0x99, 0x26, 0x64,
    0x90, 0xb2, 0x42, 0x8d, 0xe, 0x1c, 0x18, 0x60,
    0x61, 0x80,

    /* U+0058 "X" */
    0x41, 0x31, 0x9, 0x82, 0x81, 0x80, 0xe0, 0xd0,
    0x44, 0x43, 0x60, 0x80,

    /* U+0059 "Y" */
    0x41, 0x31, 0x88, 0x86, 0xc1, 0x40, 0x40, 0x20,
    0x10, 0x8, 0x4, 0x0,

    /* U+005A "Z" */
    0xfe, 0xc, 0x30, 0x41, 0x86, 0x8, 0x30, 0xc1,
    0xfc,

    /* U+005B "[" */
    0xf2, 0x49, 0x24, 0x92, 0x4e,

    /* U+005C "\\" */
    0xc2, 0x10, 0x42, 0x18, 0x42, 0x8, 0x40,

    /* U+005D "]" */
    0xe4, 0x92, 0x49, 0x24, 0x9e,

    /* U+005E "^" */
    0x10, 0x60, 0xe2, 0x44, 0x50, 0xa0, 0x80,

    /* U+005F "_" */
    0xfe,

    /* U+0060 "`" */
    0x99, 0x0,

    /* U+0061 "a" */
    0x78, 0x10, 0x5f, 0xc6, 0x18, 0xdd,

    /* U+0062 "b" */
    0x81, 0x2, 0x5, 0xcc, 0x50, 0x60, 0xc1, 0x83,
    0x8a, 0xe0,

    /* U+0063 "c" */
    0x3d, 0x8, 0x20, 0x82, 0x4, 0xf,

    /* U+0064 "d" */
    0x2, 0x4, 0x9, 0xd4, 0x70, 0x60, 0xc1, 0x82,
    0x8c, 0xe8,

    /* U+0065 "e" */
    0x3c, 0x8e, 0xf, 0xf8, 0x10, 0x10, 0x9f,

    /* U+0066 "f" */
    0x3a, 0x11, 0xf4, 0x21, 0x8, 0x42, 0x10,

    /* U+0067 "g" */
    0x3a, 0x8e, 0xc, 0x18, 0x30, 0x51, 0xbd, 0x2,
    0x6, 0x1f, 0xe0,

    /* U+0068 "h" */
    0x81, 0x2, 0x5, 0xec, 0x70, 0x60, 0xc1, 0x83,
    0x6, 0x8,

    /* U+0069 "i" */
    0x9f, 0xe0,

    /* U+006A "j" */
    0x20, 0x12, 0x49, 0x24, 0x92, 0x70,

    /* U+006B "k" */
    0x82, 0x8, 0x23, 0x9a, 0xce, 0x3c, 0x92, 0x28,
    0xc0,

    /* U+006C "l" */
    0xff, 0xe0,

    /* U+006D "m" */
    0xb9, 0xd8, 0xc6, 0x10, 0xc2, 0x18, 0x43, 0x8,
    0x61, 0xc, 0x21,

    /* U+006E "n" */
    0xbd, 0x8e, 0xc, 0x18, 0x30, 0x60, 0xc1,

    /* U+006F "o" */
    0x38, 0x8a, 0xc, 0x18, 0x30, 0x51, 0x1c,

    /* U+0070 "p" */
    0xb9, 0x8a, 0xc, 0x18, 0x30, 0x71, 0x5c, 0x81,
    0x2, 0x4, 0x0,

    /* U+0071 "q" */
    0x3a, 0x8e, 0xc, 0x18, 0x30, 0x51, 0x9d, 0x2,
    0x4, 0x8, 0x10,

    /* U+0072 "r" */
    0xbc, 0x88, 0x88, 0x88,

    /* U+0073 "s" */
    0x7e, 0x18, 0x1c, 0x18, 0x10, 0x7e,

    /* U+0074 "t" */
    0x42, 0x3e, 0x84, 0x21, 0x8, 0x41, 0xc0,

    /* U+0075 "u" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x71, 0xbd,

    /* U+0076 "v" */
    0xc2, 0x42, 0x46, 0x64, 0x24, 0x2c, 0x38, 0x18,

    /* U+0077 "w" */
    0xc6, 0x24, 0x62, 0x4e, 0x24, 0xb6, 0x69, 0x43,
    0x94, 0x31, 0xc3, 0xc,

    /* U+0078 "x" */
    0x42, 0x64, 0x3c, 0x18, 0x18, 0x2c, 0x66, 0x42,

    /* U+0079 "y" */
    0x83, 0x85, 0x1b, 0x22, 0xc5, 0xe, 0xc, 0x10,
    0x20, 0xc7, 0x0,

    /* U+007A "z" */
    0xfc, 0x31, 0x84, 0x21, 0x8c, 0x3f,

    /* U+007B "{" */
    0x19, 0x8, 0x42, 0x13, 0x4, 0x21, 0x8, 0x41,
    0x80,

    /* U+007C "|" */
    0xff, 0xfe,

    /* U+007D "}" */
    0xc1, 0x8, 0x42, 0x10, 0x64, 0x21, 0x8, 0x4c,
    0x0,

    /* U+007E "~" */
    0xf3, 0x3c,

    /* U+00B0 "°" */
    0x74, 0x63, 0x17, 0x0,

    /* U+02BC "ʼ" */
    0x58};

/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 62, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 65, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 98, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 6, .adv_w = 155, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 18, .adv_w = 137, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 29, .adv_w = 199, .box_w = 11, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 43, .adv_w = 176, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 56, .adv_w = 54, .box_w = 1, .box_h = 4, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 57, .adv_w = 72, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 62, .adv_w = 72, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 67, .adv_w = 132, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 74, .adv_w = 137, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 81, .adv_w = 64, .box_w = 2, .box_h = 3, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 82, .adv_w = 77, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 83, .adv_w = 64, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 84, .adv_w = 89, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 91, .adv_w = 137, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 137, .box_w = 4, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 105, .adv_w = 137, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 114, .adv_w = 137, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 123, .adv_w = 137, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 133, .adv_w = 137, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 142, .adv_w = 137, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 151, .adv_w = 137, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 160, .adv_w = 137, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 169, .adv_w = 137, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 178, .adv_w = 64, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 180, .adv_w = 64, .box_w = 3, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 184, .adv_w = 137, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 191, .adv_w = 137, .box_w = 7, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 195, .adv_w = 137, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 202, .adv_w = 104, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 210, .adv_w = 216, .box_w = 12, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 227, .adv_w = 153, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 240, .adv_w = 156, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 249, .adv_w = 152, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 259, .adv_w = 175, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 269, .adv_w = 133, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 277, .adv_w = 125, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 285, .adv_w = 175, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 297, .adv_w = 178, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 307, .adv_w = 81, .box_w = 3, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 311, .adv_w = 66, .box_w = 4, .box_h = 13, .ofs_x = -2, .ofs_y = -3},
    {.bitmap_index = 318, .adv_w = 149, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 327, .adv_w = 126, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 335, .adv_w = 218, .box_w = 11, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 349, .adv_w = 182, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 359, .adv_w = 187, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 372, .adv_w = 145, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 381, .adv_w = 187, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 398, .adv_w = 149, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 408, .adv_w = 132, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 417, .adv_w = 133, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 427, .adv_w = 175, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 437, .adv_w = 144, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 449, .adv_w = 223, .box_w = 14, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 467, .adv_w = 141, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 479, .adv_w = 136, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 491, .adv_w = 137, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 500, .adv_w = 79, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 505, .adv_w = 89, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 512, .adv_w = 79, .box_w = 3, .box_h = 13, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 517, .adv_w = 137, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 524, .adv_w = 107, .box_w = 7, .box_h = 1, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 525, .adv_w = 67, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 9},
    {.bitmap_index = 527, .adv_w = 135, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 533, .adv_w = 148, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 543, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 549, .adv_w = 148, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 559, .adv_w = 135, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 566, .adv_w = 83, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 573, .adv_w = 148, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 584, .adv_w = 148, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 594, .adv_w = 62, .box_w = 1, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 596, .adv_w = 62, .box_w = 3, .box_h = 15, .ofs_x = -1, .ofs_y = -4},
    {.bitmap_index = 602, .adv_w = 128, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 611, .adv_w = 62, .box_w = 1, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 613, .adv_w = 224, .box_w = 11, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 624, .adv_w = 148, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 631, .adv_w = 145, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 638, .adv_w = 148, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 649, .adv_w = 148, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 660, .adv_w = 99, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 664, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 670, .adv_w = 87, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 677, .adv_w = 148, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 684, .adv_w = 122, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 692, .adv_w = 189, .box_w = 12, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 704, .adv_w = 127, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 712, .adv_w = 122, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 723, .adv_w = 113, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 729, .adv_w = 91, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 738, .adv_w = 132, .box_w = 1, .box_h = 15, .ofs_x = 4, .ofs_y = -4},
    {.bitmap_index = 740, .adv_w = 91, .box_w = 5, .box_h = 13, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 749, .adv_w = 137, .box_w = 7, .box_h = 2, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 751, .adv_w = 103, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 755, .adv_w = 42, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 7}};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0x20c};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
    {
        {.range_start = 32, .range_length = 95, .glyph_id_start = 1, .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY},
        {.range_start = 176, .range_length = 525, .glyph_id_start = 96, .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 2, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY}};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};

/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t noto_15 = {
#else
lv_font_t noto_15 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt, /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt, /*Function pointer to get glyph's bitmap*/
    .line_height = 16,                              /*The maximum line height required by the font*/
    .base_line = 4,                                 /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc, /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};

#endif /*#if NOTO_15*/
