#include "objreader.h"

int nVerts;
float *vertices;
float *normals;
float *texcoords;
int nIndices;
int *indices;

ObjReader::readObj("teapot.obj", nVerts, &vertices, &normals, &texcoords, nIndices, &indices);

