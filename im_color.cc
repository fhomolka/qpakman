//------------------------------------------------------------------------
//  Color Management
//------------------------------------------------------------------------
//
//  Copyright (c) 2008  Andrew J Apted
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//------------------------------------------------------------------------

#include "headers.h"
#include "main.h"

#include <map>

#include "im_color.h"
#include "im_image.h"


#define MAX_COLOR_CACHE  1024

static std::map<u32_t, byte> color_cache;

static byte palette[256*3];

static bool allow_fullbright  = false;
static byte transparent_color = 0;


const byte quake1_palette[256*3] =
{
    0,  0,  0,  15, 15, 15,  31, 31, 31,  47, 47, 47,  63, 63, 63,
   75, 75, 75,  91, 91, 91, 107,107,107, 123,123,123, 139,139,139,
  155,155,155, 171,171,171, 187,187,187, 203,203,203, 219,219,219,
  235,235,235,  15, 11,  7,  23, 15, 11,  31, 23, 11,  39, 27, 15,
   47, 35, 19,  55, 43, 23,  63, 47, 23,  75, 55, 27,  83, 59, 27,
   91, 67, 31,  99, 75, 31, 107, 83, 31, 115, 87, 31, 123, 95, 35,
  131,103, 35, 143,111, 35,  11, 11, 15,  19, 19, 27,  27, 27, 39,
   39, 39, 51,  47, 47, 63,  55, 55, 75,  63, 63, 87,  71, 71,103,
   79, 79,115,  91, 91,127,  99, 99,139, 107,107,151, 115,115,163,
  123,123,175, 131,131,187, 139,139,203,   0,  0,  0,   7,  7,  0,
   11, 11,  0,  19, 19,  0,  27, 27,  0,  35, 35,  0,  43, 43,  7,
   47, 47,  7,  55, 55,  7,  63, 63,  7,  71, 71,  7,  75, 75, 11,
   83, 83, 11,  91, 91, 11,  99, 99, 11, 107,107, 15,   7,  0,  0,
   15,  0,  0,  23,  0,  0,  31,  0,  0,  39,  0,  0,  47,  0,  0,
   55,  0,  0,  63,  0,  0,  71,  0,  0,  79,  0,  0,  87,  0,  0,
   95,  0,  0, 103,  0,  0, 111,  0,  0, 119,  0,  0, 127,  0,  0,
   19, 19,  0,  27, 27,  0,  35, 35,  0,  47, 43,  0,  55, 47,  0,
   67, 55,  0,  75, 59,  7,  87, 67,  7,  95, 71,  7, 107, 75, 11,
  119, 83, 15, 131, 87, 19, 139, 91, 19, 151, 95, 27, 163, 99, 31,
  175,103, 35,  35, 19,  7,  47, 23, 11,  59, 31, 15,  75, 35, 19,
   87, 43, 23,  99, 47, 31, 115, 55, 35, 127, 59, 43, 143, 67, 51,
  159, 79, 51, 175, 99, 47, 191,119, 47, 207,143, 43, 223,171, 39,
  239,203, 31, 255,243, 27,  11,  7,  0,  27, 19,  0,  43, 35, 15,
   55, 43, 19,  71, 51, 27,  83, 55, 35,  99, 63, 43, 111, 71, 51,
  127, 83, 63, 139, 95, 71, 155,107, 83, 167,123, 95, 183,135,107,
  195,147,123, 211,163,139, 227,179,151, 171,139,163, 159,127,151,
  147,115,135, 139,103,123, 127, 91,111, 119, 83, 99, 107, 75, 87,
   95, 63, 75,  87, 55, 67,  75, 47, 55,  67, 39, 47,  55, 31, 35,
   43, 23, 27,  35, 19, 19,  23, 11, 11,  15,  7,  7, 187,115,159,
  175,107,143, 163, 95,131, 151, 87,119, 139, 79,107, 127, 75, 95,
  115, 67, 83, 107, 59, 75,  95, 51, 63,  83, 43, 55,  71, 35, 43,
   59, 31, 35,  47, 23, 27,  35, 19, 19,  23, 11, 11,  15,  7,  7,
  219,195,187, 203,179,167, 191,163,155, 175,151,139, 163,135,123,
  151,123,111, 135,111, 95, 123, 99, 83, 107, 87, 71,  95, 75, 59,
   83, 63, 51,  67, 51, 39,  55, 43, 31,  39, 31, 23,  27, 19, 15,
   15, 11,  7, 111,131,123, 103,123,111,  95,115,103,  87,107, 95,
   79, 99, 87,  71, 91, 79,  63, 83, 71,  55, 75, 63,  47, 67, 55,
   43, 59, 47,  35, 51, 39,  31, 43, 31,  23, 35, 23,  15, 27, 19,
   11, 19, 11,   7, 11,  7, 255,243, 27, 239,223, 23, 219,203, 19,
  203,183, 15, 187,167, 15, 171,151, 11, 155,131,  7, 139,115,  7,
  123, 99,  7, 107, 83,  0,  91, 71,  0,  75, 55,  0,  59, 43,  0,
   43, 31,  0,  27, 15,  0,  11,  7,  0,   0,  0,255,  11, 11,239,
   19, 19,223,  27, 27,207,  35, 35,191,  43, 43,175,  47, 47,159,
   47, 47,143,  47, 47,127,  47, 47,111,  47, 47, 95,  43, 43, 79,
   35, 35, 63,  27, 27, 47,  19, 19, 31,  11, 11, 15,  43,  0,  0,
   59,  0,  0,  75,  7,  0,  95,  7,  0, 111, 15,  0, 127, 23,  7,
  147, 31,  7, 163, 39, 11, 183, 51, 15, 195, 75, 27, 207, 99, 43,
  219,127, 59, 227,151, 79, 231,171, 95, 239,191,119, 247,211,139,
  167,123, 59, 183,155, 55, 199,195, 55, 231,227, 87, 127,191,255,
  171,231,255, 215,255,255, 103,  0,  0, 139,  0,  0, 179,  0,  0,
  215,  0,  0, 255,  0,  0, 255,243,147, 255,247,199, 255,255,255,
  159, 91, 83
};

