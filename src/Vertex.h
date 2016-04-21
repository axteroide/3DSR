#pragma once
typedef struct VertexCell * VertexPtr;
typedef struct VertexListCell * VertexListPtr;

struct VertexListCell{
	VertexPtr vertex;
	VertexListPtr rest;
};
