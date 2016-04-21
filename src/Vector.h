
#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

struct Vector{
	float x, y, z;
};

double DotProduct(Vector v1, Vector v2);
void imprimeVector(Vector v, string s);
float Magnitude(Vector v);
int Normalize(Vector v1, Vector *v2);
Vector Reflexion(Vector v1, Vector v2);
float distancia(Vector v1, Vector v2);
void ReadAVector(Vector *v);