const byte quake2_palette[256*3] =
{
    0,  0,  0,  15, 15, 15,  31, 31, 31,  47, 47, 47,  63, 63, 63,
   75, 75, 75,  91, 91, 91, 107,107,107, 123,123,123, 139,139,139,
  155,155,155, 171,171,171, 187,187,187, 203,203,203, 219,219,219,
  235,235,235,  99, 75, 35,  91, 67, 31,  83, 63, 31,  79, 59, 27,
   71, 55, 27,  63, 47, 23,  59, 43, 23,  51, 39, 19,  47, 35, 19,
   43, 31, 19,  39, 27, 15,  35, 23, 15,  27, 19, 11,  23, 15, 11,
   19, 15,  7,  15, 11,  7,  95, 95,111,  91, 91,103,  91, 83, 95,
   87, 79, 91,  83, 75, 83,  79, 71, 75,  71, 63, 67,  63, 59, 59,
   59, 55, 55,  51, 47, 47,  47, 43, 43,  39, 39, 39,  35, 35, 35,
   27, 27, 27,  23, 23, 23,  19, 19, 19, 143,119, 83, 123, 99, 67,
  115, 91, 59, 103, 79, 47, 207,151, 75, 167,123, 59, 139,103, 47,
  111, 83, 39, 235,159, 39, 203,139, 35, 175,119, 31, 147, 99, 27,
  119, 79, 23,  91, 59, 15,  63, 39, 11,  35, 23,  7, 167, 59, 43,
  159, 47, 35, 151, 43, 27, 139, 39, 19, 127, 31, 15, 115, 23, 11,
  103, 23,  7,  87, 19,  0,  75, 15,  0,  67, 15,  0,  59, 15,  0,
   51, 11,  0,  43, 11,  0,  35, 11,  0,  27,  7,  0,  19,  7,  0,
  123, 95, 75, 115, 87, 67, 107, 83, 63, 103, 79, 59,  95, 71, 55,
   87, 67, 51,  83, 63, 47,  75, 55, 43,  67, 51, 39,  63, 47, 35,
   55, 39, 27,  47, 35, 23,  39, 27, 19,  31, 23, 15,  23, 15, 11,
   15, 11,  7, 111, 59, 23,  95, 55, 23,  83, 47, 23,  67, 43, 23,
   55, 35, 19,  39, 27, 15,  27, 19, 11,  15, 11,  7, 179, 91, 79,
  191,123,111, 203,155,147, 215,187,183, 203,215,223, 179,199,211,
  159,183,195, 135,167,183, 115,151,167,  91,135,155,  71,119,139,
   47,103,127,  23, 83,111,  19, 75,103,  15, 67, 91,  11, 63, 83,
    7, 55, 75,   7, 47, 63,   7, 39, 51,   0, 31, 43,   0, 23, 31,
    0, 15, 19,   0,  7, 11,   0,  0,  0, 139, 87, 87, 131, 79, 79,
  123, 71, 71, 115, 67, 67, 107, 59, 59,  99, 51, 51,  91, 47, 47,
   87, 43, 43,  75, 35, 35,  63, 31, 31,  51, 27, 27,  43, 19, 19,
   31, 15, 15,  19, 11, 11,  11,  7,  7,   0,  0,  0, 151,159,123,
  143,151,115, 135,139,107, 127,131, 99, 119,123, 95, 115,115, 87,
  107,107, 79,  99, 99, 71,  91, 91, 67,  79, 79, 59,  67, 67, 51,
   55, 55, 43,  47, 47, 35,  35, 35, 27,  23, 23, 19,  15, 15, 11,
  159, 75, 63, 147, 67, 55, 139, 59, 47, 127, 55, 39, 119, 47, 35,
  107, 43, 27,  99, 35, 23,  87, 31, 19,  79, 27, 15,  67, 23, 11,
   55, 19, 11,  43, 15,  7,  31, 11,  7,  23,  7,  0,  11,  0,  0,
    0,  0,  0, 119,123,207, 111,115,195, 103,107,183,  99, 99,167,
   91, 91,155,  83, 87,143,  75, 79,127,  71, 71,115,  63, 63,103,
   55, 55, 87,  47, 47, 75,  39, 39, 63,  35, 31, 47,  27, 23, 35,
   19, 15, 23,  11,  7,  7, 155,171,123, 143,159,111, 135,151, 99,
  123,139, 87, 115,131, 75, 103,119, 67,  95,111, 59,  87,103, 51,
   75, 91, 39,  63, 79, 27,  55, 67, 19,  47, 59, 11,  35, 47,  7,
   27, 35,  0,  19, 23,  0,  11, 15,  0,   0,255,  0,  35,231, 15,
   63,211, 27,  83,187, 39,  95,167, 47,  95,143, 51,  95,123, 51,
  255,255,255, 255,255,211, 255,255,167, 255,255,127, 255,255, 83,
  255,255, 39, 255,235, 31, 255,215, 23, 255,191, 15, 255,171,  7,
  255,147,  0, 239,127,  0, 227,107,  0, 211, 87,  0, 199, 71,  0,
  183, 59,  0, 171, 43,  0, 155, 31,  0, 143, 23,  0, 127, 15,  0,
  115,  7,  0,  95,  0,  0,  71,  0,  0,  47,  0,  0,  27,  0,  0,
  239,  0,  0,  55, 55,255, 255,  0,  0,   0,  0,255,  43, 43, 35,
   27, 27, 23,  19, 19, 15, 235,151,127, 195,115, 83, 159, 87, 51,
  123, 63, 27, 235,211,199, 199,171,155, 167,139,119, 135,107, 87,
  159, 91, 83
};

