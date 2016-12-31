#include "modelLoader.h"

extern int nullTexture;

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
    int vnCount = 0;
    int vtCount = 0;
	int fCount = 0;

	/* Count number of verticies and triangles, and get material library name */
	while(fgets(line, lineLength, inputFile) != NULL) {
        sscanf(line, "%c", &type);
        if (type == 'v') {
            if (line[1] == ' ')
                vCount++;
            else if (line[1] == 'n')
                vnCount++;
            else if (line[1] == 't')
                vtCount++;
        }
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

    Vec3f* vertexNormals = (Vec3f*) malloc(vnCount * sizeof(Vec3f));
    if (vertexNormals == NULL)
        errorFatal("Malloc error");

    Vec2f* textureCoordinates = (Vec2f*) malloc(vtCount * sizeof(Vec2f));
    if (textureCoordinates == NULL)
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
						materials[readMaterials - 1].textureID = tmpMaterial.textureID;
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
			/* Read texture maps */
			else if (type == 'm') {
				/* Diffuse texture map */
				sscanf(line, "%s", tmpWord);
				if (strcmp(tmpWord, "map_Kd") == 0) {
					sscanf(line, "map_Kd %s", tmpWord);
					/* No texture */
					if (tmpWord[0] == '/') {
						tmpMaterial.textureID = nullTexture;
					}
					/* Load texture image file and bind it to an ID */
					else {
						sprintf(tmpWord, "images/");
						int pos = strlen(tmpWord);
						sscanf(line, "map_Kd %s", (tmpWord + pos));
						glGenTextures(1, &(tmpMaterial.textureID));

						printf("IMAGE: %s\n", tmpWord);
						printf("Whole line: %s", line);

						InternalImage* image;
						image = imageLoadTGA(tmpWord);

						glBindTexture(GL_TEXTURE_2D, tmpMaterial.textureID);
						printf("Binded texture to ID: %d\n", tmpMaterial.textureID);						

						glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_LINEAR
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_LIENAR
						if (image->dataFormat == FormatRGB) {
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
										 image->width, image->height, 0,
										 GL_RGB, GL_UNSIGNED_BYTE, image->data);
						} else if (image->dataFormat == FormatRGBA) {
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
										 image->width, image->height, 0,
										 GL_RGBA, GL_UNSIGNED_BYTE, image->data);
						}

						
						
						deleteImage(image);
					}
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
		materials[readMaterials - 1].textureID = tmpMaterial.textureID;

		/* TODO: debug text */
		//for (int i = 0; i < readMaterials; i++) {
		//	printf("MaterialID: %d\n", materials[i].id);
		//	printf("    TextureID: %d\n", materials[i].textureID);
		//}
	}

	/* Read model data */
	rewind(inputFile);

    int currentMaterial = 0;    /* TODO make triangles with no material get a NULL material */
    int currentSmoothShade = 0;

    int vRead = 0;
    int vnRead = 0;
    int vtRead = 0;
	int fRead = 0;
	while(fgets(line, lineLength, inputFile) != NULL) {
		sscanf(line, "%c", &type);
		if (type == 'v') {
            if (line[1] == ' ') {
                /* Vertex */
    			sscanf(line, "v %f %f %f",
    				&(verticies[vRead].x),
    				&(verticies[vRead].y),
    				&(verticies[vRead].z));
    			vRead++;
            }
            else if (line[1] == 'n') {
                /* Vertex normal */
                sscanf(line, "vn %f %f %f",
                    &(vertexNormals[vnRead].x),
                    &(vertexNormals[vnRead].y),
                    &(vertexNormals[vnRead].z));
    			vnRead++;
            }
            else if (line[1] == 't') {
                /* Texture coordinates */
                sscanf(line, "vt %f %f",
                    &(textureCoordinates[vtRead].x),
                    &(textureCoordinates[vtRead].y));
    			vtRead++;
            }
		} else if (type == 'f') {
            /* Triangle face */
            /* Check line type */
            int faceType = 0;
            char slash1, slash2;

            /* See comment bellow for magic number identification */
            sscanf(line, "f %*d%c",&slash1);
            if (slash1 == ' ')
                faceType = 0;               /* 0 - Vertex only              v0 v1 v2  */
            else if (slash1 == '/') {
                sscanf(line, "f %*d%c%c", &slash1, &slash2);
                if (slash2 == '/')
                    faceType = 3;           /* 3 - Vertex//Normal           v0//vn0 v1//vn1 v2//vn2  */
                else {
                    sscanf(line, "f %*d%c%*d%c", &slash1, &slash2);
                    if (slash2 == '/')
                        faceType = 2;       /* 2 - Vertex/Texcoord/Normal   v0/vt0/vn0 v1/vt1/vn1 v2/vt2/vn2  */
                    else
                        faceType = 1;       /* 1 - Vertex/Texcoord          v0/vt0 v1/vt1 v2/vt2  */
                }
            }
            else {
                fprintf(stderr, "Line: %s\n", line);
                errorFatal("Bad vertex format");
            }

            /* 0 - Vertex only              v0 v1 v2  */
            if (faceType == 0) {
                sscanf(line, "f %d %d %d",
    				&(triangles[fRead].v0),
    				&(triangles[fRead].v1),
    				&(triangles[fRead].v2));

                /* Set unused texcoord and normals to 0 */
                triangles[fRead].texcoord0.x = 0;
                triangles[fRead].texcoord0.y = 0;
                triangles[fRead].texcoord1.x = 0;
                triangles[fRead].texcoord1.y = 0;
                triangles[fRead].texcoord2.x = 0;
                triangles[fRead].texcoord2.y = 0;

                triangles[fRead].vn0.x = 0;
                triangles[fRead].vn0.y = 0;
                triangles[fRead].vn0.z = 0;
                triangles[fRead].vn1.x = 0;
                triangles[fRead].vn1.y = 0;
                triangles[fRead].vn1.z = 0;
                triangles[fRead].vn2.x = 0;
                triangles[fRead].vn2.y = 0;
                triangles[fRead].vn2.z = 0;
            }
            /* 1 - Vertex/Texcoord          v0/vt0 v1/vt1 v2/vt2  */
            else if (faceType == 1) {
                int tc0, tc1, tc2;
                sscanf(line, "f %d/%d %d/%d %d/%d",
    				&(triangles[fRead].v0),
                    &tc0,
    				&(triangles[fRead].v1),
                    &tc1,
    				&(triangles[fRead].v2),
                    &tc2);

                triangles[fRead].texcoord0.x = textureCoordinates[tc0 - 1].x;
                triangles[fRead].texcoord0.y = textureCoordinates[tc0 - 1].y;
                triangles[fRead].texcoord1.x = textureCoordinates[tc1 - 1].x;
                triangles[fRead].texcoord1.y = textureCoordinates[tc1 - 1].y;
                triangles[fRead].texcoord2.x = textureCoordinates[tc2 - 1].x;
                triangles[fRead].texcoord2.y = textureCoordinates[tc2 - 1].y;

                /* Set unused normals to 0 */
                triangles[fRead].vn0.x = 0;
                triangles[fRead].vn0.y = 0;
                triangles[fRead].vn0.z = 0;
                triangles[fRead].vn1.x = 0;
                triangles[fRead].vn1.y = 0;
                triangles[fRead].vn1.z = 0;
                triangles[fRead].vn2.x = 0;
                triangles[fRead].vn2.y = 0;
                triangles[fRead].vn2.z = 0;
            }
            /* 2 - Vertex/Texcoord/Normal   v0/vt0/vn0 v1/vt1/vn1 v2/vt2/vn2  */
            else if (faceType == 2) {
                int tc0, tc1, tc2;
                int n0, n1, n2;
                sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
    				&(triangles[fRead].v0),
                    &tc0,
                    &n0,
    				&(triangles[fRead].v1),
                    &tc1,
                    &n1,
    				&(triangles[fRead].v2),
                    &tc2,
                    &n2);

                triangles[fRead].texcoord0.x = textureCoordinates[tc0 - 1].x;
                triangles[fRead].texcoord0.y = textureCoordinates[tc0 - 1].y;
                triangles[fRead].texcoord1.x = textureCoordinates[tc1 - 1].x;
                triangles[fRead].texcoord1.y = textureCoordinates[tc1 - 1].y;
                triangles[fRead].texcoord2.x = textureCoordinates[tc2 - 1].x;
                triangles[fRead].texcoord2.y = textureCoordinates[tc2 - 1].y;

                triangles[fRead].vn0.x = vertexNormals[n0 - 1].x;
                triangles[fRead].vn0.y = vertexNormals[n0 - 1].y;
                triangles[fRead].vn0.z = vertexNormals[n0 - 1].z;
                triangles[fRead].vn1.x = vertexNormals[n1 - 1].x;
                triangles[fRead].vn1.y = vertexNormals[n1 - 1].y;
                triangles[fRead].vn1.z = vertexNormals[n1 - 1].z;
                triangles[fRead].vn2.x = vertexNormals[n2 - 1].x;
                triangles[fRead].vn2.y = vertexNormals[n2 - 1].y;
                triangles[fRead].vn2.z = vertexNormals[n2 - 1].z;
            }
            /* 3 - Vertex//Normal           v0//vn0 v1//vn1 v2//vn2  */
            else if (faceType == 3) {
                int n0, n1, n2;
                sscanf(line, "f %d//%d %d//%d %d//%d",
    				&(triangles[fRead].v0),
                    &n0,
    				&(triangles[fRead].v1),
                    &n1,
    				&(triangles[fRead].v2),
                    &n2);

                triangles[fRead].texcoord0.x = 0;
                triangles[fRead].texcoord0.y = 0;
                triangles[fRead].texcoord1.x = 0;
                triangles[fRead].texcoord1.y = 0;
                triangles[fRead].texcoord2.x = 0;
                triangles[fRead].texcoord2.y = 0;

                triangles[fRead].vn0.x = vertexNormals[n0 - 1].x;
                triangles[fRead].vn0.y = vertexNormals[n0 - 1].y;
                triangles[fRead].vn0.z = vertexNormals[n0 - 1].z;
                triangles[fRead].vn1.x = vertexNormals[n1 - 1].x;
                triangles[fRead].vn1.y = vertexNormals[n1 - 1].y;
                triangles[fRead].vn1.z = vertexNormals[n1 - 1].z;
                triangles[fRead].vn2.x = vertexNormals[n2 - 1].x;
                triangles[fRead].vn2.y = vertexNormals[n2 - 1].y;
                triangles[fRead].vn2.z = vertexNormals[n2 - 1].z;
            }

            triangles[fRead].v0--;
            triangles[fRead].v1--;
            triangles[fRead].v2--;
            triangles[fRead].materialID = currentMaterial;
            triangles[fRead].smooth = currentSmoothShade;

            fRead++;

        } else if (type == 'u') {
            /* Material */
            sscanf(line, "%s", tmpWord);
            if (strcmp(tmpWord, "usemtl") == 0) {
                sscanf(line, "usemtl %s", tmpWord);

                /* Find what material ID is that material name */
                for (int i = 0; i < materialCount; i++) {
                    if (strcmp(tmpWord, materialNames[i]) == 0) {
                        currentMaterial = i;
                        break;
                    }
                }
            }
        } else if (type == 's') {
            /* Smooth shade */
            int tmp;
            sscanf(line, "s %d", &tmp);
            if (tmp == 1)
                currentSmoothShade = 1;
            else
                currentSmoothShade = 0;
        }
	}

    model.verticies = verticies;
    model.triangles = triangles;
    model.materials = materials;
    model.numberOfVerticies = vCount;
    model.numberOfTriangles = fCount;
    model.numberOfMaterials = materialCount;

    free(vertexNormals);
    free(textureCoordinates);
    fclose(inputFile);
    // fclose(materialFile); /* TODO Closing it gives error */

    return model;
}
