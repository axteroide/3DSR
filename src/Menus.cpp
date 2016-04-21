#include "Menus.h"


#pragma region List Directorys

bool fileordirec(char path[], char filename[]) //Directory exist
{

	WIN32_FIND_DATA findFileData2;
	HANDLE          hFind2;
	char dir[MAX_PATH];

	strcpy(dir, path);

	dir[strlen(dir) - 1] = '\0';
	strcat(dir, filename);
	strcat(dir, "\\*");

	hFind2 = FindFirstFile(dir, &findFileData2);

	if (hFind2 == INVALID_HANDLE_VALUE)
		return false;
	return true;
}

void search(char cadena[], char *cadena2) //search back directory
{
	int i = strlen(cadena) - 4;
	int o = strlen(cadena) - 1;
	int confir = 1;
	do{
		if (cadena[i] == '\\')
			confir = 0;
		i--;
	} while (confir == 1);
	for (i; i<o; i++)
		cadena2[i + 1] = '\0';

}



int enterInt() //enterInt validate that an is an integer
{
	string input;
	int retVal;
	bool done = false;
	do {
		cout << "Choose the option you want ('0' to exit) ";
		getline(cin, input);
		istringstream istr(input);
		if (istr >> retVal)
		{
			done = true;
		}
	} while (!done);
	return retVal;
}


string listdirectory() //List of the actual directory
{
	WIN32_FIND_DATA findFileData;
	HANDLE          hFind;

	char dir[MAX_PATH];
	strcpy(dir, direccion.c_str());
	int contador = 0, contfich = 0;
	int numero;
	int elegido = 1;
	CHAR FileName[MAX_PATH][MAX_PATH];

	do{
		hFind = FindFirstFile(dir, &findFileData);

		if (hFind == INVALID_HANDLE_VALUE)
			std::cout << "Incorrect path" << endl;
		else
		{
			system("cls");
			cout << "'Object name?'" << endl;
			// List of all the files
			while (FindNextFile(hFind, &findFileData) != 0)
			{
				contador++;
				strcpy(FileName[contador], findFileData.cFileName);
				std::cout << contador << " " << findFileData.cFileName << endl;
			}
		}
		do
		{
			numero = enterInt();
		} while (numero>contador || numero < 0);

		contador = 0;

		if (numero != 0)
		{
			if (strstr(FileName[numero], ".obj")) //Searh for a file .obj
			{
				dir[strlen(dir) - 1] = '\0';
				strcat(dir, FileName[numero]);
				elegido = 0;
			}
			else if (strstr(FileName[numero], "..")) //Go back
			{
				system("cls");
				search(dir, dir);
				strcat(dir, "\\*");
			}
			else if (fileordirec(dir, FileName[numero]) == false) //Look if we are going to a directory or a file
			{
				cout << "Ese no es un archivo .obj" << endl;
				fflush(stdin);
				getchar();
				system("cls");
			}
			else //Go to de next Directory
			{
				system("cls");
				dir[strlen(dir) - 1] = '\0';
				strcat(dir, FileName[numero]);
				strcat(dir, "\\*");
			}
		}
		else
			elegido = 0;
	} while (elegido == 1);


	string direcc = dir;

	return direcc;

}

#pragma endregion