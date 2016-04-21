#pragma once
#include "Vertex.h"
#include "Vector.h"
#include "Polygon.h"
#include "ScreenCam.h"

typedef struct SurfaceCell  * SurfacePtr;
struct SurfaceCell{
	int no_polygons;
	PolygonCell *polygon_head;
	SurfacePtr next;
};

struct PolygonCell{
	VertexListPtr vertex_list_head;
	Vector poly_normal;
	bool poly_visible;
	PolygonPtr next;
};

struct VertexCell{
	Vector local_pos, world_pos, eye_pos, vertex_normal;
	ScreenRec screen_pos;
	PolygonListPtr poly_list_head;
	VertexPtr next;
};
