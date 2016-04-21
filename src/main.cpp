#include "main.h"


int width = DEV_MAX_Y_RES, height = DEV_MAX_X_RES, typerender=1;
bool draw_vertex_normals = false, calculate_vertex_normals = false, calculate_polygon_normals = false;
string direccion;
double timerender = 0;
bool activemousepas = false, threeD[2] = {false, false};


int size = 1;


#pragma region PrintMatrix, Normalize, InRadians, Magnitude, DotProduct, VectorMatrix
// Funcion que imprime las estadisticas de un objeto
// el nombre, numero de poligonos, que poligonos son visibles, normales, etc.
void PrintObjectStats(ObjectCell object_head){
	ObjectPtr current_object;
	SurfacePtr current_surface;
	PolygonPtr current_polygon;
	VertexListPtr current_vertex;
	
	current_object = &object_head;
	while(current_object != NULL){
		cout << "Object: " << current_object->name << endl;;
		PrintMatrix(current_object->transformation);
		current_surface = current_object->surface_head;
		while(current_surface != NULL){
			cout << endl << "SURFACE Polygons: " << current_surface->no_polygons << endl;
			current_polygon = current_surface->polygon_head;
			while(current_polygon != NULL){
				cout << "POLYGON visible:" << current_polygon->poly_visible << " Polygon Normal: " << 
					endl << "x: " << current_polygon->poly_normal.x << " y: " << current_polygon->poly_normal.y << " z: " << current_polygon->poly_normal.z << endl;
				getchar();
				current_vertex = current_polygon->vertex_list_head;
				while(current_vertex != NULL){
					//cout << "VERTEX " << "Local: x:" << current_vertex->vertex->local_pos.x << " y:" << current_vertex->vertex->local_pos.y << " z:" << current_vertex->vertex->local_pos.z << endl;
					//cout << "World: x:" << current_vertex->vertex->world_pos.x << " y:" << current_vertex->vertex->world_pos.y << " z:" << current_vertex->vertex->world_pos.z << endl;
					//cout << "Eye: x:" << current_vertex->vertex->eye_pos.x << " y:" << current_vertex->vertex->eye_pos.y << " z:" << current_vertex->vertex->eye_pos.z << endl << endl;
					imprimeVector(current_vertex->vertex->vertex_normal, "Vertex normal");
					current_vertex = current_vertex->rest;					
				}
				current_polygon = current_polygon->next;
			}
			current_surface = current_surface->next;
		}
		current_object = current_object->next;
	}

}
#pragma endregion

#pragma region Escena

// Pasamos un vector posicion por la matriz de proyección
// pos: vector a transformar
// view_ref_point: donde guardamos todas las variables de la camara
// map_offsets: donde se guarda la resolucion de la pantalla para dibujar los elementos en el centro de la ventana
// scrreen_ : la posicion final en la que quedara el vertice en coordenadas de la pantalla
// factor: factor por el que multiplicamos los objetos para que queden mas grandes
void Perspective(Vector pos, ViewPointRec view_ref_point, 
				MapRec map_offsets, int *screen_x, int *screen_y, int *screen_z, int factor){
	float temp_x, temp_y, temp_z;
	float intpart;
	
	temp_x = view_ref_point.view_plane_dist * (pos.x/(pos.z));
	temp_y = view_ref_point.view_plane_dist * (pos.y/(pos.z));
	temp_z = view_ref_point.view_plane_dist * (pos.z/(pos.z));

	if(threeD[0]){
		if(threeD[1])
			modf ((map_offsets.xmid + (map_offsets.xmid /2) ) + (temp_x * factor) , &intpart);			
		else
			modf ((map_offsets.xmid /2 ) + (temp_x * factor) , &intpart);
	}else{
		modf (map_offsets.xmid  + (temp_x * factor) , &intpart);
	}
	*screen_x = ((int) intpart);
	modf (map_offsets.ymid  + (temp_y * factor), &intpart);
	*screen_y = ((int) intpart);
	modf (temp_z, &intpart);
	*screen_z = ((int) intpart);
}

// Se encarga de dibujar todo lo que se ha de dibujar.
// pregunta si se quieren dibujar las normales del vertice, y en caso afirmativo las dibuja en negro
#pragma region WireFrameScene
void WireFrameScene(MapRec map_offsets, Matrix view_transformation, 
					ViewPointRec view_ref_point, ObjectPtr object_head, ScreenRec ejes[],int factor){
	int const NSF = 2;

	DEV_COLOUR wire_colour;
	DEV_COLOUR normal_colour;
	SurfacePtr current_surface;
	PolygonPtr current_polygon;
	VertexListPtr vertex_list;
	ObjectPtr current_object;
	ScreenRec start_pos;
	Vector tw,ts;
	int tx,ty;

	wire_colour.r = 1;
	wire_colour.g = 0;
	wire_colour.b = 1;
	normal_colour.r = 0;
	normal_colour.g = 0;
	normal_colour.b = 0;	
   
   current_object = object_head;
	while (current_object != NULL){
		if(current_object->selected){
			wire_colour.r = 1;			
			wire_colour.g = 1;
			wire_colour.b = 0;
		}else{
			wire_colour.r = 1;
			wire_colour.g = 0;
			wire_colour.b = 1;
		}
		current_surface= current_object->surface_head;
		while(current_surface != NULL){
			current_polygon=current_surface->polygon_head;
			while(current_polygon != NULL){
				if (current_polygon->poly_visible){

					vertex_list = current_polygon->vertex_list_head;
					start_pos = vertex_list->vertex->screen_pos;
					Vector vec = vertex_list->vertex->eye_pos;
					vertex_list = vertex_list->rest;					
					ScreenRec uno;
					uno = start_pos;
					while(vertex_list != NULL)
					{
						
						DEV_DRAW_TO(uno.x, uno.y, vertex_list->vertex->screen_pos.x,vertex_list->vertex->screen_pos.y,wire_colour);
						uno = vertex_list->vertex->screen_pos;						
						vertex_list = vertex_list->rest;
					}
					// Tercer vertice					
					DEV_DRAW_TO(start_pos.x , start_pos.y, uno.x, uno.y,wire_colour);


					if (draw_vertex_normals)
					{
						vertex_list = current_polygon->vertex_list_head;
						while (vertex_list != NULL)
						{
							int z;

							tw.x = vertex_list->vertex->world_pos.x + (NSF * vertex_list->vertex->vertex_normal.x);
							tw.y = vertex_list->vertex->world_pos.y + (NSF * vertex_list->vertex->vertex_normal.y);
							tw.z = vertex_list->vertex->world_pos.z + (NSF * vertex_list->vertex->vertex_normal.z);		
							VectorMatrix(tw,view_transformation,&ts);
							Perspective(ts,view_ref_point,map_offsets,&tx,&ty, &z, factor);

							DEV_DRAW_TO(vertex_list->vertex->screen_pos.x, vertex_list->vertex->screen_pos.y, tx,ty,normal_colour);
							vertex_list=vertex_list->rest;
						}						
					}					
				}
				current_polygon=current_polygon->next;
			}
			current_surface = current_surface->next;
		}
		current_object = current_object->next;
	}
	imprimeStats();
	glFlush();
}

