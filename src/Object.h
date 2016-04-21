#pragma once
#include <string>
#include "Surface.h"
#include "Matrix.h"
#include "Vertex.h"
#include "ObjectCells.h"
#include "Colour.h"

typedef struct ObjectCell * ObjectPtr;

struct ObjectCell{
	std::string name;
	int id_no, no_vertices, no_surfaces;
	SurfacePtr surface_head;
	VertexPtr vertex_head;
	Matrix transformation;
	bool selected;
	bool vertexInFile;
	ObjectCell *next;
	DEV_COLOUR col;
};
