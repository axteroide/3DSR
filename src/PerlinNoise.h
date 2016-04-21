#pragma once
#include "Colour.h"
#include "Vector.h"

#define XNOISE 100 //1024
#define YNOISE 100 //768
#define ZNOISE 100 //255
#define COLOR 3

double random();
DEV_COLOUR randomNoise();
double smoothNoise2D(Vector, int col);
double smoothNoise3D(Vector, int col);
DEV_COLOUR marble(Vector w, double size);
DEV_COLOUR wood(Vector w, double size);
void generateNoise();
double turbulence(Vector w, double size, int col);