#pragma endregion

#pragma endregion

#pragma region Calculate...

// Calculamos las coordenadas en la pantalla para cada objeto en memoria
void CalculateScreenCoordinates(ObjectPtr current_object, ZParamRec z_params,MapRec map_offsets,ViewPointRec view_ref_point,int factor){
	SurfacePtr current_surface = NULL;
	PolygonPtr current_polygon = NULL;
	VertexPtr current_vertex = NULL;
//	Vector one, two;

	while (current_object != NULL){		
		bool ftime = true;
		current_vertex = current_object->vertex_head;
		
		while(current_vertex != NULL){
			Perspective(current_vertex->eye_pos, 
									view_ref_point, map_offsets ,
									&current_vertex->screen_pos.x, 
									&current_vertex->screen_pos.y,&current_vertex->screen_pos.z , factor);
			
			double toint;
			
			modf(((current_vertex->eye_pos.z - z_params.z_min) / z_params.z_range) * DEV_MAX_Z_RES , &toint);
			current_vertex->screen_pos.z = (int) toint;

			/*if(ftime){
				if((current_vertex->screen_pos.x > 0 && current_vertex->screen_pos.x < DEV_MAX_X_RES) || (current_vertex->screen_pos.y > 0 && current_vertex->screen_pos.y < DEV_MAX_Y_RES)){
					ftime = false;
					one.x = current_vertex->screen_pos.x;
					one.y = current_vertex->screen_pos.y;
					two.x = current_vertex->screen_pos.x;
					two.y = current_vertex->screen_pos.y;
				}
			}else{
				if((current_vertex->screen_pos.x > 0 && current_vertex->screen_pos.x < DEV_MAX_X_RES) && (current_vertex->screen_pos.y > 0 && current_vertex->screen_pos.y < DEV_MAX_Y_RES)){
					if(one.x > current_vertex->screen_pos.x) one.x = current_vertex->screen_pos.x;
					if(one.y > current_vertex->screen_pos.y) one.y = current_vertex->screen_pos.y;
					if(two.x < current_vertex->screen_pos.x) two.x = current_vertex->screen_pos.x;
					if(two.y = current_vertex->screen_pos.y) two.y = current_vertex->screen_pos.y;
				}
			}*/
			current_vertex = current_vertex->next;
		}/*
		int ox, oy, tx, ty;
		if(one.x < two.x){ 
			ox = one.x; 
			tx = two.x;
		}
		else{
			ox = two.x;
			tx = one.x;
		}
		if(one.y < two.y){ 
			oy = one.y; 
			ty = two.y;
		}
		else{
			oy = two.y;
			ty = one.y;
		}

		for(int i = ox ; i <= tx ; i++)
			for(int j = oy ; j <= ty ; j++)
				buffer[i][j] = current_object->id_no;
		*/
		current_object = current_object->next;
	}
	
}

// Calcula la posicion de la camara para cada vertice
void CalculateEyeCoordinates(ObjectPtr current_object, ZParamRec *z_pararms,Matrix view_transformation)
{
	VertexPtr current_vertex;
	current_vertex = current_object->vertex_head; 
	while (current_vertex != NULL)
	{		
		VectorMatrix(current_vertex->world_pos,view_transformation,&current_vertex->eye_pos);
		if (current_vertex->eye_pos.z < z_pararms->z_min)
			z_pararms->z_min = current_vertex->eye_pos.z;
		if (current_vertex->eye_pos.z > z_pararms->z_max)
			z_pararms->z_max = current_vertex->eye_pos.z;
		current_vertex = current_vertex->next;		
	}
}

#pragma endregion

#pragma region Hidden Polygons

// Si la normal del poligono es menor que 0 ponemos el poligono como no visible
void RemovePolygonIfHidden(PolygonPtr current_polygon, ViewPointRec view_ref_point){
	Vector view_direction;
	while(current_polygon != NULL){

		// Calculamos el vector direccion de la camara con respecto al vertice y lo normalizamos
		view_direction.x = view_ref_point.x_view - current_polygon->vertex_list_head->vertex->world_pos.x;
		view_direction.y = view_ref_point.y_view - current_polygon->vertex_list_head->vertex->world_pos.y;
		view_direction.z = view_ref_point.z_view - current_polygon->vertex_list_head->vertex->world_pos.z;
		Normalize(view_direction, &view_direction);
		// Si el producto a escalar es positivo ponemos a True poly_visible
		
		current_polygon->poly_visible = DotProduct(current_polygon->poly_normal, view_direction) > 0;		
		current_polygon = current_polygon->next;
	}
}

