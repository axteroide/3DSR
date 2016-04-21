#include "Vector.h"
// Calculamos el producto a escalar de los vectores v1 y v2
double DotProduct(Vector v1, Vector v2){
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

// Devuelve el vector reflexion entre los vectores v1 y v2,
Vector Reflexion(Vector v1, Vector v2)
{
	Vector v3;
	float dot;

	dot = DotProduct(v1, v2);

	v3.x = ((2 * dot) * v1.x - v2.x);
	v3.y = ((2 * dot) * v1.y - v2.y);
	v3.z = ((2 * dot) * v1.z - v2.z);

	Normalize(v3, &v3);
	return v3;
}

// Devuelve la magnitud de un vector
float Magnitude(Vector v){
	return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

// Normaliza el vector v1 y lo guarda en v2
int Normalize(Vector v1, Vector *v2){
	float denominator;
	denominator = Magnitude(v1);
	if (denominator == 0){
		*v2 = v1;
	}
	else{
		v2->x = v1.x / denominator;
		v2->y = v1.y / denominator;
		v2->z = v1.z / denominator;
	}
	return 1;
}

// Imprime un vector por pantalla
void imprimeVector(Vector v, string s){
	cout << endl << s << " x: " << v.x << " y: " << v.y << " z: " << v.z << endl;
}

// Funcion que retorna la distancia entre dos vectores
float distancia(Vector v1, Vector v2){
	return sqrt(pow((v2.x - v1.x), 2) + pow((v2.y - v1.y), 2) + pow((v2.z - v1.z), 2));
}

///
// Pregunta la posicion de un vector y las guarda en v
///
#pragma region ReadAVector
void ReadAVector(Vector *v){
	cout << "Enter new vector" << endl;
	cout << "x : ";
	cin >> v->x;
	cout << "y : ";
	cin >> v->y;
	cout << "z : ";
	cin >> v->z;
}