const byte hexen2_palette[256*3] =
{
    0,  0,  0,   0,  0,  0,   8,  8,  8,  16, 16, 16,  24, 24, 24,
   32, 32, 32,  40, 40, 40,  48, 48, 48,  56, 56, 56,  64, 64, 64,
   72, 72, 72,  80, 80, 80,  84, 84, 84,  88, 88, 88,  96, 96, 96,
  104,104,104, 112,112,112, 120,120,120, 128,128,128, 136,136,136,
  148,148,148, 156,156,156, 168,168,168, 180,180,180, 184,184,184,
  196,196,196, 204,204,204, 212,212,212, 224,224,224, 232,232,232,
  240,240,240, 252,252,252,   8,  8, 12,  16, 16, 20,  24, 24, 28,
   28, 32, 36,  36, 36, 44,  44, 44, 52,  48, 52, 60,  56, 56, 68,
   64, 64, 72,  76, 76, 88,  92, 92,104, 108,112,128, 128,132,152,
  152,156,176, 168,172,196, 188,196,220,  32, 24, 20,  40, 32, 28,
   48, 36, 32,  52, 44, 40,  60, 52, 44,  68, 56, 52,  76, 64, 56,
   84, 72, 64,  92, 76, 72, 100, 84, 76, 108, 92, 84, 112, 96, 88,
  120,104, 96, 128,112,100, 136,116,108, 144,124,112,  20, 24, 20,
   28, 32, 28,  32, 36, 32,  40, 44, 40,  44, 48, 44,  48, 56, 48,
   56, 64, 56,  64, 68, 64,  68, 76, 68,  84, 92, 84, 104,112,104,
  120,128,120, 140,148,136, 156,164,152, 172,180,168, 188,196,184,
   48, 32,  8,  60, 40,  8,  72, 48, 16,  84, 56, 20,  92, 64, 28,
  100, 72, 36, 108, 80, 44, 120, 92, 52, 136,104, 60, 148,116, 72,
  160,128, 84, 168,136, 92, 180,144,100, 188,152,108, 196,160,116,
  204,168,124,  16, 20, 16,  20, 28, 20,  24, 32, 24,  28, 36, 28,
   32, 44, 32,  36, 48, 36,  40, 56, 40,  44, 60, 44,  48, 68, 48,
   52, 76, 52,  60, 84, 60,  68, 92, 64,  76,100, 72,  84,108, 76,
   92,116, 84, 100,128, 92,  24, 12,  8,  32, 16,  8,  40, 20,  8,
   52, 24, 12,  60, 28, 12,  68, 32, 12,  76, 36, 16,  84, 44, 20,
   92, 48, 24, 100, 56, 28, 112, 64, 32, 120, 72, 36, 128, 80, 44,
  144, 92, 56, 168,112, 72, 192,132, 88,  24,  4,  4,  36,  4,  4,
   48,  0,  0,  60,  0,  0,  68,  0,  0,  80,  0,  0,  88,  0,  0,
  100,  0,  0, 112,  0,  0, 132,  0,  0, 152,  0,  0, 172,  0,  0,
  192,  0,  0, 212,  0,  0, 232,  0,  0, 252,  0,  0,  16, 12, 32,
   28, 20, 48,  32, 28, 56,  40, 36, 68,  52, 44, 80,  60, 56, 92,
   68, 64,104,  80, 72,116,  88, 84,128, 100, 96,140, 108,108,152,
  120,116,164, 132,132,176, 144,144,188, 156,156,200, 172,172,212,
   36, 20,  4,  52, 24,  4,  68, 32,  4,  80, 40,  0, 100, 48,  4,
  124, 60,  4, 140, 72,  4, 156, 88,  8, 172,100,  8, 188,116, 12,
  204,128, 12, 220,144, 16, 236,160, 20, 252,184, 56, 248,200, 80,
  248,220,120,  20, 16,  4,  28, 24,  8,  36, 32,  8,  44, 40, 12,
   52, 48, 16,  56, 56, 16,  64, 64, 20,  68, 72, 24,  72, 80, 28,
   80, 92, 32,  84,104, 40,  88,116, 44,  92,128, 52,  92,140, 52,
   92,148, 56,  96,160, 64,  60, 16, 16,  72, 24, 24,  84, 28, 28,
  100, 36, 36, 112, 44, 44, 124, 52, 48, 140, 64, 56, 152, 76, 64,
   44, 20,  8,  56, 28, 12,  72, 32, 16,  84, 40, 20,  96, 44, 28,
  112, 52, 32, 124, 56, 40, 140, 64, 48,  24, 20, 16,  36, 28, 20,
   44, 36, 28,  56, 44, 32,  64, 52, 36,  72, 60, 44,  80, 68, 48,
   92, 76, 52, 100, 84, 60, 112, 92, 68, 120,100, 72, 132,112, 80,
  144,120, 88, 152,128, 96, 160,136,104, 168,148,112,  36, 24, 12,
   44, 32, 16,  52, 40, 20,  60, 44, 20,  72, 52, 24,  80, 60, 28,
   88, 68, 28, 104, 76, 32, 148, 96, 56, 160,108, 64, 172,116, 72,
  180,124, 80, 192,132, 88, 204,140, 92, 216,156,108,  60, 20, 92,
  100, 36,116, 168, 72,164, 204,108,192,   4, 84,  4,   4,132,  4,
    0,180,  0,   0,216,  0,   4,  4,144,  16, 68,204,  36,132,224,
   88,168,232, 216,  4,  4, 244, 72,  0, 252,128,  0, 252,172, 24,
  252,252,252
};

