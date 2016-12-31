#include "imageTGA.h"


/* TODO: check if the image is .tga format!!! */
/* TODO: read all and then check and quit if unsupported? */
/* TODO: add clean up functions (and create functions?) */
/* TODO: filename better naming convention */
/* TODO: make better error messages */
/* TODO: comment code */

InternalImage* loadUncompressedTGA(InternalImage* image, FILE* imageFile);
InternalImage* loadCompressedTGA(InternalImage* image, FILE* imageFile);

void imageFlipVertical(InternalImage* image);
void imageSwapRB(InternalImage* image);

InternalImage* imageLoadTGA(const char* filename) {
    /* Open the image file */
    FILE* imageFile = fopen(filename, "rb");
    if (imageFile == NULL) {
		fprintf(stderr, "TGA image: \"%s\" ", filename);
        errorFatal("fopen...");
	}

    /* Read the TGA file header */
    Header header;
    if (fread(&header, sizeof(Header), 1, imageFile) == 0) {
        fclose(imageFile);
        errorFatal("TGA header fread...");
    }

    /* Create an image */
    InternalImage* image = NULL;
    image = (InternalImage*) malloc(sizeof(InternalImage));
    if (image == NULL) {
        fclose(imageFile);
        errorFatal("Internal image malloc...");
    }

	#ifdef DEBUG
    printf("\n");
    printf("Header:\n");
    printf("    IDLength .......... %d\n",                              header.IDLength);
    printf("    colorMapType ...... %d\n",                              header.colorMapType);
    printf("    imageType ......... %d",                                header.imageType);
    if (header.imageType == TypeNoImageData)
        printf("  (Uncompressed  No image data)\n");
    else if (header.imageType == TypePaletteIndexed)
        printf("  (Uncompressed  Palette indexed)\n");
    else if (header.imageType == TypeRGB)
        printf("  (Uncompressed  RGB)\n");
    else if (header.imageType == TypeGrayscale)
        printf("  (Uncompressed  Grayscale)\n");
    else if (header.imageType == TypeCompressedPaletteIndexed)
        printf("  (Compressed  Palette indexed)\n");
    else if (header.imageType == TypeCompressedRGB)
        printf("  (Compressed  RGB)\n");
    else if (header.imageType == TypeCompressedGrayscale)
        printf("  (Compressed  Grayscale)\n");
    printf("    firstEntryIndex ... %hu\n",          (*(unsigned short*)header.firstEntryIndex));
    printf("    colorMapLength .... %hu\n",          (*(unsigned short*)header.colorMapLength));
    printf("    colorMapEntrySize . %d\n",                              header.colorMapEntrySize);
    printf("    xOrigin ........... %hu\n",          (*(unsigned short*)header.xOrigin));
    printf("    yOrigin ........... %hu\n",          (*(unsigned short*)header.yOrigin));
    printf("    imageWidth ........ %hu  pixels\n",  (*(unsigned short*)header.imageWidth));
    printf("    imageHeight ....... %hu  pixels\n",  (*(unsigned short*)header.imageHeight));
    printf("    pixelDepth ........ %d bit\n",                          header.pixelDepth);
    printf("    imageDescriptor ... %d\n",                              header.imageDescriptor);
    printf("----------\n");
	#endif

    /* Get TGA image pixel width and height */
    image->width  = *((unsigned short*)header.imageWidth);
    image->height = *((unsigned short*)header.imageHeight);

    /* TODO: what about grayscale??? */

    /* Check TGA image pixel depth */
    int bytesPerPixel = header.pixelDepth / 8;
    if (bytesPerPixel < 3) {
        fclose(imageFile);
        errorFatal("Unsupported TGA image pixel depth...");
    }

    /* Calculate TGA image size in bytes */
    image->size = image->width * image->height * bytesPerPixel;

    /* Allocate TGA image data memory */
    image->data = (unsigned char*) malloc(sizeof(unsigned char) * image->size);
    if (image->data == NULL) {
        fclose(imageFile);
        errorFatal("TGA image memory allocation...");
    }

    /* Get the rest of TGA header data into the image */
    switch (header.imageType) {
        case TypeRGB:
        case TypeCompressedRGB:
            if (bytesPerPixel == 3) {
                image->dataFormat = FormatRGB;
                image->dataType   = DataUnsignedByte;
                image->colorDepth = 24;
            } else {
                image->dataFormat = FormatRGBA;
                image->dataType   = DataUnsignedByte;
                image->colorDepth = 32;
            }
            break;
        case TypeGrayscale:
        case TypeCompressedGrayscale:
            image->dataFormat = FormatLuminance;
            image->dataType   = DataUnsignedByte;
            image->colorDepth = 8;
            break;
    }

    /* Skip ID block */
    if (header.IDLength > 0) {
        if (fseek(imageFile, SEEK_CUR, header.IDLength) == -1) {
            fclose(imageFile);
            errorFatal("TGA ID block fseek...");
        }
    }

    /* TODO: return value errors?? */

    /* Check image type */
    if (header.imageType == TypeRGB) {
        image = loadUncompressedTGA(image, imageFile);
    } else if (header.imageType == TypeCompressedRGB) {
        image = loadCompressedTGA(image, imageFile);
    } else {
        fclose(imageFile);
        errorFatal("Unsupported TGA image type...");
    }

    fclose(imageFile);

    return image;
}

