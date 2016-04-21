#pragma once

#include "Vertex.h"
#include "Vector.h"

typedef struct PolygonCell * PolygonPtr;
typedef struct PolygonListCell * PolygonListPtr;

struct PolygonListCell{
	PolygonPtr poly;
	PolygonListPtr rest;
};

