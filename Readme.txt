3D Simple Renderer

A simple real time renderer engine. Similar to OpenGL or DirectX.
This engine reads OBJ files with all the vertex first, and then the faces, ignoring the rest,
rendering the object or objects added throught CPU instead of GPU. It also transform
the objects, rotating, scaling or moving them.
It uses the Blinn Phong shading model interpolatign the normal allowing to change ambiental, difuse, and specular coeficient.
Adds simple 3D Perlin, , marble and wood textures to the objects.
Dependecies: 
- GLUT (For Windows): https://www.opengl.org/resources/libraries/glut/glut37.zip
- GLEW (For Windows): https://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0-win32.zip/download
