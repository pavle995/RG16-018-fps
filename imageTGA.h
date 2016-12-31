#ifndef IMAGETGA_H
#define IMAGETGA_H

#include <stdlib.h>
#include <stdio.h>

#include "project.h"
#include "util.h"

/* TODO: return to the old standard */
typedef struct Header {
    unsigned char  IDLength;
    unsigned char  colorMapType;
    unsigned char  imageType;
    /* Color map specification */
    unsigned char  firstEntryIndex  [2];
    unsigned char  colorMapLength   [2];
    unsigned char  colorMapEntrySize;
    /* Image specification */
    unsigned char  xOrigin          [2];
    unsigned char  yOrigin          [2];
    unsigned char  imageWidth       [2];
    unsigned char  imageHeight      [2];
    unsigned char  pixelDepth;
    unsigned char  imageDescriptor;
} Header;

/* TGA image types and compressions */
typedef enum {
    TypeNoImageData    = 0,
    TypePaletteIndexed = 1,
    TypeRGB            = 2,
    TypeGrayscale      = 3,
    TypeCompressedPaletteIndexed = 9,
    TypeCompressedRGB            = 10,
    TypeCompressedGrayscale      = 11
} ImageType;

/* Pixel ordering, 4th bit is left-right, 5th bit is bottom-top */
typedef enum {
    DescriptorBottomLeft  = 0b00000000,     /* From bottom left to top right */
    DescriptorBottomRight = 0b00010000,     /* From bottom right to top left */
    DescriptorTopLeft     = 0b00100000,     /* From top left to bottom right */
    DescriptorTopRight    = 0b00110000      /* From top right to bottom left */
} ImageDescriptor;

typedef struct InternalImage {
    unsigned char  colorDepth;
    unsigned char  dataType;        /* TODO: check this */
    unsigned char  dataFormat;      /* TODO: check this */
    unsigned short width;
    unsigned short height;
    unsigned long  size;
    unsigned char* data;
} InternalImage;

/* TODO: Check formats and what is luminance */

typedef enum {
    FormatRGB       = 0,
    FormatRGBA      = 1,
    FormatLuminance = 2
} DataFormat;

/* TODO: Check data types and what is luminance */

typedef enum {
    DataUnsignedByte = 0
} DataType;


/* TODO: is this needed? */
typedef struct RGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGB;

typedef struct RGBA {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} RGBA;

InternalImage* imageLoadTGA(const char* filename);
void deleteImage(InternalImage* image);

#endif