// Pasamos todos los poligonos por RemovePolygonIfHidden, para ocultar las caras no visibles
void RemoveHiddenSurfaces(ObjectPtr current_object,ViewPointRec view_ref_point){
	SurfacePtr current_surface; 
	current_surface = current_object->surface_head;
	while (current_surface != NULL)
	{
		RemovePolygonIfHidden(current_surface->polygon_head, view_ref_point);
		current_surface = current_surface->next;
	}
}

#pragma endregion

#pragma region Normales
// Calculamos la normal del poligono y la normalizamos
void CalculateAPolygonNormal(PolygonPtr current_polygon){
	VertexListPtr vertex_list;
	Vector w1, w2, w3;
	//float Qx, Qy, Qz, Px, Py, Pz;
	//	Vector p, q;



	vertex_list = current_polygon->vertex_list_head;
	w1 = vertex_list->vertex->world_pos;

	vertex_list = vertex_list->rest;
	w2 = vertex_list->vertex->world_pos;

	vertex_list = vertex_list->rest;
	w3 = vertex_list->vertex->world_pos;


	current_polygon->poly_normal.x = (w2.y - w1.y) * (w3.z - w2.z) - (w2.z - w1.z) * (w3.y - w2.y);
	current_polygon->poly_normal.y = (w2.z - w1.z) * (w3.x - w2.x) - (w2.x - w1.x) * (w3.z - w2.z);
	current_polygon->poly_normal.z = (w2.x - w1.x) * (w3.y - w2.y) - (w2.y - w1.y) * (w3.x - w2.x);

	Normalize(current_polygon->poly_normal, &current_polygon->poly_normal);
}

// Pasamos por todos los poligonos para calcular sus normales
void CalculatePolygonNormals(ObjectPtr current_object){
	SurfacePtr current_surface;
	PolygonPtr current_polygon;
	current_surface = current_object->surface_head;
	
	while (current_surface != NULL)
	{
		int poli = 0;
		current_polygon = current_surface->polygon_head;
		while (current_polygon != NULL && poli < current_surface->no_polygons)
		{
			CalculateAPolygonNormal(current_polygon);
			current_polygon = current_polygon->next;
			
			poli++;
		}
		current_surface = current_surface->next;
	}
}


// Calculamos las normales de los vertices promediadas
void CalculateAVertexNormal(VertexPtr current_vertex){
	PolygonListPtr poly_list;
	Vector sum_vector;
	int poly_count;

	sum_vector.x = 0;
	sum_vector.y = 0;
	sum_vector.z = 0;

	poly_count = 0;
	poly_list = current_vertex->poly_list_head;

	while(poly_list != NULL){
		poly_count++;
		sum_vector.x += poly_list->poly->poly_normal.x;
		sum_vector.y += poly_list->poly->poly_normal.y;
		sum_vector.z += poly_list->poly->poly_normal.z;
		poly_list = poly_list->rest;
		
	}
	current_vertex->vertex_normal.x = sum_vector.x /poly_count;
	current_vertex->vertex_normal.y = sum_vector.y /poly_count;
	current_vertex->vertex_normal.z = sum_vector.z /poly_count;	
	Normalize(current_vertex->vertex_normal, &current_vertex->vertex_normal);
}

// Pasamos por todos los vertices para calcular sus normales
void CalculateVertexNormals(ObjectPtr current_object){
	VertexPtr current_vertex;
	int contavert = 0;
	current_vertex = current_object->vertex_head;
	while(current_vertex != NULL && contavert < current_object->no_vertices){
		CalculateAVertexNormal(current_vertex);
		current_vertex = current_vertex->next;
	}
}

// Función que llama a las funciones para calcular las normales de los poligonos y de los vertices respectivamente
void CalculateNormals(ObjectPtr current_object){
	CalculatePolygonNormals(current_object);
	CalculateVertexNormals(current_object);
}

void doObjectsNormals(ObjectPtr *object_head){
	ObjectPtr current_object;
	current_object = (*object_head);
	while(current_object != NULL){
		CalculateNormals(current_object);
		current_object = current_object->next;
	}
}

void hideObjects(ObjectPtr *object_head,  ViewPointRec view_ref_point){
	ObjectPtr current_object;
	current_object = (*object_head);
	while(current_object != NULL){
		RemoveHiddenSurfaces(current_object, view_ref_point);
		current_object = current_object->next;
	}
}

#pragma endregion

#pragma region TransformTo...

/// Multiplica los vertices del objeto para transformarlos a las coordenadas de nuestro mundo
#pragma region TransformToWorldCoordinates
void TransformToWorldCoordinates(ObjectPtr *current_object){
	VertexPtr current_vertex;
	current_vertex = (*current_object)->vertex_head;
	while(current_vertex != NULL){
		VectorMatrix(current_vertex->local_pos, (*current_object)->transformation, &current_vertex->world_pos);	
		
		current_vertex = current_vertex->next;
	}
}
#pragma endregion

// Transforma la escena
// Primero calculamos la posicion en el mundo de los vertices del objeto
// despues calculamos las normales.
// Hacemos el culling de la escena y ocultamos los poligonos cuyas normales sean obtusas
// calculamos las coordenadas del objeto respecto al ojo
// Calculamos las coordenadas con respecto a la pantalla (los pixeles en los que estara el vertice)
#pragma region TransformScene
void TransformScene(MapRec map_offsets, Matrix view_transformation, ViewPointRec view_ref_point,ObjectPtr *object_head,ZParamRec *z_params,bool *scene_changed, int factor){
	float const HighReal= 1E30;
	ObjectPtr current_object;

	z_params->z_min = HighReal;
	z_params->z_max = -HighReal;
	z_params->z_range = 0;
	current_object = (*object_head);
	while(current_object != NULL){
		TransformToWorldCoordinates(&current_object);	
		//CalculateNormals(current_object);
		//RemoveHiddenSurfaces(current_object, view_ref_point);
		CalculateEyeCoordinates(current_object, z_params,view_transformation);
		
		current_object = current_object->next;
	}
	// Sacamos el rango de las Z que hay entre el vertice mas alejado y el mas cercano
	z_params->z_range = z_params->z_max- z_params->z_min;
	CalculateScreenCoordinates((*object_head), *z_params,map_offsets,view_ref_point, factor);
	scene_changed = false;
}
#pragma endregion
/***********************************************/
#pragma endregion




