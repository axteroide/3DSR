#include "Matrix.h"
// Multiplica un vector v por una matriz t y lo guarda en un vector v2

int VectorMatrix(Vector v1, Matrix transformation, Vector *v2){
	v2->x = v1.x * transformation[0][0] +
		v1.y * transformation[1][0] +
		v1.z * transformation[2][0] +
		transformation[3][0];
	v2->y = v1.x * transformation[0][1] +
		v1.y * transformation[1][1] +
		v1.z * transformation[2][1] +
		transformation[3][1];
	v2->z = v1.x * transformation[0][2] +
		v1.y * transformation[1][2] +
		v1.z * transformation[2][2] +
		transformation[3][2];
	return 1;
}

///
// Pone los 1 en la matriz de identidad
///
void SetIdentityMatrix(Matrix m){
	int diag;
	for (diag = 0; diag <= 3; diag++)
		m[diag][diag] = 1;
}

// Funcion que imprime una matriz
void PrintMatrix(Matrix transformation){
	for (int i = 0; i<4; i++){
		for (int j = 0; j < 4; j++){
			cout << transformation[i][j] << "   ";
		}
		cout << endl;
	}
}


///
// Pone toda la matriz a 0
///
void SetZeroMatrix(Matrix m){
	m[0][0] = 0;
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = 0;
	m[1][0] = 0;
	m[1][1] = 0;
	m[1][2] = 0;
	m[1][3] = 0;
	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = 0;
	m[2][3] = 0;
	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 0;
}


///
// Multiplica dos matrices (m1 y m2) y las devuelve en m3.
///
#pragma region MultiplyMatrices
void MultiplyMatrices(Matrix m1, Matrix m2, Matrix m3){
	int i, j, k; //0..3
	Matrix m4;

	SetZeroMatrix(m4);

	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++)
			for (k = 0; k<4; k++)
				m4[i][j] = (m4[i][j]) + (m1[i][k] * m2[k][j]);


	for (i = 0; i<4; i++)
		for (j = 0; j<4; j++)
			m3[i][j] = m4[i][j];

}
#pragma endregion

// Funciona
///
// Obtiene la transformacion que se necesita para hacer una translacion segun el vector tv
// (Vector tv, Matrix tm)
///
#pragma region GetTranslationMatrix
void GetTranslationMatrix(Vector tv, Matrix tm){
	tm[3][0] = tv.x;
	tm[3][1] = tv.y;
	tm[3][2] = tv.z;
}
#pragma endregion 

///
// Obtiene la transformacion que se necesita para hacer una escala segun el vector tv
// (Vector tv, Matrix tm)
///
#pragma region GetScalingMatrix
void GetScalingMatrix(Vector sv, Matrix sm){
	//SetIdentityMatrix(sm);
	sm[0][0] = sv.x;
	sm[1][1] = sv.y;
	sm[2][2] = sv.z;
}
#pragma endregion

///
// Obtiene la transformacion que se necesita para hacer una escala segun el vector tv
// (Vector tv, Matrix tm)
///
#pragma region GetRotationMatrix
void GetRotationMatrix(Vector rv, Matrix rm){
	Matrix x_rot, y_rot, z_rot;
	SetZeroMatrix(x_rot);
	SetZeroMatrix(y_rot);
	SetZeroMatrix(z_rot);

	SetIdentityMatrix(x_rot);
	SetIdentityMatrix(y_rot);
	SetIdentityMatrix(z_rot);

	rv.x = InRadians(rv.x);
	rv.y = InRadians(rv.y);
	rv.z = InRadians(rv.z);

	if (rv.x != 0){
		x_rot[1][1] = cos(rv.x);
		x_rot[1][2] = sin(rv.x);
		x_rot[2][1] = -sin(rv.x);
		x_rot[2][2] = cos(rv.x);
	}
	if (rv.y != 0){
		y_rot[0][0] = cos(rv.y);
		y_rot[0][2] = -sin(rv.y);
		y_rot[2][1] = sin(rv.y);
		y_rot[2][2] = cos(rv.y);
	}
	if (rv.z != 0){
		z_rot[0][0] = cos(rv.z);
		z_rot[0][1] = sin(rv.z);
		z_rot[1][0] = -sin(rv.z);
		z_rot[1][1] = cos(rv.z);
	}
	MultiplyMatrices(z_rot, y_rot, rm);
	MultiplyMatrices(x_rot, rm, rm);
}
#pragma endregion
