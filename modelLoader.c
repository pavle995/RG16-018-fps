#include "modelLoader.h"

Model loadModel(const char* filename) {
    Model model;

    FILE* inputFile = fopen(filename, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "file: %s\n", filename);
        errorFatal("Error opening model file");
    }

	int lineLength = 200;
	char line[lineLength];
	char type;

	/* Count number of verticies and triangles */
	int vCount = 0;
	int fCount = 0;
	while(fgets(line, lineLength, inputFile) != NULL) {
		sscanf(line, "%c", &type);
		if (type == 'v')
			vCount++;
		else if (type == 'f')
			fCount++;
	}

	/* Allocate memory */
	Vertex* verticies = (Vertex*) malloc(vCount * sizeof(Vertex));
	if (verticies == NULL)
		errorFatal("Malloc error");

	Triangle* triangles = (Triangle*) malloc(fCount * sizeof(Triangle));
	if (triangles == NULL)
		errorFatal("Malloc error");

	/* Read model data */
	rewind(inputFile);

	int vRead = 0;
	int fRead = 0;
	while(fgets(line, lineLength, inputFile) != NULL) {
		sscanf(line, "%c", &type);
		if (type == 'v') {
			sscanf(line, "v %f %f %f",
				&(verticies[vRead].x),
				&(verticies[vRead].y),
				&(verticies[vRead].z));
			vRead++;
		} else if (type == 'f') {
			sscanf(line, "f %d %d %d",
				&(triangles[fRead].v0),
				&(triangles[fRead].v1),
				&(triangles[fRead].v2));

				triangles[fRead].v0--;
				triangles[fRead].v1--;
				triangles[fRead].v2--;
			fRead++;
        }
	}

    fclose(inputFile);

    model.verticies = verticies;
    model.triangles = triangles;
    model.numberOfVerticies = vCount;
    model.numberOfTriangles = fCount;

    return model;
}
