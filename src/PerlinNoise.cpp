#include "PerlinNoise.h"

// Genera un numero random entre 0 y 1
double random(){
	return ((double)rand() / (RAND_MAX));
}

DEV_COLOUR randomNoise(){
	DEV_COLOUR w;
	w.r = random();
	w.g = random();
	w.b = random();
	return w;
}



double noise[XNOISE][YNOISE][ZNOISE][COLOR];
void generateNoise()
{

	int z = 0;

	for (int c = 0; c < COLOR; c++)
		for (z = 0; z < ZNOISE; z++)
			for (int x = 0; x < XNOISE; x++)
				for (int y = 0; y < YNOISE; y++)
				{
					noise[x][y][z][c] = (rand() % 32768) / 32768.0;
				}
}


double smoothNoise2D(Vector w, int col){
	double fractX = w.x - int(w.x);
	double fractY = w.y - int(w.y);
	double fractZ = w.z - int(w.z);

	//wrap around
	int x1 = (int(w.x) + XNOISE) % XNOISE;
	int y1 = (int(w.y) + YNOISE) % YNOISE;

	//neighbor values
	int x2 = (x1 + XNOISE - 1) % XNOISE;
	int y2 = (y1 + YNOISE - 1) % YNOISE;

	//smooth the noise with bilinear interpolation
	double value = 0.0;
	value += fractX       * fractY       * noise[x1][y1][0][col];
	value += fractX       * (1 - fractY) * noise[x1][y2][0][col];
	value += (1 - fractX) * fractY       * noise[x2][y1][0][col];
	value += (1 - fractX) * (1 - fractY) * noise[x2][y2][0][col];

	return value;
}
double smoothNoise3D(Vector w, int col){
	//get fractional part of x and y
	double fractX = w.x - int(w.x);
	double fractY = w.y - int(w.y);
	double fractZ = w.z - int(w.z);

	//wrap around
	int x1 = (int(w.x) + XNOISE) % XNOISE;
	int y1 = (int(w.y) + YNOISE) % YNOISE;
	int z1 = (int(w.z) + ZNOISE) % ZNOISE;

	//neighbor values
	int x2 = (x1 + XNOISE - 1) % XNOISE;
	int y2 = (y1 + YNOISE - 1) % YNOISE;
	int z2 = (z1 + ZNOISE - 1) % ZNOISE;

	//smooth the noise with bilinear interpolation
	double value = 0.0;
	value += fractX       * fractY       * fractZ         * noise[x1][y1][z1][col];
	value += fractX       * fractY       * (1 - fractZ)   * noise[x1][y1][z2][col];
	value += fractX       * (1 - fractY) * fractZ         * noise[x1][y2][z1][col];
	value += fractX       * (1 - fractY) * (1 - fractZ)   * noise[x1][y2][z2][col];
	value += (1 - fractX) * fractY       * fractZ         * noise[x2][y1][z1][col];
	value += (1 - fractX) * fractY       * (1 - fractZ)   * noise[x2][y1][z2][col];
	value += (1 - fractX) * (1 - fractY) * fractZ          * noise[x2][y2][z1][col];
	value += (1 - fractX) * (1 - fractY) * (1 - fractZ)    * noise[x2][y2][z2][col];

	return value;
}

double turbulence(Vector w, double size, int col){
	double value = 0.0, initialSize = size;
	Vector temp;

	while (size >= 1)
	{
		temp.x = w.x / size;
		temp.y = w.y / size;
		temp.z = w.z / size;
		//value += smoothNoise2D(temp) * size;
		value += smoothNoise3D(temp, col) * size;
		size /= 2.0;
	}

	return(value / initialSize);
}

DEV_COLOUR marble(Vector w, double size){
	DEV_COLOUR col;
	double xPeriod = 5.0; //defines repetition of marble lines in x direction
	double yPeriod = 10.0; //defines repetition of marble lines in y direction
	double zPeriod = 2.0;
	//turbPower = 0 ==> it becomes a normal sine pattern
	double turbPower = 5.0; //makes twists
	double turbSize = 32.0; //initial size of the turbulence


	double xyzValue = w.x * xPeriod / XNOISE + w.y * yPeriod / YNOISE + w.z * zPeriod / ZNOISE + turbPower * turbulence(w, size, 0);
	double sineValue = fabs(sin(xyzValue * 3.14159));
	col.r = col.g = col.b = sineValue;

	return col;
}

DEV_COLOUR wood(Vector w, double size){
	DEV_COLOUR c;
	double xyPeriod = 12.0; //number of rings
	double turbPower = 0.02; //makes twists
	double turbSize = 32.0; //initial size of the turbulence

	double xValue = (w.x - XNOISE / 2) / double(XNOISE);
	double yValue = (w.y - YNOISE / 2) / double(YNOISE);
	double zValue = (w.z - ZNOISE / 2) / double(ZNOISE);

	double distValue = sqrt(xValue * xValue + yValue * yValue + zValue * zValue) + turbPower * turbulence(w, size, 0);
	double sineValue = 0.5 * fabs(sin(2 * xyPeriod * distValue * 3.14159));
	c.r = 0.3125 + sineValue;
	c.g = 0.1171 + sineValue;
	c.b = 0.1171;

	return c;
}