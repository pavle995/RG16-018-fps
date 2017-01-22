#ifndef LEVELGRID_H
#define LEVELGRID_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "util.h"

typedef enum {
    emptyCell, blockedCell, unusedCell, unknownCell
} Cell;

// typedef struct Node {
//     int numberOfEdges;
//     struct Node** edges;
//     int gridX;
//     int gridY;
//     Cell state;
//     int marked;
// } Node;
//
// typedef struct Graph {
//     int numberOfNodes;
//     Node** nodes;
// } Graph;

typedef struct Grid {
    int width;
    int height;
    int emptyCells;
    int blockedCells;
    int unusedCells;
    int unknownCells;
    int marked;
    char** data;
} Grid;

Grid*  loadGrid(const char* fileName);
// Graph* createGraph(Grid* grid);

#endif
