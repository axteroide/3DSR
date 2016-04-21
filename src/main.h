
#define DEBUG 0

//
//#include <stdlib.h>
//#ifdef __APPLE__
//#include <GLUT/glut.h>
//#else
//#include <GL/glut.h>
//#endif

#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <conio.h>
#include <time.h>
#include <sstream>
#include <windows.h>

#include "Iluminacion.h"
#include "LoadObject.h"
#include "Menus.h"
#include "LecturaFicheros.h"



int DEV_X_RES; // Desde 0 hasta DEV_MAX_X_RES
int DEV_Y_RES;
int DEV_Z_RES;




void CalculateScreenCoordinates(ObjectPtr current_object, ZParamRec z_params,MapRec map_offsets,ViewPointRec view_ref_point, int factor);
void CalculateEyeCoordinates(ObjectPtr current_object, ZParamRec *z_pararms,Matrix view_transformation);
void CalculateAPolygonNormal(PolygonPtr current_polygon);
void CalculatePolygonNormals(ObjectPtr current_object);
void CalculateAVertexNormal(VertexPtr current_vertex);
void CalculateVertexNormals(ObjectPtr current_object);
void CalculateNormals(ObjectPtr current_object);
void changeSize(int w, int h);
void DEV_CLEAR_SCREEN();
void DEV_DRAW_TO(float x, float y, float x2, float y2, DEV_COLOUR colour);
void doObjectsNormals(ObjectPtr *object_head);
double DotProduct(Vector v1, Vector v2);

void hideObjects(ObjectPtr *object_head,  ViewPointRec view_ref_point);
void imprimeStats();
void Initialize(MapRec *map_offsets, ViewPointRec *view_ref_point, Matrix *view_transformation, bool *scene_changed, int *no_of_objects, ObjectPtr *object_head,struct ValoresIluminacion *);

void mouse(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void MultiplyMatrices (Matrix m1, Matrix m2, Matrix m3);

//float Magnitude(Vector v);
//int Normalize(Vector v1, Vector *v2);
void OpenFile(string object_name);
void Perspective(Vector pos, ViewPointRec view_ref_point, 
				MapRec map_offsets, int *screen_x, int *screen_y,int *screen_z, int factor);


void render();
void printMenu(void);
void RemovePolygonIfHidden(PolygonPtr current_polygon, ViewPointRec view_ref_point);
void RemoveHiddenSurfaces(ObjectPtr current_object,ViewPointRec view_ref_point);
void SwapBuffers();
void TransformToWorldCoordinates(ObjectPtr *current_object);
void TransformScene(MapRec map_offsets, Matrix view_transformation, ViewPointRec view_ref_point, 
					ObjectPtr *object_head, ZParamRec *z_params, bool *scene_changed, int factor);
void WireFrameScene(MapRec map_offsets, Matrix view_transformation, 
					ViewPointRec view_ref_point, ObjectPtr object_head, ScreenRec ejes[], int factor);
void imprimeStats();
