#include "LoadObject.h"
#pragma region Funciones de carga de objeto

// Añade un poligono a la lista de poligonos del vertice, para que cada vertice sepa a que poligonos pertenece
#pragma region AddPolygonToPolygonList
void AddPolygonToPolygonList(PolygonPtr current_polygon, PolygonListPtr *poly_list){
	if ((*poly_list) == NULL){
		(*poly_list) = new PolygonListCell;
		(*poly_list)->poly = current_polygon;
		(*poly_list)->rest = NULL;
	}
	else
		AddPolygonToPolygonList(current_polygon, &(*poly_list)->rest);
}
#pragma endregion

// Crea las superficies del objeto, los poligonos y le dice a cada poligono de que vertices esta compuesto
#pragma region MakeSurfaces
void MakeSurfaces(int no_surfaces, ObjectPtr *object_head,
	int *no_vertices, VertexPtr *vertex_head, int *surface_at,
	VertexPtr *vertex_at, string object_file, int no_polygons, ifstream *inputsur, string buffer, Vector *vertexNormals){


	int surface_count;
	SurfacePtr current_surface;

	int **polygons_surface;
	int **vertex_polygons;
	int *vertex_normals;

	// Creamos un array con las posiciones del numero de poligonos y inicializamos a 0
	polygons_surface = new int *[(*object_head)->no_surfaces];
	for (int i = 0; i < (*object_head)->no_surfaces; i++)
		polygons_surface[i] = new int[no_polygons];

	vertex_polygons = new int *[no_polygons];
	for (int i = 0; i < no_polygons; i++)
		vertex_polygons[i] = new int[38];

	for (int i = 0; i < (*object_head)->no_surfaces; i++)
		for (int j = 0; j < no_polygons; j++)
			polygons_surface[i][j] = 0;

	for (int i = 0; i < no_polygons; i++)
		for (int j = 0; j < 20; j++)
			vertex_polygons[i][j] = 0;

	if ((*object_head)->vertexInFile){
		vertex_normals = new int[(*object_head)->no_vertices];
		for (int i = 0; i < (*object_head)->no_vertices; i++)
			vertex_normals[i] = 0;
	}

	bool face = false, surface = false;
	int contfac = -1, contvert = 0, contsurface = -1;
	int bar = 0;
	int countnum = 0;
	int numbers[3];
	string cadena;
	int length;
	int cont;

	do{
		if (buffer == "f"){
			surface = false;
			face = true;
			contfac++;
			*inputsur >> buffer;
			contvert = 0;
			bar = 0;
			countnum = 0;
			while (!inputsur->eof() && isNumber(buffer.c_str(), buffer.length())){
				// Si solo hay una barra significa que guardamos la normal del vertice
				// Si hay dos tambien hay posicion de la textura
				length = (int)buffer.length();
				numbers[0] = 0;
				numbers[1] = 0;
				numbers[2] = 0;
				cadena = buffer.c_str();
				countnum = 0;
				for (int i = 0; i < length; i++){
					if (cadena.substr(i, 1) == "/"){
						bar++;
						countnum++;
					}
					else{
						string numero = cadena.substr(i, 1);
						numbers[countnum] = ::atoi(numero.c_str()) + (numbers[countnum] * 10);
					}
				}
				if (bar == 1){
					numbers[2] = numbers[1];
					numbers[1] = 0;
				}
				if ((*object_head)->vertexInFile)
					if (bar > 0){
						if (vertex_normals[numbers[0] - 1] == 0){
							vertex_normals[numbers[0] - 1] = numbers[2];
							vertex_at[numbers[0] - 1]->vertex_normal = vertexNormals[numbers[2] - 1];
						}
					}
				vertex_polygons[contfac][contvert] = numbers[0];
				contvert++;
				*inputsur >> buffer;
			}
		}
		else if (buffer == "su" && (*object_head)->no_surfaces != 1){
			contsurface++;
			face = false;
			surface = true;
			cont = 0;
			*inputsur >> buffer;
			while (!inputsur->eof() && isNumber(buffer.c_str(), buffer.length())){
				(*(*(polygons_surface + contsurface) + cont)) = ::atoi(buffer.c_str());
				cont++;
				*inputsur >> buffer;
			}
		}
		else{
			face = false;
			surface = false;
			*inputsur >> buffer;
		}
	} while (!inputsur->eof());

	// Si solo hay una superficie (Debido a que el archivo no contiene superficies) 
	// asignamos todos los poligonos a esta
	if (no_surfaces == 1){
		for (int i = 0; i < no_polygons; i++)
			polygons_surface[0][i] = i + 1;
	}

	// Empezamos a leer los poligonos del objeto
	(*object_head)->surface_head = NULL;


	for (surface_count = 0; surface_count < no_surfaces; surface_count++){
		// Creamos la cabecera de la superficie		

		if ((*object_head)->surface_head == NULL){
			current_surface = new SurfaceCell;
			current_surface->next = NULL;
			current_surface->polygon_head = NULL;
			current_surface->no_polygons = 0;
			(*object_head)->surface_head = current_surface;
		}
		else{
			current_surface->next = new SurfaceCell;
			current_surface = current_surface->next;
		}

		int contapol = 0;
		for (contapol = 0; contapol < no_polygons; contapol++)
			if (polygons_surface[surface_count][contapol] == 0){
				break;
			}
		current_surface->no_polygons = contapol;

		// Empezamos a leer los poligonos de la superficie
		int poly_count;
		PolygonPtr current_polygon = NULL;

		current_surface->polygon_head = NULL;
		contfac = 0;
		for (poly_count = 0; poly_count < current_surface->no_polygons; poly_count++){
			// Creamos la cabecera del poligono

			if (current_surface->polygon_head == NULL){
				current_polygon = new PolygonCell;
				current_surface->polygon_head = current_polygon;
				current_surface->polygon_head->poly_visible = true;
				current_surface->polygon_head->poly_normal.x = 0;
				current_surface->polygon_head->poly_normal.y = 0;
				current_surface->polygon_head->poly_normal.z = 0;
			}
			else{
				current_polygon->next = new PolygonCell;
				current_polygon = current_polygon->next;
			}
			int current_vertex;
			VertexListPtr vertex_list = NULL;
			VertexPtr temp_vertex = NULL;

			(*current_polygon).vertex_list_head = NULL;
			int cont = 0;

			contvert = 0;
			while (vertex_polygons[(polygons_surface[surface_count][poly_count]) - 1][contvert] > 0){

				current_vertex = vertex_polygons[polygons_surface[surface_count][poly_count] - 1][contvert] - 1;
				//Creamos la cabecera de la lista de vertices del poligono
				if ((*current_polygon).vertex_list_head == NULL){
					vertex_list = new VertexListCell;
					(*current_polygon).vertex_list_head = vertex_list;
				}
				else{
					vertex_list->rest = new VertexListCell;
					vertex_list = vertex_list->rest;
				}

				if (surface_at[current_vertex] == 0)
					surface_at[current_vertex] = surface_count;
				else if (surface_at[current_vertex] != surface_count){
					no_vertices++;
					surface_at[current_vertex] = surface_count;
					vertex_at[current_vertex]->poly_list_head = NULL;
				}
				(*vertex_list).vertex = vertex_at[current_vertex];

				AddPolygonToPolygonList(current_polygon, &vertex_list->vertex->poly_list_head);

				contvert++;
			}
			vertex_list->rest = NULL;
			contfac++;
		}
		current_polygon->next = NULL;

	}

	// Liberamos la memoria reservada que no se va a utilizar mas
	for (int i = 0; i < (*object_head)->no_surfaces; i++)
		delete[] polygons_surface[i];

	for (int i = 0; i < no_polygons; i++)
		delete[] vertex_polygons[i];
	delete[] vertex_polygons;
	//	delete[] vertex_normals;
}

