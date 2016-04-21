
#include <fstream>
#include "ScreenCam.h"
#include "ScreenCam.h"
#include "Object.h"
#include "ObjectCells.h"
#include "Colour.h"

void AddPolygonToPolygonList(PolygonPtr current_polygon, PolygonListPtr *poly_list);

void MakeSurfaces(int no_surfaces, ObjectPtr *object_head,
	int *no_vertices, VertexPtr *vertex_head, int *surface_at, VertexPtr *vertex_at
	, string object_file, int no_polygons, ifstream *, string, Vector *vertexNormals);
void ReadVertices(int no_vertices, VertexPtr vertex_head, int *surface_at, VertexPtr *vertex_at, ifstream *, string *);
void loadNormals(ifstream *input, Vector *vertexNormals, string *buffer);
int LoadObject(string object_name, int no_of_objects, ObjectPtr *object_head, ViewPointRec *view_ref_point, Matrix *view_transformation, int *factor);