const byte haktoria_palette[256*3] =
{
    0,  0,  0,   8,  8,  8,  16, 16, 16,  25, 25, 25,  33, 33, 33,
   41, 41, 41,  49, 49, 49,  57, 57, 57,  66, 66, 66,  74, 74, 74,
   82, 82, 82,  90, 90, 90,  98, 98, 98, 107,107,107, 115,115,115,
  123,123,123, 131,131,131, 139,139,139, 147,147,147, 156,156,156,
  164,164,164, 172,172,172, 180,180,180, 188,188,188, 197,197,197,
  205,205,205, 213,213,213, 221,221,221, 229,229,229, 238,238,238,
  246,246,246, 254,254,254,   8,  8, 12,  16, 16, 20,  20, 16, 36,
   30, 26, 46,  40, 36, 57,  49, 47, 68,  59, 57, 78,  72, 70, 92,
   86, 84,106,  99, 97,120, 112,112,135, 125,127,150, 138,141,164,
  151,156,179, 174,184,208, 208,208,216,  12, 13,  5,  25, 26, 15,
   38, 39, 26,  44, 45, 32,  50, 52, 37,  56, 58, 43,  67, 69, 52,
   76, 78, 59,  84, 86, 65,  93, 95, 72, 104,105, 80, 115,115, 88,
  125,126, 97, 136,136,105, 145,144,111, 154,151,118,  20, 24, 20,
   35, 38, 35,  49, 52, 49,  64, 66, 64,  78, 81, 77,  92, 96, 90,
  106,112,104, 116,121,112, 125,131,121, 134,141,130, 144,151,139,
  153,161,148, 162,170,157, 171,179,166, 180,187,176, 189,196,185,
   48, 32,  8,  60, 40,  8,  72, 48, 16,  84, 56, 20,  92, 64, 28,
  100, 72, 36, 108, 80, 44, 120, 92, 52, 136,104, 60, 148,116, 72,
  160,128, 84, 168,136, 92, 180,144,100, 188,152,108, 196,160,116,
  204,168,124,  16, 20, 16,  20, 28, 20,  24, 32, 24,  28, 36, 28,
   32, 44, 32,  36, 48, 36,  40, 56, 40,  44, 60, 44,  48, 68, 48,
   52, 76, 52,  60, 84, 60,  68, 92, 64,  76,100, 72,  84,108, 76,
   92,116, 84, 100,128, 92,  27, 15,  8,  34, 18, 10,  40, 21, 11,
   47, 24, 13,  54, 28, 14,  61, 31, 16,  67, 34, 17,  74, 37, 19,
   82, 43, 22,  89, 49, 26,  97, 55, 29, 112, 67, 36, 128, 79, 44,
  143, 91, 51, 167,113, 71, 192,135, 91,  24,  4,  4,  46,  4,  4,
   68,  3,  3,  89,  3,  3, 111,  3,  3, 133,  2,  2, 155,  2,  2,
  177,  2,  2, 198,  2,  2, 220,  1,  1, 242,  1,  1, 245, 44, 44,
  247, 87, 87, 250,129,129, 252,172,172, 255,215,215,   2,  4, 41,
    2,  4, 51,   1,  4, 62,   1,  4, 73,   1,  4, 83,   0,  4, 93,
    0,  4,104,   4,  8,131,   8, 12,157,  12, 16,184,  35, 37,207,
   57, 59,230,  80, 80,253,  80,108,253,  80,136,252,  80,164,252,
   36, 20,  4,  52, 24,  4,  68, 32,  4,  80, 40,  0,  97, 47,  2,
  115, 61,  6, 132, 76,  9, 150, 90, 13, 167,105, 17, 185,119, 21,
  202,134, 24, 220,148, 28, 230,168, 51, 241,188, 74, 251,208, 97,
  251,221,123,  11, 23,  7,  17, 35, 12,  23, 48, 18,  29, 60, 23,
   35, 72, 28,  41, 85, 34,  47, 97, 39,  54,112, 46,  62,127, 53,
   69,142, 59,  77,157, 66,  84,172, 73,  94,192, 81, 102,213, 91,
  111,234,101, 119,255,111,  60, 16, 16,  72, 24, 24,  84, 28, 28,
  100, 36, 36, 112, 44, 44, 124, 52, 48, 140, 64, 56, 152, 76, 64,
   44, 20,  8,  56, 28, 12,  72, 32, 16,  84, 40, 20,  96, 44, 28,
  112, 52, 32, 124, 56, 40, 140, 64, 48,  24, 20, 16,  36, 28, 20,
   44, 36, 28,  56, 44, 32,  64, 52, 36,  72, 60, 44,  80, 68, 48,
   92, 76, 52, 100, 84, 60, 112, 92, 68, 120,100, 72, 132,112, 80,
  144,120, 88, 152,128, 96, 160,136,104, 168,148,112,  43, 13, 64,
   71, 24, 97, 100, 36,129, 128, 47,162,   2, 14, 20,   4, 30, 39,
    8, 47, 71,  20, 69, 91, 151, 99, 59, 169,114, 70, 186,128, 82,
  204,143, 93, 212,156,112, 219,170,130, 227,183,149, 140, 52,156,
  168, 63,194, 197, 74,232, 254,169,253,   4, 84,  4,   4,159,  4,
    3,234,  3, 121,255,121,  80,196,252,  80,216,252,  80,236,252,
  188,252,244, 244, 14,  3, 248, 87,  2, 251,160,  1, 255,233,  0,
  159, 91, 83,
};