#pragma endregion

// Leemos los vertices del fichero y los guardamos en una lista que contiene todos los ficheros en el orden de lectura
#pragma region ReadVertices
void ReadVertices(int no_vertices, VertexPtr *vertex_head, int *surface_at, VertexPtr *vertex_at, ifstream *input, string *buffer){
	int vertex_count = 0;

	while (!input->eof() && (*buffer != "v")){
		*input >> *buffer;
	}

	// Leemos los vertices para guardar las posiciones de todos los vertices en el array
	while (!input->eof() && ((!isNumber(buffer->c_str(), buffer->length())) && *buffer == "v")) {
		/*if(buffer->substr(0,2) == "vn"){
		}else if(buffer->substr(0,2) == "vt"){
		}else*/ if (buffer->substr(0, 1) == "v"){
			surface_at[vertex_count] = 0;
			vertex_at[vertex_count] = new VertexCell;
			*input >> *buffer;
			vertex_at[vertex_count]->local_pos.x = ::atof(buffer->c_str());
			*input >> *buffer;
			vertex_at[vertex_count]->local_pos.y = ::atof(buffer->c_str());
			*input >> *buffer;
			vertex_at[vertex_count]->local_pos.z = ::atof(buffer->c_str());

			vertex_at[vertex_count]->poly_list_head = NULL;
			vertex_count++;
		}
	*input >> *buffer;
	}

	// Ponemos bien la estructuctura de vertex_head en la que se guarda un listado de todos los vertices.
	*vertex_head = vertex_at[0];
	for (vertex_count = 0; vertex_count < no_vertices - 1; vertex_count++)
		vertex_at[vertex_count]->next = vertex_at[vertex_count + 1];

	vertex_at[no_vertices - 1]->next = NULL;
}
#pragma endregion

#pragma region loadNormals
void loadNormals(ifstream *input, Vector *vertexNormals, string *buffer){
	int count = 0;
	while (!input->eof() && (*buffer != "vn")){
		*input >> *buffer;
	}

	do{
		if (*buffer == "vn"){
			*input >> *buffer;
			vertexNormals[count].x = ::atof(buffer->c_str());
			*input >> *buffer;
			vertexNormals[count].y = ::atof(buffer->c_str());
			*input >> *buffer;
			vertexNormals[count].z = ::atof(buffer->c_str());

		}
		*input >> *buffer;
		count++;
	} while (!input->eof() && ((!isNumber(buffer->c_str(), buffer->length())) && *buffer == "vn"));
}