///
// Función que pregunta al usuario que transformacion quiere hacer y espera una de ellas, o ninguna
///
#pragma region GetTransformation
void GetTransformation (string name, Matrix transformation){
	char ch = 'Y';
	Vector trans_vector;
	Matrix trans_matrix;
	SetZeroMatrix(trans_matrix);
	SetIdentityMatrix(trans_matrix);
	while(ch == 'Y' || ch == 'y'){
		cout << endl << "Transformation of: " << name << endl;;
		cout << "(R - Rotation)" << endl;
		cout << "(S - Scaling)" << endl;
		cout << "(T - Translation)" << endl;
		cout << "(N - None)" << endl;
		while(ch != 'N' && ch != 'n' && ch != 'R' && ch != 'r' && ch != 'S' && ch != 's' && ch != 'T' && ch != 't'){
			cout << "Which transformation? ";
			fflush(stdin);
			cin >> ch;
		}
		if(ch != 'N' && ch != 'n'){
			SetIdentityMatrix(trans_matrix);
			switch(ch){
				case 'R':
				case 'r':
					cout << "Enter angles of rotation" << endl;
					cout << "Positive angle => anticlockwise rotation :";
					ReadAVector(&trans_vector);
					GetRotationMatrix(trans_vector, trans_matrix);
					break;
				case 'S':
				case 's':
					cout << "Enter scaling factors" << endl;
					ReadAVector(&trans_vector);
					GetScalingMatrix(trans_vector, trans_matrix);
					break;
				case 'T':
				case 't':
					cout << "Enter translation factors" << endl;
					ReadAVector(&trans_vector);
					GetTranslationMatrix(trans_vector, trans_matrix);
					break;
			}
			MultiplyMatrices(transformation, trans_matrix, transformation);
			calculate_polygon_normals = true;
			calculate_vertex_normals = true;
			cout << "Anyo more transformations for: " << name << " (y/n)?";
			cin >> ch;
		}
	}
}
#pragma endregion
#pragma endregion

#pragma region Initialize

// Función que inicializa los valores
void Initialize(MapRec *map_offsets, ViewPointRec *view_ref_point, Matrix *view_transformation, bool *scene_changed, int *no_of_objects, ObjectPtr *object_head, struct ValoresIluminacion *ilu){
	float intpart;
	ifstream input;
	modf ((DEV_MAX_X_RES + 1 ) / 2 , &intpart);
	map_offsets->xmid = (int) intpart;
	modf ((DEV_MAX_Y_RES + 1 ) / 2 , &intpart);
	map_offsets->ymid = (int) intpart;

	SetZeroMatrix(*view_transformation);
	SetIdentityMatrix(*view_transformation);
	SetViewVariables(view_ref_point, view_transformation);
	
	scene_changed = false;
	no_of_objects = 0;
	(*object_head) = NULL;

	input.open("datos.txt");
	if( !input.is_open() ) //Comprobacion de si el fichero existe, si no existe, se crea y se recogen los datos, si existe, simplemente se recogen
	{		
		newfich(ilu,view_ref_point);
	}
	else
	{
		getdata(&input,ilu,view_ref_point);
	}
}
#pragma endregion


#pragma region keyPressed


