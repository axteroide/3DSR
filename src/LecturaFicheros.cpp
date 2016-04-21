#include "Lecturaficheros.h"

#pragma region Copia de datos
void copyvector(Vector* copy, char* data, int num)
{
	char copia[260];
	int contador = num, contador2 = 0, contador3 = 0;

	int longi = strlen(data) + 1;
	while (longi > contador)
	{
		if (data[contador] == ' ' || data[contador] == '\0')
		{
			copia[contador2] = '\0';
			istringstream iss(copia);
			if (contador3 == 0)
				iss >> copy->x;
			else if (contador3 == 1)
				iss >> copy->y;
			else if (contador3 == 2)
				iss >> copy->z;
			contador2 = 0;
			contador3++;
		}

		copia[contador2] = data[contador];

		contador++;
		contador2++;
	}
}

#pragma endregion

#pragma region Recogida de datos
int getdata(ifstream *input, struct ValoresIluminacion *ilu, ViewPointRec *view_ref_point)
{
	string buffer;
	char line[260];
	char copy[260];
	Vector Veccopia;

	while (!input->eof())
	{
		&input->getline(line, 260, '\n');
		if (line[0] != '#' && line[0] != 0)
		{
			if (line[0] == 'I' && line[1] == 'a')
			{
				copyfun(copy, line, 3);
				ilu->Ia = atof(copy);
			}
			else if (line[0] == 'I' && line[1] == 'p' && line[2] == 'r')
			{
				copyfun(copy, line, 4);
				ilu->Ipr = atof(copy);
			}
			else if (line[0] == 'I' && line[1] == 'p' && line[2] == 'g')
			{
				copyfun(copy, line, 4);
				ilu->Ipg = atof(copy);
			}
			else if (line[0] == 'I' && line[1] == 'p' && line[2] == 'b')
			{
				copyfun(copy, line, 4);
				ilu->Ipb = atof(copy);
			}
			else if (line[0] == 'K' && line[1] == 'r')
			{
				copyfun(copy, line, 3);
				ilu->Kr = atof(copy);
			}
			else if (line[0] == 'K' && line[1] == 'g')
			{
				copyfun(copy, line, 3);
				ilu->Kg = atof(copy);
			}
			else if (line[0] == 'K' && line[1] == 'b')
			{
				copyfun(copy, line, 3);
				ilu->Kb = atof(copy);
			}
			else if (line[0] == 'K' && line[1] == 's')
			{
				copyfun(copy, line, 3);
				ilu->Ks = atof(copy);
			}
			else if (line[0] == 'n')
			{
				copyfun(copy, line, 2);
				ilu->n = atof(copy);
			}
			else if (line[0] == 'd' && line[1] == 'd')
			{
				copyfun(copy, line, 3);
				direccion = copy;
			}
			else if (line[0] == 'v' && line[1] == 'l')
			{
				copyvector(&Veccopia, line, 3);
				ilu->vl = Veccopia;
				cout << ilu->vl.x << " " << ilu->vl.y << " " << ilu->vl.z << " ";
			}
			else if (line[0] == 'r' && line[1] == 'h' && line[2] == 'o')
			{
				copyfun(copy, line, 4);
				view_ref_point->rho = atof(copy);
			}
			else if (line[0] == 't' && line[1] == 'h' && line[2] == 'e' && line[3] == 't' && line[4] == 'a')
			{
				copyfun(copy, line, 6);
				view_ref_point->theta = atof(copy);
			}
			else if (line[0] == 'p' && line[1] == 'h' && line[2] == 'i')
			{
				copyfun(copy, line, 4);
				view_ref_point->phi = atof(copy);
			}
			else if (line[0] == 'v' && line[1] == 'p' && line[2] == 'd')
			{
				copyfun(copy, line, 4);
				view_ref_point->view_plane_dist = atof(copy);
			}
		}
	}

	input->close();

	return 1;
}
#pragma endregion

#pragma region Nuevo fichero
int newfich(struct ValoresIluminacion *ilu, ViewPointRec *view_ref_point)
{
	ofstream fichero("datos.txt");
	fichero << "#Ia es la cantidad de luz ambiental reflejado por la superficie" << endl << endl;
	fichero << "Ia 0.0f" << endl << endl;
	fichero << "#Ip es la intensidad de la fuente luminosa puntual." << endl << endl;
	fichero << "Ipr 1.0f " << endl;
	fichero << "Ipg 1.0f" << endl;
	fichero << "Ipb 1.0f" << endl << endl;
	fichero << "#Kd es el coeficiente de reflexión difusa del material, el cual es una constante entre 0 y 1 y varía de un material a otro." << endl << endl;
	fichero << "Kr 0.2f" << endl;
	fichero << "Kg 0.4f" << endl;
	fichero << "Kb 0.6f" << endl << endl;
	fichero << "#Coeficiente de reflexión especular del material" << endl << endl;
	fichero << "Ks 1.0f" << endl << endl;
	fichero << "#Ie = Intensidad efectiva es proporcional a f(phi)=cos^n phi , n depende del tipo de material, " << endl;
	fichero << "#n creciente tiende hacia la reflexión perfecta, n suele usarse ene l intervalo[1,200]" << endl << endl;
	fichero << "n 100" << endl << endl;
	fichero << "#Vector de Luz (Hacia donde apunta la luz)" << endl << endl;
	fichero << "vl 1 1 1" << endl << endl;
	fichero << "#Distancia" << endl << endl;
	fichero << "rho 2000" << endl << endl;
	fichero << "#Angulo theta" << endl << endl;
	fichero << "theta 45" << endl << endl;
	fichero << "#Angulo phi" << endl << endl;
	fichero << "phi 45" << endl << endl;
	fichero << "#Distancia al plano" << endl << endl;
	fichero << "vpd 100" << endl << endl;
	fichero << "#Directorio donde se encuentran los modelos (es necesario poner por cada directorio \\\\ y al final del todo un *)" << endl << endl;
	fichero << "dd C:\\modelos\\*";
	fichero.close();

	ilu->Ia = 0.0f;
	ilu->Ipr = 1.0f;
	ilu->Ipg = 1.0f;
	ilu->Ipb = 1.0f;
	ilu->Kr = 0.2f;
	ilu->Kg = 0.4f;
	ilu->Kb = 0.6f;
	ilu->Ks = 1.0f;
	ilu->n = 100;
	ilu->vl.x = 1;
	ilu->vl.y = 1;
	ilu->vl.z = 1;
	view_ref_point->rho = 2000;
	view_ref_point->theta = 45;
	view_ref_point->phi = 45;
	view_ref_point->view_plane_dist = 100;


	direccion = "C:\\modelos\\*";
	return 1;
}
#pragma endregion
