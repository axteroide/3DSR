#include "Iluminacion.h"



#pragma region Globales
int pixels;
extern int size;
bool iluminacion = true;
bool perlin = true;
#pragma endregion


#pragma region Dibujado
// Dibuja un punto en las coordenadas de pantalla x e y con el color c
void DEV_DRAW_PIXEL(float x, float y, DEV_COLOUR c){
	glColor3f(c.r, c.g, c.b);
	
      glVertex2f(x - 1, y - 1);
      glVertex2f(x, y);
   
}

// Dibuja una linea entre las coordenadas de x e y a x2 e y2 con el color colour
void DEV_DRAW_TO(float x, float y, float x2, float y2, DEV_COLOUR colour){
	glColor3f(colour.r, colour.g, colour.b);
	glBegin(GL_LINES);
		glVertex2f(x, y);
		glVertex2f(x2, y2);
	glEnd();

}

// Limpia el buffer
void SwapBuffers(){
   	glutSwapBuffers();
}

// Limpia la pantalla
void DEV_CLEAR_SCREEN(){	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Dibuja las lineas de los ejes que se le pasan ya con las coordenadas de la pantalla en un vector
// Siendo las posiciones del vector 0,1 y 2 el punto del vector x,y,z y la posicion 4 el punto de origen
void dibujaEjes(ScreenRec ejes[5]){
	DEV_COLOUR coloreje;
	for(int i = 0; i < 3 ; i++){
		switch(i){
			case 0:
				coloreje.r = 1;
				coloreje.g = 0;
				coloreje.b = 0;
			break;
			case 1:
				coloreje.r = 0;
				coloreje.g = 1;
				coloreje.b = 0;
			break;
			case 2:
				coloreje.r = 0;
				coloreje.g = 0;
				coloreje.b = 1;
			break;
			case 3:
				coloreje.r = 0;
				coloreje.g = 0;
				coloreje.b = 0;
			break;
		}
		DEV_DRAW_TO(ejes[4].x,ejes[4].y,ejes[i].x,ejes[i].y, coloreje);	
	}
}

#pragma endregion

// Calcula el modelo de iluminacion para un pixel y lo dibuja en pantalla
void RenderPixel(int x,int y, float i, float r, Vector w, struct ValoresIluminacion ilu){ //i es (N*L) y r es (R*V))
	DEV_COLOUR c, wo;

	//wo = wood(w, pow(2, size));
	//wo = marble(w, pow(2, size));
	//wo = turbulence(w, pow(2, size));
	wo.r = wo.g = wo.b = turbulence(w, pow(2, size), 0);
	//wo.g = turbulence(w, pow(2, size), 1);
	//wo.b = turbulence(w, pow(2, size), 2);
	if (perlin){
		if (iluminacion == false){
			c.r = wo.r * ilu.Ia + ilu.Ipr * ((wo.r * i) + (ilu.Ks*r));
			c.g = wo.g * ilu.Ia + ilu.Ipg * ((wo.g * i) + (ilu.Ks*r));
			c.b = wo.b * ilu.Ia + ilu.Ipb * ((wo.b * i) + (ilu.Ks*r));
		}
		else
			c = wo;
	}
	else{
		c.r = ilu.Kr * ilu.Ia + ilu.Ipr * ((ilu.Kr * i) + (ilu.Ks*r));
		c.g = ilu.Kg * ilu.Ia + ilu.Ipg * ((ilu.Kg * i) + (ilu.Ks*r));
		c.b = ilu.Kb * ilu.Ia + ilu.Ipb * ((ilu.Kb * i) + (ilu.Ks*r));
	}
	DEV_DRAW_PIXEL(x,y,c);	
}

// Interpola las x de un poligono para una "y" dada y las renderiza
void RenderSpan (int y, EdgeBoxPtr * edge_box1, EdgeBoxPtr *edge_box2, int **z_buffer, struct ValoresIluminacion ilu,  ViewPointRec view_ref_point, bool threeD[]){
	EdgeBox temp_edge_box;
	int x1, x2, dx, x, z;
	float curr_z,  dz;
	Vector curr_w, dw, curr_n, dn;
	int NEWRESMAX =  DEV_MAX_X_RES, NEWRESMIN = 0;

	if(threeD[0] == true){
		if(threeD[1]){			
			NEWRESMAX = DEV_MAX_X_RES;
			NEWRESMIN = (DEV_MAX_X_RES / 2) + 1;
		}else
			NEWRESMAX = (DEV_MAX_X_RES / 2) - 1;
	}

	// Si la x de edge_box 1 es mayor les damos la vuelta para ir de mayor a menor
	if( (*edge_box1)->x > (*edge_box2)->x ){
		temp_edge_box = (*(*edge_box1));

		(*edge_box1)->x = (*edge_box2)->x;
		(*edge_box1)->n = (*edge_box2)->n;
		(*edge_box1)->r = (*edge_box2)->r;
		(*edge_box1)->z = (*edge_box2)->z;
		(*edge_box1)->w = (*edge_box2)->w;

		(*(*edge_box2)).x = temp_edge_box.x;
		(*(*edge_box2)).n = temp_edge_box.n;
		(*(*edge_box2)).r = temp_edge_box.r;
		(*(*edge_box2)).z = temp_edge_box.z;
		(*(*edge_box2)).w = temp_edge_box.w;
		(*(*edge_box2)).next = temp_edge_box.next;
	}
	x1 =  round((*edge_box1)->x);
	x2 =  round((*edge_box2)->x);

	if(x1 != x2){
		// Interpolamos todos los valores de la y de inicio
		dx = x2 - x1;
		curr_z = (*edge_box1)->z;
		
		curr_n = (*edge_box1)->n;
		//curr_r = (*edge_box1)->r;
		curr_w = (*edge_box1)->w;		

		// Calculamos el peso con respecto a la y final
		dz = ((*edge_box2)->z - curr_z)/dx;
//		di = ((*edge_box2)->n - curr_n)/dx;
		//dr = ((*edge_box2)->r - curr_r)/dx;

		dn.x = ((*edge_box2)->n.x - curr_n.x) / dx;
		dn.y = ((*edge_box2)->n.y - curr_n.y) / dx;
		dn.z = ((*edge_box2)->n.z - curr_n.z) / dx;	

		dw.x = ((*edge_box2)->w.x - curr_w.x) / dx;
		dw.y = ((*edge_box2)->w.y - curr_w.y) / dx;
		dw.z = ((*edge_box2)->w.z - curr_w.z) / dx;	
		// Empezamos a rasterizar, si la z es menor se dibujara, de lo contrario no
		for(x = x1; x <= x2 - 1; x++){
			z = round(curr_z);
			if((x >= NEWRESMIN && y >= 0) && (x < NEWRESMAX && y < DEV_MAX_Y_RES))
				if( z < z_buffer[x][y]){
					pixels++;
					z_buffer[x][y] = z;
					float i = DotProduct(curr_n, ilu.vl);
					if( i < 0 ) i = 0;
					Vector reflex;
					reflex = Reflexion(curr_n, ilu.vl);
					//curr_r = DotProduct(reflex, View1);
					float vr = DotProduct(reflex, VectorOjo(curr_w, view_ref_point));
					if(vr < 0) vr = 0;
					vr = pow(vr, ilu.n);
					RenderPixel(x, y, i, vr, curr_w, ilu);
				}

			curr_z = curr_z + dz;
			//curr_n = curr_n + dn;
//			curr_r = curr_r + dr;

			curr_n.x = curr_n.x + dn.x;
			curr_n.y = curr_n.y + dn.y;
			curr_n.z = curr_n.z + dn.z;

			curr_w.x = curr_w.x + dw.x;
			curr_w.y = curr_w.y + dw.y;
			curr_w.z = curr_w.z + dw.z;
		}
	}
}

// Interpola las aristas de un poligono y llama a RenderSpan para que este las rasterice
void AddEdgeToList (VertexPtr vertex1,VertexPtr vertex2, EdgeBuffer edge_list_at){
	VertexPtr temp_vertex;
	float curr_x, curr_z,  dx, dz;
	Vector curr_w, dw, curr_n, dn;
	int  y1, y2,  dy, y;
	
	EdgeBoxPtr edge_box;
	
	// Si la posicion y del vertice 1 es mas grande que la del 2 les damos la vuelta para mas tarde ir de menor a mayor
	if(vertex1->screen_pos.y > vertex2->screen_pos.y){
		temp_vertex = vertex1;
		vertex1 = vertex2;
		vertex2 = temp_vertex;
		/*view_temp = View1;
		View1 = View2;
		View2 = view_temp;*/
	}

	y1 = vertex1->screen_pos.y;
	y2 = vertex2->screen_pos.y;
	
	if( y1 != y2){
		dy = y2 - y1;
		// Calculamos todo el modelo y posicion del vertice 1 y las guardamos en "current"
		curr_x = vertex1->screen_pos.x;
		curr_z = vertex1->screen_pos.z;
		//curr_i = DotProduct(vertex1->vertex_normal, light_vector);
		curr_n = vertex1->vertex_normal;
		//if(curr_i < 0) curr_i = 0;
		//reflex = Reflexion(vertex1->vertex_normal, light_vector);
		//curr_r = DotProduct(reflex, View1);

		//if(curr_r < 0) curr_r = 0;
		//curr_r = pow(curr_r, n);
		curr_w = vertex1->world_pos;

		// Calculamos los pesos con los valores de el vertice2
		dx = (vertex2->screen_pos.x - curr_x) / dy;
		dz = (vertex2->screen_pos.z - curr_z) / dy;		
		//dn2 = DotProduct(vertex2->vertex_normal, light_vector);
		//if(di2 < 0) di2 = 0;		
		//di = (di2 - curr_i) / dy;
		/*dr = DotProduct(Reflexion(vertex2->vertex_normal, light_vector), View2);
		if(dr < 0) dr = 0; 
		dr = (pow(dr, n) - curr_r) / dy;*/

		dn.x = (vertex2->vertex_normal.x - curr_n.x) / dy;
		dn.y = (vertex2->vertex_normal.y - curr_n.y) / dy;
		dn.z = (vertex2->vertex_normal.z - curr_n.z) / dy;


		dw.x = (vertex2->world_pos.x - curr_w.x) / dy;
		dw.y = (vertex2->world_pos.y - curr_w.y) / dy;
		dw.z = (vertex2->world_pos.z - curr_w.z) / dy;

		
		for( y  = y1; y <= y2 - 1 ; y++){
			if(y > 0 && y < DEV_MAX_Y_RES){				
				// Creamos una EdgeBox en la que guardaremos todos los valores para cada pixel de una arista
				// Una vez termine con el poligono la estructura acaba siendo de un vector de el tamaño de la pantalla
				// que para cada y guarda los valores de ese punto x e y y apunta al punto x e y de la siguiente arista
				// con lo que despues rasterizaremos todas las x
				edge_box = new EdgeBox;			
				edge_box->x = curr_x;
				float intpart;
				modf(curr_z, &intpart);
				edge_box->z = intpart;
				edge_box->n = curr_n;
				//edge_box->r = curr_r;
				edge_box->w = curr_w;
				edge_box->next = edge_list_at[y];
				edge_list_at[y] = edge_box;
			}
			curr_x = curr_x + dx;
			curr_z = curr_z + dz;
			//curr_i = curr_i + di;
			//curr_r = curr_r + dr;

			curr_n.x = curr_n.x + dn.x;
			curr_n.y = curr_n.y + dn.y;
			curr_n.z = curr_n.z + dn.z;

			curr_w.x = curr_w.x + dw.x;
			curr_w.y = curr_w.y + dw.y;
			curr_w.z = curr_w.z + dw.z;
		}		
	}
}


// Empezamos a renderizar un poligono
void RenderPolygon(PolygonPtr current_polygon, EdgeBuffer edge_list_at, Vector light_vector, int **z_buffer, ViewPointRec view_ref_point, struct ValoresIluminacion ilu, bool threeD[]){
	int y;
	VertexListPtr vertex0, vertex1, vertex2;
	//Vector View1, View2;
	for(int y = 0 ;  y < DEV_MAX_Y_RES ; y++)
		edge_list_at[y] = NULL;
	
	vertex1 = current_polygon->vertex_list_head;
	vertex0 = vertex1;
	vertex2 = vertex1->rest;

	//View1 = VectorOjo(vertex1->vertex, view_ref_point);
	//View2 = VectorOjo(vertex2->vertex, view_ref_point);
	// Pasamos por todas las aristas menos por la ultima
	do{	
		AddEdgeToList(vertex1->vertex, vertex2->vertex, edge_list_at);
		vertex1 = vertex2;
		vertex2 = vertex2->rest;
		//View1 = View2;
		/*if( vertex2 != NULL ){
			View2 = VectorOjo(vertex2->vertex, view_ref_point);
		}*/
	}while(vertex2 != NULL);
	// Pasamos ahora por la ultima arista
	//View2 = VectorOjo(vertex0->vertex, view_ref_point);
	AddEdgeToList(vertex0->vertex, vertex1->vertex, edge_list_at);	
	// Aqui empezamos a rasterizar el poligono
	for(y = 0 ;  y < DEV_MAX_Y_RES; y++)
		if(edge_list_at[y] != NULL){
			RenderSpan(y , &edge_list_at[y], &edge_list_at[y]->next, z_buffer, ilu, view_ref_point, threeD);
		}	
}

// Renderizamos todos los objetos
void RenderObject(ObjectPtr current_object, EdgeBuffer edge_list_at, Vector light_vector, int **z_buffer, ViewPointRec view_ref_point, struct ValoresIluminacion ilu, bool threeD[]){
	SurfacePtr current_surface;
	PolygonPtr current_polygon;

	current_surface = current_object->surface_head;

	while(current_surface != NULL){
		current_polygon = current_surface->polygon_head;
		while(current_polygon != NULL){
			if(current_polygon->poly_visible){				
				RenderPolygon(current_polygon, edge_list_at, light_vector, z_buffer, view_ref_point, ilu, threeD);	
				for(int y = 0 ;  y < DEV_MAX_Y_RES ; y++)
					if(edge_list_at[y] != NULL){
						delete[] edge_list_at[y]->next;
						delete[] edge_list_at[y];
					}

			}
			current_polygon = current_polygon->next;
		}
		current_surface = current_surface->next;
	}
}


// Renderizamos la escena
void RenderScene(ObjectPtr object_head, ViewPointRec view_ref_point, Matrix view_transformation, struct ValoresIluminacion ilu, bool threeD[]){
	int **z_buffer_at;
	EdgeBuffer edge_list_at;
	Vector light_vector;
	ObjectPtr current_object;	
	z_buffer_at = new int *[DEV_MAX_X_RES];
	for(int i = 0 ; i < DEV_MAX_X_RES ; i++)
		z_buffer_at[i] = new int [DEV_MAX_Y_RES];
	

	srand(time(NULL));
	InitializeZBuffer(z_buffer_at);

	// Inicializamos la direccion de la luz y la normalizamos
	light_vector.x = ilu.vl.x;
	light_vector.y = ilu.vl.y;
	light_vector.z = ilu.vl.z;
	//cout << light_vector.x << " " << light_vector.y << " " << light_vector.z << " ";

	Normalize(light_vector, &light_vector);
	pixels = 0;

	current_object = object_head;
	glBegin(GL_LINES);
	while(current_object != NULL){
		RenderObject(current_object,edge_list_at, light_vector, z_buffer_at, view_ref_point, ilu,threeD);
		current_object = current_object->next;
	}
	glEnd();
	imprimeIlu(ilu);

	for(int i = 0 ; i < DEV_MAX_X_RES ; i++)
		delete[] z_buffer_at[i];	
	delete[] z_buffer_at;	
}

// Funciones para imprimir textos en la ventana de Glut
#pragma region TextFunctions
void renderString(void *font, string s) {
	
	glColor3f(0, 0, 0);
	for (string::iterator i = s.begin(); i != s.end(); ++i){
		char c = *i;
		glutBitmapCharacter(font, c);
	}
  
}

void imprimeInt(string cad, int num){
	glColor3f(0, 0, 0);
	renderString(GLUT_BITMAP_8_BY_13, cad);	
	stringstream out8;
	out8 << num;
	renderString(GLUT_BITMAP_8_BY_13, out8.str());
}

void imprimeFloat(string cad, float num){
	glColor3f(0, 0, 0);
	renderString(GLUT_BITMAP_8_BY_13, cad);	
	stringstream out8;
	out8 << num;
	renderString(GLUT_BITMAP_8_BY_13, out8.str());
}

void imprimeIlu(struct ValoresIluminacion ilu){
	glColor3f(0, 0, 0);
	glRasterPos2f(5, DEV_MAX_Y_RES - 10);
	imprimeFloat(" Ia= ", ilu.Ia);
	glRasterPos2f(5, DEV_MAX_Y_RES - 25);
	imprimeFloat(" Ipr:", ilu.Ipr);
	imprimeFloat(" Ipg:", ilu.Ipg);
	imprimeFloat(" Ipb:", ilu.Ipb);
	glRasterPos2f(5, DEV_MAX_Y_RES - 40);
	imprimeFloat(" Kr:", ilu.Kr);
	imprimeFloat(" Kg:", ilu.Kg);
	imprimeFloat(" Kb:", ilu.Kb);
	glRasterPos2f(5, DEV_MAX_Y_RES - 55);
	imprimeFloat("Especular Ks:", ilu.Ks);
	imprimeFloat(" n:", ilu.n);
}

#pragma endregion