///
// Declaramos ciertas variables que necesitamos que sean globales debido a como esta estructurado Glut
///
MapRec map_offsets; // Variables de resolucion de pantalla
ViewPointRec view_ref_point; // Variables de posicion del ojo
Matrix view_transformation; // Transformacionde vista (Para convertir de coordenadas con respecto al mundo a coordenadas con respecto al ojo)
bool scene_changed = false, objects_added = false;
int no_of_objects = 0, inte, object_count = 0; //int se cambia por inte porque int es una palabra reservada
ObjectCell *object_head = NULL; // Estructura en la que guardaremos todos los objetos
ZParamRec z_params; // Parametros de la z
ScreenRec ejes[5]; // Vector de posiciones de la pantalla para los ejes
struct ValoresIluminacion ilu; // Estructura donde guardamos los valores de la iluminacion
int factor = 1; // Factor de escala
extern bool iluminacion;
extern bool perlin;
// Funcion de glut a la que se le pasa una tecla y un punto
// La usaremos como menu principal
void keyPressed(unsigned char key, int x, int y){
	string object_name;
	ObjectPtr current_object;
	switch (key){
		case 'O': // Carga de objeto
		case 'o':
				fflush(stdin);
				cout << endl;

				object_name = listdirectory();
				
				fflush(stdin);

				int obj;
				obj = LoadObject(object_name, no_of_objects, &object_head, &view_ref_point, &view_transformation, &factor);
				if(obj == 0)
					break;
				
				no_of_objects++;
				cout << "Object loaded successfull" << endl;
				fflush(stdin);
				cout << "Do you want transforming the object?(y/n) ";
				char ch;
				cin >> ch;
				SetZeroMatrix(object_head->transformation);
				SetIdentityMatrix(object_head->transformation);
				objects_added = true;
				if(ch == 'Y' || ch == 'y')	
					GetTransformation(object_head->name, (*object_head).transformation);
				scene_changed = true;
				if(object_head->vertexInFile){
					calculate_polygon_normals = true;
				}else{
					calculate_polygon_normals = true;
					calculate_vertex_normals = true;
				}
				//object_changed = true;
			break;

		case 'T': // Transformacion de objeto
		case 't':
				current_object = object_head;
				if( no_of_objects > 0){
					cout << endl;
					cout << "'Select object to transform'";
					for(object_count = no_of_objects; object_count >= 1; object_count--){
						cout << endl;						
						cout  << object_count << " - ";
						cout << current_object->name;
						current_object = current_object->next;
					}
					do{
						cout << endl;
						cout << "Your choice? ";
						cin >> inte;
					}while(inte > no_of_objects || inte < 0);
					current_object = object_head; 
					while (current_object->id_no != (inte - 1)){
						current_object = current_object->next;
					}
					GetTransformation(current_object->name, current_object->transformation);					
					scene_changed = true;
				}
				else
				{
						cout << endl;
						cout << "No objects currently loaded";
				}
			break;

		case 'M': // Cambio de el punto de vista
		case 'm':
			for(int contador=0;contador<4;contador++){
				for(int i=0;i<4;i++){
					view_transformation[contador][i]=0;
				}
			}			
			GetViewRefPoint(&view_ref_point, &view_transformation, &factor);
			scene_changed = true;
			break;

		case 'W': // Renderizado de la escena
		case 'w':		
		case 'Q':
		case 'q':	
		case 'E':
		case 'e':	
				switch (key)
				{
					case 'W':
					case 'w':
							typerender=0;
						break;

					case 'Q':
					case 'q':
							typerender=1;
						break;

					case 'E':
					case 'e':
							typerender=2;
						break;
				}
				clock_t comienzo,final;
				comienzo = clock();
				DEV_CLEAR_SCREEN();
				render();
				glFlush();
				final = clock();
				timerender = diffclock(final, comienzo);			
			break;
		case 'V':
		case 'v':
			cout << "Draw vertex normals (y/n)?";
			fflush(stdin);
			cin >> ch;
			//ch = 'n';
			if (ch == 'y')
			draw_vertex_normals = true;
			break;
		case 'n':
			ilu.n -= 10;
			if(ilu.n < 1) ilu.n = 1;
			break;
		case 'N':
			ilu.n+= 10;
			if(ilu.n > 255) ilu.n = 255;
			break;
		case 'a':
			ilu.Ia -= 0.1;
			if(ilu.Ia < 0) ilu.Ia = 0;
			break;
		case 'A':
			ilu.Ia += 0.1;
			if(ilu.Ia > 1) ilu.Ia = 1;
			break;			
		case 's':
			ilu.Ks -= 0.1;
			if(ilu.Ks < 0) ilu.Ks = 0;
			break;			
		case 'S':
			ilu.Ks += 0.1;
			if(ilu.Ks > 1) ilu.Ks = 1;
			break;
		case 'z':
			ilu.Ipr -= 0.1;
			if(ilu.Ipr < 0) ilu.Ipr = 0;
			break;			
		case 'Z':
			ilu.Ipr += 0.1;
			if(ilu.Ipr > 1) ilu.Ipr = 1;
			break;
		case 'x':
			ilu.Ipg -= 0.1;
			if(ilu.Ipg < 0) ilu.Ipg = 0;
			break;			
		case 'X':
			ilu.Ipg += 0.1;
			if(ilu.Ipg > 1) ilu.Ipg = 1;
			break;
		case 'c':
			ilu.Ipb -= 0.1;
			if(ilu.Ipb < 0) ilu.Ipb = 0;
			break;			
		case 'C':
			ilu.Ipb += 0.1;
			if(ilu.Ipb > 1) ilu.Ipb = 1;
			break;
		default:
		case 'd':
			ilu.Kr -= 0.1;
			if(ilu.Kr < 0) ilu.Kr = 0;
			break;			
		case 'D':
			ilu.Kr += 0.1;
			if(ilu.Kr > 1) ilu.Kr = 1;
			break;
		case 'f':
			view_ref_point.view_plane_dist -= 10;
			break;
		case 'F':
			view_ref_point.view_plane_dist += 10;
			break;	
		case 'R':
			view_ref_point.rho += 10;
			break;
		case 'r':
			view_ref_point.rho -= 10;
			break;
		case 'Y':
			size++;
			break;
		case 'y':
			size--;
			break;
		case 'g':
		case 'G':
			generateNoise();

			break;
		case 'i':
		case 'I':
			iluminacion = !iluminacion;
			break;
		case 'p':
			case 'P':
				perlin = !perlin;
				break;
	}
	
	system("cls");
	cout << endl;
	cout << " O - Load an object'" << endl;
	cout << " T - Transform an object'" << endl;
	cout << " W - Wireframe scene'" << endl;
	cout << " Q - RenderScene" << endl;
	cout << " E - Wireframe scene & RenderScene" << endl;
	cout << " M - Move view point'" << endl;
	cout << " r/R - Change rho" << endl;
	cout << " V - Draw vertex Normals" << endl;
	cout << " n/N - Exponent of Specular" << endl;
	cout << " s/S - Specular rate" << endl;
	cout << " a/A - Ambient intensity" << endl;
	cout << " zxc/ZXC - Intensiry RGB" << endl;
	cout << " f/f - View plane dist" << endl;
	cout << " Active/Deactivate Motion sensor" << endl;
	cout << " i/I Active/Deactivate Ilumination" << endl;
	cout << " Perlin Noise" << endl;
	cout << " p/P Active/Deactivate" << endl;
	cout << " y/Y Octaves" << endl;
	cout << " g Generate new noise table" << endl;
	cout << endl;			

	fflush(stdin);	
	cout << "'Your choice?'";
}

void specialKey(int key, int x, int y){
	switch(key){
	case GLUT_KEY_F1:
		activemousepas = !activemousepas;
		break;
	case GLUT_KEY_F2:
		threeD[0] = !threeD[0];
		break;
	}

}
#pragma endregion

