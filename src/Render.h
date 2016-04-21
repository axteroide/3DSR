#include "Object.h"

void RenderPolygon(PolygonPtr current_polygon, EdgeBuffer edge_list_at, Vector light_vector, int ** z_buffer, ViewPointRec view_ref_point, struct ValoresIluminacion, bool threeD[]);
void RenderObject(ObjectPtr current_object, EdgeBuffer edge_list_at, Vector light_vector, int ** z_buffer, ViewPointRec view_ref_point, struct ValoresIluminacion, bool threeD[]);
void RenderPixel(int x, int y, float i, float r, Vector w, struct ValoresIluminacion);
void RenderSpan(int y, EdgeBoxPtr *edge_box1, EdgeBoxPtr * edge_box2, int ** z_buffer_at, struct ValoresIluminacion, ViewPointRec view_ref_point, bool threeD[]);
void RenderScene(ObjectPtr object_head, ViewPointRec view_ref_point, Matrix view_transformation, struct ValoresIluminacion, bool threeD[]);
void renderString(void *font, string s);