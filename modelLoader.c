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
    char tmpWord[100];

    FILE* materialFile = NULL;
    int hasMaterials = 0;
    char materialFileName[100];
    char** materialNames;
    Material* materials = NULL;
    int materialCount = 0;

    int vCount = 0;
	int fCount = 0;

	/* Count number of verticies and triangles, and get material library name */
	while(fgets(line, lineLength, inputFile) != NULL) {
        sscanf(line, "%c", &type);
        if (type == 'v')
        	vCount++;
        else if (type == 'f')
        	fCount++;

        /* Get material library name */
        else if (type == 'm') {
            sscanf(line, "%s", tmpWord);
            if (strcmp(tmpWord, "mtllib") == 0) {
                hasMaterials = 1;
                sscanf(line, "mtllib %s", tmpWord);
                strcpy(materialFileName, tmpWord);
            }
        }
	}

	/* Allocate vertex and triangle memory */
	Vertex* verticies = (Vertex*) malloc(vCount * sizeof(Vertex));
	if (verticies == NULL)
		errorFatal("Malloc error");

	Triangle* triangles = (Triangle*) malloc(fCount * sizeof(Triangle));
	if (triangles == NULL)
		errorFatal("Malloc error");

    /* Read materials */
    if (hasMaterials) {
        /* Add directory path to the material file name */
        int nameLength = strlen(filename);
        int pos = 0;
        for (int i = nameLength; i >= 0; i--) {
            if (filename[i] == '/') {
                pos = i;
                break;
            }
        }
        strcpy(tmpWord, materialFileName);
        strcpy(materialFileName, filename);
        if (pos != 0) {
            materialFileName[pos] = '/';
            pos++;
        }
        strcpy(materialFileName + pos, tmpWord);

        materialFile = fopen(materialFileName, "r");

        /* Count new materials */
        while(fgets(line, lineLength, materialFile) != NULL) {
    		sscanf(line, "%c", &type);
            if (type == 'n') {
                sscanf(line, "%s", tmpWord);
                if (strcmp(tmpWord, "newmtl") == 0) {
                    materialCount++;
                }
            }
    	}

        /* Allocate material memory */
        materials = (Material*) malloc(materialCount * sizeof(Material));
        if (materials == NULL)
    		errorFatal("Malloc error");

        materialNames = (char**) malloc(materialCount * sizeof(char*));
        if (materialNames == NULL)
            errorFatal("Malloc error");

        /* Read material data */
        rewind(materialFile);

        Material tmpMaterial;
        float tmp0 = 0;
        float tmp1 = 0;
        float tmp2 = 0;
        int readMaterials = 0;
        while(fgets(line, lineLength, materialFile) != NULL) {
            sscanf(line, "%c", &type);
            if (type == 'n') {
                /* New material */
                sscanf(line, "%s", tmpWord);
                if (strcmp(tmpWord, "newmtl") == 0) {
                    sscanf(line, "newmtl %s", tmpWord);
                    int wordLength = strlen(tmpWord) + 1;
                    materialNames[readMaterials] = (char*) malloc(wordLength * sizeof(char));
                    if (materialNames[readMaterials] == NULL)
                        errorFatal("Malloc error");
                    strcpy(materialNames[readMaterials], tmpWord);
                    tmpMaterial.id = readMaterials;

                    /* Save the previous material */
                    if (readMaterials > 0) {
                        materials[readMaterials - 1].id = tmpMaterial.id - 1;
                        materials[readMaterials - 1].shininess = tmpMaterial.shininess;
                        materials[readMaterials - 1].ambient[0] = tmpMaterial.ambient[0];
                        materials[readMaterials - 1].ambient[1] = tmpMaterial.ambient[1];
                        materials[readMaterials - 1].ambient[2] = tmpMaterial.ambient[2];
                        materials[readMaterials - 1].diffuse[0] = tmpMaterial.diffuse[0];
                        materials[readMaterials - 1].diffuse[1] = tmpMaterial.diffuse[1];
                        materials[readMaterials - 1].diffuse[2] = tmpMaterial.diffuse[2];
                        materials[readMaterials - 1].specular[0] = tmpMaterial.specular[0];
                        materials[readMaterials - 1].specular[1] = tmpMaterial.specular[1];
                        materials[readMaterials - 1].specular[2] = tmpMaterial.specular[2];
                        materials[readMaterials - 1].emission[0] = tmpMaterial.emission[0];
                        materials[readMaterials - 1].emission[1] = tmpMaterial.emission[1];
                        materials[readMaterials - 1].emission[2] = tmpMaterial.emission[2];
                        materials[readMaterials - 1].alpha = tmpMaterial.alpha;
                        materials[readMaterials - 1].illuminationModel = tmpMaterial.illuminationModel;
                    }
                    readMaterials++;
                }
            } else if (type == 'N') {
                if (line[1] == 's') {
                    /* Shininess */
                    sscanf(line, "Ns %f", &tmp0);
                    tmpMaterial.shininess = tmp0;
                }
            } else if (type == 'K') {
                if (line[1] == 'a') {
                    /* Ambient */
                    sscanf(line, "Ka %f %f %f", &tmp0, &tmp1, &tmp2);
                    tmpMaterial.ambient[0] = tmp0;
                    tmpMaterial.ambient[1] = tmp1;
                    tmpMaterial.ambient[2] = tmp2;
                } else if (line[1] == 'd') {
                    /* Diffuse */
                    sscanf(line, "Kd %f %f %f", &tmp0, &tmp1, &tmp2);
                    tmpMaterial.diffuse[0] = tmp0;
                    tmpMaterial.diffuse[1] = tmp1;
                    tmpMaterial.diffuse[2] = tmp2;
                } else if (line[1] == 's') {
                    /* Specular */
                    sscanf(line, "Ks %f %f %f", &tmp0, &tmp1, &tmp2);
                    tmpMaterial.specular[0] = tmp0;
                    tmpMaterial.specular[1] = tmp1;
                    tmpMaterial.specular[2] = tmp2;
                } else if (line[1] == 'e') {
                    /* Emission */
                    sscanf(line, "Ke %f %f %f", &tmp0, &tmp1, &tmp2);
                    tmpMaterial.emission[0] = tmp0;
                    tmpMaterial.emission[1] = tmp1;
                    tmpMaterial.emission[2] = tmp2;
                }
            } else if (type == 'N') {
                if (line[1] == 'i') {
                    /* Don't know what this is TODO: find out */
                }
            } else if (type == 'd') {
                /* Alpha */
                sscanf(line, "d %f", &tmp0);
                tmpMaterial.alpha = tmp0;
            } else if (type == 'i') {
                sscanf(line, "%s", tmpWord);
                if (strcmp(tmpWord, "illum") == 0) {
                    sscanf(line, "illum %f", &tmp0);
                    tmpMaterial.illuminationModel = tmp0;
                }
            }
        }

        /* Save the last material */
        materials[readMaterials - 1].id = tmpMaterial.id;
        materials[readMaterials - 1].shininess = tmpMaterial.shininess;
        materials[readMaterials - 1].ambient[0] = tmpMaterial.ambient[0];
        materials[readMaterials - 1].ambient[1] = tmpMaterial.ambient[1];
        materials[readMaterials - 1].ambient[2] = tmpMaterial.ambient[2];
        materials[readMaterials - 1].diffuse[0] = tmpMaterial.diffuse[0];
        materials[readMaterials - 1].diffuse[1] = tmpMaterial.diffuse[1];
        materials[readMaterials - 1].diffuse[2] = tmpMaterial.diffuse[2];
        materials[readMaterials - 1].specular[0] = tmpMaterial.specular[0];
        materials[readMaterials - 1].specular[1] = tmpMaterial.specular[1];
        materials[readMaterials - 1].specular[2] = tmpMaterial.specular[2];
        materials[readMaterials - 1].emission[0] = tmpMaterial.emission[0];
        materials[readMaterials - 1].emission[1] = tmpMaterial.emission[1];
        materials[readMaterials - 1].emission[2] = tmpMaterial.emission[2];
        materials[readMaterials - 1].alpha = tmpMaterial.alpha;
        materials[readMaterials - 1].illuminationModel = tmpMaterial.illuminationModel;
    }

	/* Read model data */
	rewind(inputFile);

	int vRead = 0;
	int fRead = 0;
    int currentMaterial = 0;    /* TODO make triangles with no material get a NULL material */
	while(fgets(line, lineLength, inputFile) != NULL) {
		sscanf(line, "%c", &type);
		if (type == 'v') {
            /* Vertex */
			sscanf(line, "v %f %f %f",
				&(verticies[vRead].x),
				&(verticies[vRead].y),
				&(verticies[vRead].z));
			vRead++;
		} else if (type == 'f') {
            /* Triangle face */
			sscanf(line, "f %d %d %d",
				&(triangles[fRead].v0),
				&(triangles[fRead].v1),
				&(triangles[fRead].v2));

				triangles[fRead].v0--;
				triangles[fRead].v1--;
				triangles[fRead].v2--;
                triangles[fRead].materialID = currentMaterial;
			fRead++;
        } else if (type == 'u') {
            /* Material */
            sscanf(line, "%s", tmpWord);
            if (strcmp(tmpWord, "usemtl") == 0) {
                sscanf(line, "usemtl %s", tmpWord);

                /* Find what material ID ist that material name */
                for (int i = 0; i < materialCount; i++) {
                    if (strcmp(tmpWord, materialNames[i]) == 0) {
                        currentMaterial = i;
                        break;
                    }
                }
            }
        }
	}

    fclose(inputFile);
    // fclose(materialFile); /* TODO Closing it gives error */

    model.verticies = verticies;
    model.triangles = triangles;
    model.materials = materials;
    model.numberOfVerticies = vCount;
    model.numberOfTriangles = fCount;
    model.numberOfMaterials = materialCount;

    return model;
}