void render(){
	int times = 1;
	int initTheta = view_ref_point.theta; 
	if(threeD[0])
		times = 2;
	view_ref_point.theta = view_ref_point.theta - 6;
	for(int i = 0 ; i < times ; i++){
		if(i == 1)
			threeD[1] = true;

		if(threeD[0])
			view_ref_point.theta += 4;

		SetViewVariables(&view_ref_point, &view_transformation);


		if(scene_changed){
			float intpart;					
			TransformScene(map_offsets, view_transformation, view_ref_point, &object_head, &z_params, &scene_changed, factor);	
		
			Vector world;
			Vector eye;
			Vector ejesreal[5];

			world.x = 0;
			world.y = 0;
			world.z = 0;
			VectorMatrix(world,view_transformation,&eye);
					
			ejesreal[4].x = (view_ref_point.view_plane_dist * (eye.x/eye.z));
			ejesreal[4].y = (view_ref_point.view_plane_dist * (eye.y/eye.z));
			ejesreal[4].z = (view_ref_point.view_plane_dist * (eye.z/eye.z));



			if(threeD[0]){
				if(threeD[1])
					modf ((map_offsets.xmid + (map_offsets.xmid / 2) ) + (ejesreal[4].x * factor) , &intpart);
				else
					modf ((map_offsets.xmid / 2 ) + (ejesreal[4].x * factor) , &intpart);
			}else{
				modf (map_offsets.xmid  + (ejesreal[4].x * factor) , &intpart);
			}
			ejes[4].x = ((int) intpart);
			modf (map_offsets.ymid  + (ejesreal[4].y * factor), &intpart);
			ejes[4].y = ((int) intpart);
			modf (ejesreal[4].z, &intpart);
			ejes[4].z = ((int) intpart);



			world.x = view_ref_point.rho;
			world.y = 0;
			world.z = 0;
			VectorMatrix(world,view_transformation,&eye);
					
			ejesreal[0].x = (view_ref_point.view_plane_dist * (eye.x/eye.z));
			ejesreal[0].y = (view_ref_point.view_plane_dist * (eye.y/eye.z));
			ejesreal[0].z = (view_ref_point.view_plane_dist * (eye.z/eye.z));
					


			if(threeD[0]){
				if(threeD[1])
					modf ((map_offsets.xmid + (map_offsets.xmid / 2) ) + (ejesreal[0].x * factor) , &intpart);
				else
					modf ((map_offsets.xmid / 2 ) + (ejesreal[0].x * factor) , &intpart);
			}else{
				modf (map_offsets.xmid  + (ejesreal[0].x * factor) , &intpart);
			}
			ejes[0].x = ((int) intpart);
			modf (map_offsets.ymid  + (ejesreal[0].y * factor), &intpart);
			ejes[0].y = ((int) intpart);
			modf (ejesreal[0].z, &intpart);
			ejes[0].z = ((int) intpart);
					
			world.x = 0;
			world.y = view_ref_point.rho;
			world.z = 0;
			VectorMatrix(world,view_transformation,&eye);
				
			ejesreal[1].x = (view_ref_point.view_plane_dist * (eye.x/eye.z));
			ejesreal[1].y = (view_ref_point.view_plane_dist * (eye.y/eye.z));
			ejesreal[1].z = (view_ref_point.view_plane_dist * (eye.z/eye.z));
					


			if(threeD[0]){
				if(threeD[1])
					modf ((map_offsets.xmid + (map_offsets.xmid / 2) ) + (ejesreal[1].x * factor) , &intpart);
				else
					modf ((map_offsets.xmid / 2 ) + (ejesreal[1].x * factor) , &intpart);
			}else{
				modf (map_offsets.xmid  + (ejesreal[1].x * factor) , &intpart);
			}
			ejes[1].x = ((int) intpart);
			modf (map_offsets.ymid  + (ejesreal[1].y * factor), &intpart);
			ejes[1].y = ((int) intpart);
			modf (ejesreal[1].z, &intpart);
			ejes[1].z = ((int) intpart);

			world.x = 0;
			world.y = 0;
			world.z = view_ref_point.rho - 1;
			VectorMatrix(world,view_transformation,&eye);

			ejesreal[2].x = (view_ref_point.view_plane_dist * (eye.x/eye.z));
			ejesreal[2].y = (view_ref_point.view_plane_dist * (eye.y/eye.z));
			ejesreal[2].z = (view_ref_point.view_plane_dist * (eye.z/eye.z));

			if(threeD[0]){
				if(threeD[1])
					modf ((map_offsets.xmid + (map_offsets.xmid / 2) ) + (ejesreal[2].x * factor) , &intpart);
				else
					modf ((map_offsets.xmid / 2 ) + (ejesreal[2].x * factor) , &intpart);
			}else{
				modf (map_offsets.xmid  + (ejesreal[2].x * factor) , &intpart);
			}
			ejes[2].x = ((int) intpart);
			modf (map_offsets.ymid  + (ejesreal[2].y * factor), &intpart);
			ejes[2].y = ((int) intpart);
			modf (ejesreal[2].z, &intpart);
			ejes[2].z = ((int) intpart);

			if(threeD[0]){
				int limMax, limMin;
				if(threeD[1]){
					limMax = DEV_MAX_X_RES;
					limMin = DEV_MAX_X_RES / 2 + 1;
				}else{
					limMax = DEV_MAX_X_RES / 2 - 1;
					limMin = 0;
				}
				for(int i = 0 ; i < 3 ; i++){
					if(ejes[i].x > limMax && threeD[1] == false){ // Limite de los ejes para el ojo izquierdo
						Vector one, two;
						one.x = ejes[i].x;
						one.y = ejes[i].y;
						two.x = ejes[4].x;
						two.y = ejes[4].y;
						if(one.y > two.y){
							Vector temp;
							temp = one;
							one = two;
							two = temp;
						}
						float pendiente = (two.y - one.y)/(two.x - one.x);
						float b = ejes[4].y - pendiente * ejes[4].x;

						ejes[i].x = limMax;
						//ejes[i].y = b;
						ejes[i].y = pendiente * limMax + b;

					}else
					if(ejes[i].x < limMin && threeD[1] == true){ // Limite de los ejes para el ojo derecho
						Vector one, two;
						one.x = ejes[i].x;
						one.y = ejes[i].y;
						two.x = ejes[4].x;
						two.y = ejes[4].y;
						if(one.y > two.y){
							Vector temp;
							temp = one;
							one = two;
							two = temp;
						}
						//float pendiente = ( two.y - two.x)/( one.y - one.x);
						float pendiente = (two.y - one.y)/(two.x - one.x);
						float b = ejes[i].y - pendiente * ejes[i].x;
						//cout << b << endl; 
						//float b = ejes[4].y + pendiente * ejes[4].x;
						//cout << b << endl; 
						ejes[i].x = limMin;
						 //ejes[i].y = b;
						ejes[i].y = pendiente * limMin + b;
						
					}
				}			
			}
		}
		if(calculate_polygon_normals){
			ObjectPtr current_object;
			current_object = object_head;
			while(current_object != NULL){
				CalculatePolygonNormals(current_object);
				current_object = current_object->next;
			}	
		}
	
		if(calculate_vertex_normals){
			ObjectPtr current_object;
			current_object = object_head;
			while(current_object != NULL){
				CalculateVertexNormals(current_object);
				current_object = current_object->next;
			}
		}
		if( calculate_polygon_normals || scene_changed){
			hideObjects(&object_head, view_ref_point);
			calculate_vertex_normals = false;
			calculate_polygon_normals = false;
		}
		

		dibujaEjes(ejes);

		if(typerender==0)
			WireFrameScene(map_offsets, view_transformation, view_ref_point, object_head, ejes, factor);
		else if(typerender==1)
			RenderScene(object_head, view_ref_point, view_transformation, ilu, threeD);
		else if(typerender==2){
			RenderScene(object_head, view_ref_point, view_transformation, ilu, threeD);
			WireFrameScene(map_offsets, view_transformation, view_ref_point, object_head, ejes, factor);
		}
		//if(!threeD)
			imprimeStats();
		
		threeD[1] = false;
	}
	view_ref_point.theta = initTheta; 
}

