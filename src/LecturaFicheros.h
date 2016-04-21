#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include "Vector.h"
#include "ScreenCam.h"
#include "StrucIluminacion.h"

using namespace std;

extern string direccion;


void copyvector(Vector* copy, char* data, int num);
int getdata(ifstream *input, struct ValoresIluminacion *ilu, ViewPointRec *view_ref_point);
int newfich(struct ValoresIluminacion *ilu, ViewPointRec *view_ref_point);