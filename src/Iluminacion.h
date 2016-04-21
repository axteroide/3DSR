

#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>

#include "Matrix.h"
#include "ScreenCam.h"
#include "Object.h"
#include "ObjectCells.h"
#include "PerlinNoise.h"
#include "Colour.h"
#include "StrucIluminacion.h"
using namespace std;

#define PI 3.14159265

typedef struct EdgeBox * EdgeBoxPtr;

/*
struct DEV_COLOUR{
	float r,g,b; // Han de ser de tipo DEV_Z_RES
}; 
*/



struct EdgeBox{
	float x,z,r;
	Vector w ,n;
	EdgeBoxPtr next;
};

typedef struct ObjectCell * ObjectPtr;
typedef EdgeBoxPtr EdgeBuffer[DEV_MAX_Y_RES];










//float ArcTangent(float x, float z);
void AddEdgeToList (VertexPtr vertex1, VertexPtr vertex2, EdgeBuffer edge_list_at);

void DEV_DRAW_TO(float x, float y, float x2, float y2, DEV_COLOUR colour);
void DEV_DRAW_PIXEL(float x, float y, DEV_COLOUR colour);
void dibujaEjes(ScreenRec ejes[5]);
void imprimeFloat(string cad, float num);
void imprimeIlu(struct ValoresIluminacion);
void imprimeInt(string, int);
//void InitializeZBuffer(int **);
void RenderPolygon(PolygonPtr current_polygon, EdgeBuffer edge_list_at, Vector light_vector, int ** z_buffer, ViewPointRec view_ref_point, struct ValoresIluminacion, bool threeD[]);
void RenderObject(ObjectPtr current_object, EdgeBuffer edge_list_at, Vector light_vector, int ** z_buffer, ViewPointRec view_ref_point, struct ValoresIluminacion, bool threeD[]);
void RenderPixel(int x,int y, float i, float r, Vector w, struct ValoresIluminacion);
void RenderSpan (int y, EdgeBoxPtr *edge_box1, EdgeBoxPtr * edge_box2, int ** z_buffer_at, struct ValoresIluminacion, ViewPointRec view_ref_point, bool threeD[]);
void RenderScene(ObjectPtr object_head, ViewPointRec view_ref_point, Matrix view_transformation, struct ValoresIluminacion, bool threeD[]);
void renderString(void *font, string s);
//float WoodGrain(Vector w);