#pragma region Mousefuncs
bool leftm = false, rightm = false, middlem = false;
int xmouse = 0, ymouse = 0;


void mouse(int button, int state, int x, int y){
	switch(button){
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN){
			leftm = true;
			xmouse = x;
			ymouse = y;
		}
		else{
			leftm = false;
			/*cout << buffer[x][y];
			if(buffer[x][y] != -1){
				ObjectPtr current_object;
				current_object = object_head;
				while(current_object != NULL){
					if(buffer[x][y] == current_object->id_no){
						current_object->selected = true;
						return;
					}else{
						current_object->selected = false;
					}

					current_object = current_object->next;
				}
			}			*/
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if(state == GLUT_DOWN){
			rightm = true;
			xmouse = x;
			ymouse = y;
		}
		else
			rightm = false;
		break;
	case GLUT_MIDDLE_BUTTON:		
		if(state == GLUT_DOWN){
			middlem = true;
			xmouse = x;
			ymouse = y;
		}
		else
			middlem = false;
		break;
	}
}

void mouseMotion(int x, int y){
	if(x == xmouse && y == ymouse)
		return;

	if(rightm){
		view_ref_point.theta += (x - xmouse) / 3;
		if( view_ref_point.theta > 360 ){
			view_ref_point.theta = 0;
		}else if( view_ref_point.theta < 0 ){
			view_ref_point.theta = 360;
		}
		view_ref_point.phi += (y - ymouse) / 3;
		if( view_ref_point.phi > 360)
			view_ref_point.phi = 0;
		else if( view_ref_point.phi < 0)
			view_ref_point.phi = 360;
		scene_changed = true;
	}
	if(middlem){
		view_ref_point.rho += (y - ymouse) / 2;
	}
	xmouse = x;
	ymouse = y;	
	SetViewVariables(&view_ref_point, &view_transformation);
	clock_t comienzo,final;
	comienzo = clock();
	DEV_CLEAR_SCREEN();
	render();
	glFlush();
	final = clock();
	timerender = diffclock(final, comienzo);
}

void mousePassive(int x, int y){
	//cout << "aqui1" << endl;
	if(activemousepas==true){
			//cout << "aqui" << endl;
			//getchar();
			view_ref_point.theta += (x - xmouse) / 10;
			if( view_ref_point.theta > 360 ){
				view_ref_point.theta = 0;
			}else if( view_ref_point.theta < 0 ){
				view_ref_point.theta = 360;
			}
			view_ref_point.phi += (y - ymouse) / 10;
			if( view_ref_point.phi > 360)
				view_ref_point.phi = 0;
			else if( view_ref_point.phi < 0)
				view_ref_point.phi = 360;
			xmouse = x;
			ymouse = y;
			scene_changed = true;
			SetViewVariables(&view_ref_point, &view_transformation);
			clock_t comienzo,final;
			comienzo = clock();
			DEV_CLEAR_SCREEN();
			render();
			glFlush();
			final = clock();
			timerender = diffclock(final, comienzo);
	}
	
}

#pragma endregion