#pragma endregion

// Funcion que guarda en memoria los datos de un objeto 3D en formato OBJ
#pragma region LoadObject
int LoadObject(string object_name, int no_of_objects, ObjectPtr *object_head, ViewPointRec *view_ref_point, Matrix *view_transformation, int *factor){
	int const MaxNoOFVertices = 120000;
	Vector *vertexNormals;
	string object_file;
	ObjectPtr current_object;
	ifstream input;
	string buffer;
	int no_polygon, no_textures = 0, no_normals = 0;
	VertexPtr vertex_at[MaxNoOFVertices];
	int surface_at[MaxNoOFVertices];

	cout << "Abre fichero" << endl;
	input.open(object_name);
	if (!input.is_open()){
		return	0;
	}

	for (int i = 0; i < MaxNoOFVertices; i++){
		surface_at[i] = 0;
	}

	object_file = object_name;

	current_object = new ObjectCell;

	current_object->next = (*object_head);
	(*object_head) = current_object;
	(*(*object_head)).name = object_name;
	(*object_head)->selected = false;
	(*(*object_head)).id_no = no_of_objects;
	(*(*object_head)).no_vertices = 0;
	(*(*object_head)).no_surfaces = 0;
	(*object_head)->vertexInFile = false;
	no_polygon = 0;

	/// Contamos los vertices, superficies, etc

	while (!input.eof()){
		input >> buffer;
		if (buffer.substr(0, 2) == "vn"){  // Cuenta las normales
			no_normals += 1;
		}
		else if (buffer.substr(0, 2) == "vt"){ // Cuenta las coordenadas de las texturas
			no_textures += 1;
		}
		else if (buffer.substr(0, 1) == "v"){
			(*object_head)->no_vertices++;
		}
		else if (buffer.substr(0, 1) == "f"){ // Cuenta los poligonos			
			no_polygon++;
		}
		else if (buffer.substr(0, 2) == "su"){ // Cuanta las superficies
			(*object_head)->no_surfaces++;
		}
		else if (buffer.substr(0, 1) == "o"){ // Saca el nombre del objeto
			input >> buffer;
			(*object_head)->name = buffer.c_str();
		}
	}

	if ((*object_head)->no_vertices > MaxNoOFVertices){
		cout << "El objeto tiene demasiados vertices." << endl;
		delete current_object;
		getchar();
		return 0;
	}
	if ((*(*object_head)).no_surfaces == 0)
		(*(*object_head)).no_surfaces = 1;

	input.close();



	input.open(object_name);
	input.clear();
	if (!input.is_open()){
		return	0;

	}
	buffer = "";

	ReadVertices((*object_head)->no_vertices, &(*object_head)->vertex_head,
		surface_at, vertex_at, &input, &buffer);
	if (no_normals != 0){
		(*object_head)->vertexInFile = true;
		vertexNormals = new Vector[no_normals];

		loadNormals(&input, vertexNormals, &buffer);


	}
	MakeSurfaces((*object_head)->no_surfaces, object_head, &(*object_head)->no_vertices, &(*object_head)->vertex_head, surface_at, vertex_at, object_name, no_polygon, &input, buffer, vertexNormals);
	input.close();
	cout << "Cierra fichero" << endl;


	cout << "Abrir fichero de opciones" << endl;

	char optionfile[260];
	int longi;
	char line[260];
	char copy[260];

	strcpy(optionfile, object_name.c_str());
	longi = strlen(optionfile);
	optionfile[longi - 3] = 't'; optionfile[longi - 2] = 'x'; optionfile[longi - 1] = 't';

	input.open(optionfile);
	if (!input.is_open()){
		return 1;
	}
	else
	{
		while (!input.eof())
		{
			input.getline(line, 260, '\n');
			if (line[0] != '#' && line[0] != 0)
			{
				if (line[0] == 'R' && line[1] == 'h' && line[2] == 'o')
				{
					copyfun(copy, line, 4);
					view_ref_point->rho = atoi(copy);
				}
				else if (line[0] == 'T' && line[1] == 'h' && line[2] == 'e' && line[3] == 't' && line[4] == 'a')
				{
					copyfun(copy, line, 6);
					view_ref_point->theta = atoi(copy);
				}
				else if (line[0] == 'P' && line[1] == 'h' && line[2] == 'i')
				{
					copyfun(copy, line, 4);
					view_ref_point->phi = atoi(copy);
				}
				else if (line[0] == 'V' && line[1] == 'd')
				{
					copyfun(copy, line, 3);
					view_ref_point->view_plane_dist = atoi(copy);
				}
				else if (line[0] == 's' && line[1] == 'l')
				{
					copyfun(copy, line, 3);
					*factor = atoi(copy);
				}
			}
		}
		SetViewVariables(view_ref_point, view_transformation);
	}
	input.close();
	cout << "Cierra fichero" << endl;
	return 1;
}
#pragma endregion

#pragma endregion