void COL_WritePalette(const byte *source)
{
  memcpy(palette, source, sizeof(palette));

  color_cache.clear();
}


void COL_SetPalette(game_kind_e type)
{
  switch (type)
  {
    case GAME_Quake1:
      COL_WritePalette(quake1_palette);
      break;

    case GAME_Quake2:
      COL_WritePalette(quake2_palette);
      break;

    case GAME_Hexen2:
      COL_WritePalette(hexen2_palette);
      break;

    case GAME_Haktoria:
      COL_WritePalette(haktoria_palette);
      break;

    default:
      FatalError("COL_SetPalette: unknown game (%d)\n", (int)type);
  }
}


void COL_SetTransparent(byte pix)
{
  if (transparent_color != pix)
  {
    transparent_color = pix;

    color_cache.clear();
  }
}


void COL_SetFullBright(bool enable)
{
  if (enable != allow_fullbright)
  {
    allow_fullbright = enable;

    color_cache.clear();
  }
}


u32_t COL_ReadPalette(byte pix)
{
  byte R = palette[pix*3 + 2];
  byte G = palette[pix*3 + 1];
  byte B = palette[pix*3 + 0];

  return MAKE_RGB(R, G, B);
}


u32_t COL_ReadPalWithTrans(byte pix)
{
  if (pix == transparent_color)
    return MAKE_RGBA(0, 0, 0, ALPHA_TRANS);

  byte R = palette[pix*3 + 2];
  byte G = palette[pix*3 + 1];
  byte B = palette[pix*3 + 0];

  return MAKE_RGB(R, G, B);
}


