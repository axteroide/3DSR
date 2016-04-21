#pragma once

#include "Vector.h"

struct ValoresIluminacion{
	float Ia; //Ka es la cantidad de luz ambiental reflejado por la superficie
	float Ipr; //Ip es la intensidad de la fuente luminosa puntual rgb.
	float Ipg;
	float Ipb;
	float Kr; //Kd es el coeficiente de reflexi�n difusa del material, el cual es una constante entre 0 y 1 y var�a de un material a otro.
	float Kg;
	float Kb;
	float Ks;
	int n;
	Vector vl; //Vector de luz
};
