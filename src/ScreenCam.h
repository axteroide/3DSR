#pragma once


#include <cmath>
#include "Matrix.h"
#include "Utils.h"

#define DEV_MAX_X_RES 1024 // Monitor X resolution
#define DEV_MAX_Y_RES 768  // Monitor Y resolution
#define DEV_MAX_Z_RES 255  // Monitor Frame buffer bit-depth


typedef int ZBuffer[DEV_MAX_X_RES][DEV_MAX_Y_RES];

struct ScreenRec{
	int x, y, z;
};

struct ViewPointRec{
	int view_plane_dist, rho, theta, phi;
	float sin_theta, cos_theta, sin_phi, cos_phi, x_view, y_view, z_view;
};

struct MapRec{
	int xmid, ymid;
};

struct ZParamRec{
	float z_min, z_max, z_range;
};



void InitializeZBuffer(int **);
void SetViewVariables(ViewPointRec *view_ref_point, Matrix * view_transformation);
void SetViewTransformation(ViewPointRec *view_point_ref, Matrix  view_transformation);
Vector VectorOjo(Vector vertex, ViewPointRec view_ref_point);
void GetViewRefPoint(ViewPointRec *view_ref_point, Matrix *view_transformation, int *factor);