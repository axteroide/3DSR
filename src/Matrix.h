#pragma once
#include "Vector.h"
#include "Utils.h"
typedef float Matrix[4][4];


int VectorMatrix(Vector v1, Matrix transformation, Vector *v2);
void SetIdentityMatrix(Matrix m);
void PrintMatrix(Matrix transformation);
void SetZeroMatrix(Matrix m); 
void GetTranslationMatrix(Vector tv, Matrix tm);
void GetScalingMatrix(Vector sv, Matrix sm);
void GetRotationMatrix(Vector rv, Matrix rm);
void GetTransformation(string name, Matrix *transformation);