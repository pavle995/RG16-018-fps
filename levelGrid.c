#include "levelGrid.h"

Grid* createGrid(int width, int height, char** grid) {
    Grid* newGrid = NULL;
    /* Allocate Grid memory */
    newGrid = (Grid*) malloc(sizeof(Grid));
    if (newGrid == NULL)
        errorFatal("createGrid newGrid malloc...");
    newGrid->data = (char**) malloc(height * sizeof(char*));
    if (newGrid->data == NULL)
        errorFatal("createGrid newGrid data height malloc...");
    for (int i = 0; i < height; i++) {
        (newGrid->data)[i] = (char*) malloc(width * sizeof(char));
        if ((newGrid->data)[i] == NULL)
            errorFatal("createGrid newGrid data width malloc...");
    }
    /* Fill the new Grid object data */
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            (newGrid->data)[i][j] = grid[i][j];
        }
    }
    newGrid->height = height;
    newGrid->width  = width;
    /* Return the new object pointer */
    return newGrid;
}

Grid* loadGrid(const char* fileName) {
    Grid* newGrid = NULL;
    /* Open grid file for reading */
    //printf("Filename: %s\n", fileName);
    FILE* inputFile = fopen(fileName, "r");
    if (inputFile == NULL)
        errorFatal("loadGrid inputFile fopen...");
    int lineLength = 200;
    char line[lineLength];
    int lineNumber = 0;

    int tmpWidth;
    int tmpHeight;
    char** tmpData;

    /* Read file lines */
    while (fgets(line, lineLength, inputFile) != NULL) {
        /* Check file type */
        if (lineNumber == 0) {
            line[strlen(line)-1] = '\0';
            if (strcmp(line, "grid file") != 0)
                errorFatal("loadGrid incorectFile...");
        }
        /* Get grid dimensions */
        else if (lineNumber == 1) {
            sscanf(line, "%d %d", &tmpHeight, &tmpWidth);
            //printf("Width: %d\nHeight: %d\n", tmpWidth, tmpHeight);
            /* Create temporary grid data */
            tmpData = (char**) malloc(tmpHeight * sizeof(char*));
            if (tmpData == NULL)
                errorFatal("loadGrid tmpData malloc...");
            for (int i = 0; i < tmpHeight; i++) {
                tmpData[i] = (char*) malloc(tmpWidth * sizeof(char));
                if (tmpData[i] == NULL)
                    errorFatal("loadGrid tmpData[i] malloc...");
            }
        }
        /* Read lines */
        else {
            int i = lineNumber - 2;
            strncpy(tmpData[i], line, tmpWidth);
        }
        //printf("%s", line);
        lineNumber++;
    }

    // printf("Read grid:\n");
    // for (int i = 0; i < tmpHeight; i++) {
    //     for (int j = 0; j < tmpWidth; j++)
    //         printf("%c", tmpData[i][j]);
    //     printf("\n");
    // }

    /* Convert to correct grid format */
    int emptyCells   = 0;
    int blockedCells = 0;
    int unusedCells  = 0;
    int unknownCells = 0;
    for (int i = 0; i < tmpHeight; i++) {
        for (int j = 0; j < tmpWidth; j++) {
            if (tmpData[i][j] == ' ') {
                tmpData[i][j] = emptyCell;
                emptyCells++;
            } else if (tmpData[i][j] == '#') {
                tmpData[i][j] = blockedCell;
                blockedCells++;
            } else if (tmpData[i][j] == '.') {
                tmpData[i][j] = unusedCell;
                unusedCells++;
            } else {
                tmpData[i][j] = unknownCell;
                unknownCells++;
            }
        }
    }

    newGrid = createGrid(tmpWidth, tmpHeight, tmpData);
    newGrid->emptyCells   = emptyCells;
    newGrid->blockedCells = blockedCells;
    newGrid->unusedCells  = unusedCells;
    newGrid->unknownCells = unknownCells;
    free(tmpData);

    // printf("Data in grid:\n");
    // for (int i = 0; i < newGrid->height; i++) {
    //     for (int j = 0; j < newGrid->width; j++)
    //         printf("%d", (newGrid->data)[i][j]);
    //     printf("\n");
    // }

    fclose(inputFile);
    return newGrid;
}

// TODO: Create a graph for navigation and path finding

// typedef enum {
//     upLeftDirection = 0, upDirection, upRightDirection,
//     leftDirection, rightDirection,
//     downLeftDirection, downDirection, downRightDirection = 7
// } Direction;
//
// #define UP      [i - 1][j]
// #define DOWN    [i + 1][j]
// #define LEFT    [i][j - 1]
// #define RIGHT   [i][j + 1]
// #define UP_LEFT      [i - 1][j - 1]
// #define UP_RIGHT     [i - 1][j + 1]
// #define DOWN_LEFT    [i + 1][j - 1]
// #define DOWN_RIGHT   [i + 1][j + 1]
//
// #define UNUSED_CELL -1

// Graph* createGraph(Grid* grid) {
//
// }

// Node* findNode(int y, int x, Graph* graph) {
//
// }

// void findPath(Grid* grid, Graph* graph, int startX, int startY, int targetX, int targetY) {
//
// }
