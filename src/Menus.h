
#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>
#include <conio.h>
#include <time.h>
#include <windows.h>

using namespace std;

extern string direccion;


bool fileordirec(char path[], char filename[]);
void search(char cadena[], char *cadena2);
int enterInt();
string listdirectory();
