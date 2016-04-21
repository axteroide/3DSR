#include "Utils.h"

//Funcion que retorna 1 si la cadena que se le pasa es numerica completamente 
int isNumber(const char *str, int length){
	char numbers[11] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '/' };
	bool isnumber;
	for (int i = 0; i < length; i++){
		isnumber = false;
		for (int j = 0; j < 11; j++)
			if (str[i] == numbers[j]){
				isnumber = true;
				break;
			}

		if (!isnumber)
			return 0;
	}
	return 1;
}
// Función que transforma de grados a radianes
double InRadians(float degrees){
	return degrees * PI / 180;
}

void copyfun(char* copy, char* data, int num)
{
	int contador = num, contador2 = 0;
	int longi = strlen(data);
	while (data[contador] != 'f' && longi > contador)
	{
		copy[contador2] = data[contador];
		contador++;
		contador2++;
	}
	copy[contador2] = '\0';
}

// Funcion que calcula la diferencia en milisegundos de dos tiempos
double diffclock(clock_t clock1, clock_t clock2){
	double diffticks = clock1 - clock2;
	double diffms = (diffticks) / CLOCKS_PER_SEC;
	return diffms;
}

