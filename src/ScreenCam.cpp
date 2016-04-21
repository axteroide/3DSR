#include "ScreenCam.h"

// Inicializa el ZBuffer a el valor maximo entero
void InitializeZBuffer(int **z_buffer){
	int x, y;
	for (x = 0; x < DEV_MAX_X_RES; x++)
		for (y = 0; y < DEV_MAX_Y_RES; y++)
			z_buffer[x][y] = 32768;
}


///
// Setea las variables de vista
///
#pragma region SetViewVariables
void SetViewVariables(ViewPointRec *view_ref_point, Matrix * view_transformation){
	view_ref_point->sin_theta = 0;
	view_ref_point->cos_theta = 0;
	view_ref_point->sin_phi = 0;
	view_ref_point->cos_phi = 0;
	view_ref_point->sin_theta = sin(InRadians(view_ref_point->theta));
	view_ref_point->cos_theta = cos(InRadians(view_ref_point->theta));
	view_ref_point->sin_phi = sin(InRadians(view_ref_point->phi));
	if (view_ref_point->phi == 90){
		view_ref_point->cos_phi = 0;
	}
	else
		view_ref_point->cos_phi = cos(InRadians(view_ref_point->phi));

	view_ref_point->x_view = view_ref_point->rho * view_ref_point->cos_theta * view_ref_point->sin_phi;
	view_ref_point->y_view = view_ref_point->rho * view_ref_point->sin_theta * view_ref_point->sin_phi;
	view_ref_point->z_view = view_ref_point->rho * view_ref_point->cos_phi;
	SetViewTransformation(view_ref_point, *view_transformation);
}
#pragma endregion

#pragma region SetViewTransformation
///
// Setea la matriz de transformacion de el ojo
///
void SetViewTransformation(ViewPointRec *view_point_ref, Matrix  view_transformation){
	SetIdentityMatrix(view_transformation);
	view_transformation[0][0] = (-view_point_ref->sin_theta);
	view_transformation[0][1] = (-view_point_ref->cos_theta) * view_point_ref->cos_phi;
	view_transformation[0][2] = (-view_point_ref->cos_theta) * view_point_ref->sin_phi;
	view_transformation[1][0] = view_point_ref->cos_theta;
	view_transformation[1][1] = (-view_point_ref->sin_theta) * view_point_ref->cos_phi;
	view_transformation[1][2] = (-view_point_ref->sin_theta) * view_point_ref->sin_phi;
	view_transformation[2][1] = view_point_ref->sin_phi;
	view_transformation[2][2] = (-view_point_ref->cos_phi);
	view_transformation[3][2] = view_point_ref->rho;

}
#pragma endregion


///
// Obtiene Rho, Theta, Phi, d, y el factor de escala preguntandoselo al usuario
///
#pragma region GetViewRefPoint
void GetViewRefPoint(ViewPointRec *view_ref_point, Matrix *view_transformation, int *factor){
	fflush(stdin);
	cout << endl;
	cout << "Enter new view point details -" << endl;
	cout << "Rho (currently ', rho:1, '): ";
	cin >> view_ref_point->rho;
	cout << endl;
	fflush(stdin);
	cout << "Theta (currently ',theta:1, '): ";
	cin >> view_ref_point->theta;
	cout << endl;
	fflush(stdin);
	cout << "Phi (currently ',phi:1, '): ";
	cin >> view_ref_point->phi;
	cout << endl;
	fflush(stdin);
	cout << "Viewing distance (currently ',view_plane_dist:1, '): ";
	cin >> view_ref_point->view_plane_dist;
	cout << "Scale factor: ";
	cin >> *factor;

	SetViewVariables(view_ref_point, view_transformation);
}

// Devuelve la V (la posicion de el ojo respecto a la posicion en el mundo de un vertice) y lo  normalizamos
Vector VectorOjo(Vector vertex, ViewPointRec view_ref_point)
{
	Vector V;

	V.x = view_ref_point.x_view - vertex.x;
	V.y = view_ref_point.y_view - vertex.y;
	V.z = view_ref_point.z_view - vertex.z;

	Normalize(V, &V);
	return V;
}