byte COL_FindColor(const byte *palette, u32_t rgb_col)
{
  int best_idx  = -1;
  int best_dist = (1<<30);


  // Note: we skip index #0 (black), which is used for transparency
  //       in skies.  Black is duplicated at index #48 though.
  int min_col = (game_type == GAME_Quake1) ? 1 : 0;
  int max_col = allow_fullbright ? 255 : 255-32;

  for (int i = min_col; i <= max_col; i++)
  {
    int dr = RGB_R(rgb_col) - palette[i*3+2];
    int dg = RGB_G(rgb_col) - palette[i*3+1];
    int db = RGB_B(rgb_col) - palette[i*3+0];

    int dist = dr*dr + dg*dg + db*db;

    // exact match?
    if (dist == 0)
      return i;

    if (dist < best_dist)
    {
      best_idx  = i;
      best_dist = dist;
    }
  }

  SYS_ASSERT(best_idx >= 0);

  return best_idx;
}


byte COL_MapColor(u32_t rgb_col)
{
  if (RGB_A(rgb_col) <= 128)
    return transparent_color;

  // ignore alpha from now on
  rgb_col &= 0xFFFFFF;

  if (color_cache.find(rgb_col) != color_cache.end())
    return color_cache[rgb_col];

  // don't let the color cache grow without limit
  if ((int)color_cache.size() >= MAX_COLOR_CACHE)
  {
    color_cache.clear();
  }

  byte pal_idx = COL_FindColor(palette, rgb_col);

  color_cache[rgb_col] = pal_idx;

  return pal_idx;
}