InternalImage* loadUncompressedTGA(InternalImage* image, FILE* imageFile) {
    /* Read pixel bytes */
    if (fread(image->data, 1, image->size, imageFile) != image->size) {
        fclose(imageFile);
        errorFatal("Uncompressed TGA image fread...");
    }

    imageSwapRB(image);
    //imageFlipVertical(image);

    return image;
}

InternalImage* loadCompressedTGA(InternalImage* image, FILE* imageFile) {
	UNUSED(image);
	UNUSED(imageFile);

    return image;
}

void imageFlipVertical(InternalImage* image) {
    if (image == NULL)
        return;

    unsigned char tmpR;
    unsigned char tmpG;
    unsigned char tmpB;
    unsigned char tmpA;

    int bytesPerPixel = image->colorDepth / 8;
    int height = image->height;
    int halfHeight = height / 2;
    int width  = image->width * bytesPerPixel;

    //10 x 10 = 100
    //40 x 40 = 400
    //
    //    0 1 2 3 4 5 6 7
    // 0  o o o o o o o o
    // 0  o o o o o o o o
    // 0  o o o o o o o o
    //              3 x 2

    /* TODO: check border (last 4 bytes) with width */
    for (int i = 0; i < halfHeight; i++) {
        for (int j = 0; j < width; j += bytesPerPixel) {
            tmpR = (image->data)[i * width + j    ];
            tmpG = (image->data)[i * width + j + 1];
            tmpB = (image->data)[i * width + j + 2];
            tmpA = (image->data)[i * width + j + 3];

            (image->data)[i * width + j    ] = (image->data)[(height - 1 - i) * width + j    ];
            (image->data)[i * width + j + 1] = (image->data)[(height - 1 - i) * width + j + 1];
            (image->data)[i * width + j + 2] = (image->data)[(height - 1 - i) * width + j + 2];
            (image->data)[i * width + j + 3] = (image->data)[(height - 1 - i) * width + j + 3];

            (image->data)[(height - 1 - i) * width + j    ] = tmpR;
            (image->data)[(height - 1 - i) * width + j + 1] = tmpG;
            (image->data)[(height - 1 - i) * width + j + 2] = tmpB;
            (image->data)[(height - 1 - i) * width + j + 3] = tmpA;
        }
    }
}

void imageSwapRB(InternalImage* image) {
    if (image == NULL)
        return;

    int bytesPerPixel = image->colorDepth / 8;
    for (unsigned long i = 0; i < image->size; i += bytesPerPixel) {
        /* Red and Blue pixel components XOR swap */
        (image->data)[i    ] = (image->data)[i] ^ (image->data)[i + 2];
        (image->data)[i + 2] = (image->data)[i] ^ (image->data)[i + 2];
        (image->data)[i    ] = (image->data)[i] ^ (image->data)[i + 2];
    }
}

void deleteImage(InternalImage* image) {
	free(image->data);
	free(image);
}