#pragma region changeSize
void changeSize(int w, int h) {

	//// Prevent a divide by zero, when window is too short
	//// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	//float ratio =  w * 1.0 / h;
	map_offsets.xmid = w/2;
	map_offsets.ymid = h/2;
	// Set viewport size to be entire OpenGL window.
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  
   // Set matrix mode to projection.
   glMatrixMode(GL_PROJECTION);

   // Clear current projection matrix to identity.
   glLoadIdentity();

   // Specify the orthographic (or perpendicular) projection, 
   // i.e., define the viewing box.
   glOrtho(0, w, 0, h, 1.0, 0.0);
   //glOrtho(-1, 3.0, -2, 2.0, 1.0, 0.0);
   // Set matrix mode to modelview.
   glMatrixMode(GL_MODELVIEW);

   // Clear current modelview matrix to identity.
   glLoadIdentity();
   clock_t comienzo,final;
	comienzo = clock();
	DEV_CLEAR_SCREEN();
	render();
	glFlush();
	final = clock();
	timerender = diffclock(final, comienzo);
}
#pragma endregion

#pragma region Text
int objects = 0, polygons = 0, vertices = 0;

void imprimeStats(){	
    glRasterPos2f(5, DEV_MAX_Y_RES - 70);
	renderString(GLUT_BITMAP_8_BY_13, "Phi: ");
	stringstream out;
	out << view_ref_point.phi;
	renderString(GLUT_BITMAP_8_BY_13, out.str());
	renderString(GLUT_BITMAP_8_BY_13, " Theta: ");	
	stringstream out2;
	out2 << view_ref_point.theta;
	renderString(GLUT_BITMAP_8_BY_13, out2.str());
	renderString(GLUT_BITMAP_8_BY_13, " Rho: ");	
	stringstream out6;
	out6 << view_ref_point.rho;
	renderString(GLUT_BITMAP_8_BY_13, out6.str());
	renderString(GLUT_BITMAP_8_BY_13, " View_plane_dist: ");	
	stringstream out10;
	out10 << view_ref_point.view_plane_dist;
	renderString(GLUT_BITMAP_8_BY_13, out10.str());
	if(objects_added){		
		objects = 0; polygons = 0; vertices = 0;
		ObjectPtr current_object;
		SurfacePtr current_surface;
		current_object = object_head;
		while(current_object != NULL){
			objects++;
			vertices += current_object->no_vertices;
			current_surface = current_object->surface_head;
			while(current_surface != NULL){
				polygons += current_surface->no_polygons;		
				current_surface = current_surface->next;
			}
			current_object = current_object->next;
		}
		objects_added = false;
	}
	glRasterPos2f(5, DEV_MAX_Y_RES - 85);
	renderString(GLUT_BITMAP_8_BY_13, "No of objects: ");
	stringstream out3;
	out3 << objects;
	renderString(GLUT_BITMAP_8_BY_13, out3.str());
	renderString(GLUT_BITMAP_8_BY_13, "  No of polygons: ");	
	stringstream out4;
	out4 << polygons;
	renderString(GLUT_BITMAP_8_BY_13, out4.str());
	renderString(GLUT_BITMAP_8_BY_13, "  No of vertices: ");	
	stringstream out5;
	out5 << vertices;
	renderString(GLUT_BITMAP_8_BY_13, out5.str());
	
	glRasterPos2f(5, DEV_MAX_Y_RES - 100);
	renderString(GLUT_BITMAP_8_BY_13, "Time rendering: ");	
	stringstream out7;
	out7 << timerender;
	renderString(GLUT_BITMAP_8_BY_13, out7.str());
	/*glRasterPos2f(5, DEV_MAX_Y_RES - 80);
	renderString(GLUT_BITMAP_8_BY_13, "Position buffer: ");	
	stringstream out8;
	out8 << resbuffer;*/
	//renderString(GLUT_BITMAP_8_BY_13, out8.str());
	glFlush();
}

#pragma endregion

void printMenu(void){

	DEV_CLEAR_SCREEN();
	//imprimeStats();
	render();
	glFlush();
	system("cls");
	cout << endl;
	cout << " O - Load an object'" << endl;
	cout << " T - Transform an object'" << endl;
	cout << " W - Wireframe scene'" << endl;
	cout << " Q - RenderScene" << endl;
	cout << " E - Wireframe scene & RenderScene" << endl;
	cout << " M - Move view point'" << endl;
	cout << " V - Draw vertex Normals" << endl;
	cout << " n/N - Exponent of Specular" << endl;
	cout << " s/S - Specular rate" << endl;
	cout << " a/A - Ambient intensity" << endl;
	cout << " zxc/ZXC - Intensiry RGB" << endl;
	cout << " f/f - View plane dist" << endl;
	cout << " Active/Deactivate Motion sensor" << endl;
	cout << " i/I Active/Deactivate Ilumination" << endl;
	cout << " Perlin Noise" << endl;
	cout << " p/P Active/Deactivate" << endl;
	cout << " s/S Octaves" << endl;
	cout << " g Generate new noise table" << endl;
	cout << endl;			
	cout << "'Your choice?'";
}


#pragma region main
int main(int argc, char **argv) {
		

	generateNoise();

	// Set display mode as single-buffered and RGB color.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
   
	// Set OpenGL window size.
	glutInitWindowSize(DEV_MAX_X_RES, DEV_MAX_Y_RES);

	// Set position of OpenGL window upper-left corner.
	glutInitWindowPosition(800, 0); 
   
	// Create OpenGL window with title.
	glutCreateWindow("Entorno");
   
	// Initialize.
	glClearColor(1.0, 1.0, 1.0, 0.0); 
   
	// Register display routine.
	glutDisplayFunc(printMenu); 
   
	// Register reshape routine.
	glutReshapeFunc(changeSize);
		
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specialKey);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mousePassive);

	// Register keyboard routine.
	//glutKeyboardFunc(keyInput);
	Initialize(&map_offsets, &view_ref_point, &view_transformation, &scene_changed, &no_of_objects, &object_head, &ilu);
		
	// Begin processing.
	glutMainLoop(); 	
	return 0;  
}	
#pragma endregion