void COL_LoadPaletteFromFile(const char *filename)
{
  int length;

  u8_t *data = FileLoad(filename, &length);

  if (! data)
    FatalError("Could not open colors file: %s\n", filename);

  // see whether file is text or binary
  bool is_binary = false;

  for (int i = 0; i < length; i++)
  {
    if (data[i] & 0x80 || data[i] < 7)
    {
      is_binary = true;
      break;
    }
  }

  printf("Loading colors%s from: %s\n",
         is_binary ? "" : " (TEXT)", filename);

  if (is_binary)
  {
    if (length < (int)sizeof(palette))
      FatalError("Colors file too short!\n");

    memcpy(palette, data, sizeof(palette));
  }
  else
  {
    // modify the text : nothing but digits and spaces
    for (int i = 0; i < length; i++)
    {
      if (! isalnum(data[i]))
        data[i] = ' ';
    }

    // parse R,G,B triplets
    // NOTE that FileLoad() ensures a NUL-terminated buffer

    const char *buf = (char *)data;

    for (int pix = 0; pix < 256; pix++)
    {
      while (*buf == ' ') buf++;

      if (*buf == 0)
        FatalError("Not enough colors (failed at #%d)\n", pix);

      int R=0, G=0, B=0;
      int num_chars = 0;

      int got = sscanf(buf, "%d %d %d%n", &R, &G, &B, &num_chars);

      if (got == EOF || got == 0)
        FatalError("Not enough colors (failed at #%d)\n", pix);

      if (R < 0 || R > 255 ||
          G < 0 || G > 255 ||
          B < 0 || B > 255)
      {
        FatalError("Bad color at #%d : (%d %d %d)\n", pix, R, G, B);
      }

      palette[pix*3+0] = R;
      palette[pix*3+1] = G;
      palette[pix*3+2] = B;

      buf += num_chars;
    }
  }

  FileFree(data);
}


//--- editor settings ---
// vi:ts=2:sw=2:expandtab
