/* Marta Garcia (marta.ggonzalez@students.salle.url.edu) */
/* Carla Abellana (carlabo.abellana@students.salle.url.edu) */

//include some standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

//include OpenGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//include some custom code files
#include "glfunctions.h" //include all OpenGL stuff
#include "Shader.h" // class to compile shaders

//libreries transaltion mix
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;
//defines
#define TINYOBJLOADER_IMPLEMENTATION
#define NULL 0
//includes fitxers
#include "tiny_obj_loader.h"
#include "imageloader.h"
//fitxers dels objectes
const char filename[] = "assets/sphere.obj";
const char filename1[] = "assets/star.obj";
const char filename2[] = "assets/plane.obj";
//vectors on s'emmagatzemen les figures, per crear VAO
vector <tinyobj::shape_t> shapes;
vector <tinyobj::shape_t> shapes_star;
vector <tinyobj::shape_t> shapes_plane;
vector <tinyobj::shape_t> shapes_sky;

//variables globals
int g_ViewportWidth = 512; int g_ViewportHeight = 512;	// Default window size, in pixels
double mouse_x, mouse_y;								//variables storing mouse position
const vec3 g_backgroundColor(0.2f, 0.2f, 0.2f);			// background colour - a GLM 3-component vector

GLuint g_simpleShader = 0;	//shader identifier
GLuint g_Vao[] = {0, 0, 0};	//vao
GLuint g_NumTriangles = 0;	//  Numbre of triangles we are painting.
GLuint g_NumTriangles_star = 0;
GLuint g_NumTriangles_plane = 0;

GLuint g_simpleShader_sky = 0;	//shader identifier
GLuint g_Vao_sky = 0;			//vao
GLuint g_NumTriangles_sky = 0;	// Number of triangles we are painting.

//variables for planetary rotation
float rota1 = 0.0f;
float rota2 = 0.0f;
float rota_merc = 0.0f;
float rota_sat = 0.0f;
float rota_ura = 0.0f;
float rota_nept = 0.0f;
//variables for camera operations
vec3 aux = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraPos = vec3(1.0f, 1.0f, 1.0f);
vec3 cameraPosit = vec3(0.0f, 2.0f, 5.0f);
vec3 cameraCenter = vec3(0.0f, 0.0f, -2.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
mat4 view_matrix = glm::lookAt(
	cameraPos,
	cameraCenter,
	cameraUp
);

//Variables for planets textures 
GLuint texture_sun;
GLuint texture_earth;
GLuint texture_moon;
GLuint texture_mercury;
GLuint texture_saturn;
GLuint texture_rings;
GLuint texture_uranus;
GLuint texture_neptune;

//other textures used
GLuint texture_clouds;
GLuint texture_star;
GLuint texture_id_sky;
GLuint texture_id_earth_normal;
GLuint texture_id_earth_spec;
GLuint texture_id_clouds_trans;

//mouse movement
bool firstMouse = true;
float _yaw = -90.0f;
float _pitch = 0.0f;
float lastX = 0.0f;
float lastY = 0.0f;

//Variables for the camera and light direction
vec3 cameraFront = vec3(0.0f, 0.0f, 0.0f);
vec3 g_light_dir(0.0f, 0.0f, -2.0);

// ------------------------------------------------------------------------------------------
// This function manually creates a square geometry (defined in the array vertices[])
// ------------------------------------------------------------------------------------------

void load(){
	
	//**********************
	// CODE TO SET GEOMETRY
	//**********************

	//Textures secundàries

	Image* earth_normal = loadBMP("assets/earthnormal.bmp");
	glGenTextures(1, &texture_id_earth_normal);
	glBindTexture(GL_TEXTURE_2D, texture_id_earth_normal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, // target
		0,						// level = 0 base, no mipmaps
		GL_RGB,					// how the data will be stored
		earth_normal->width,	// width of the image
		earth_normal->height,	// height of the image
		0,						// border
		GL_RGB,					// format of original data
		GL_UNSIGNED_BYTE,		// type of data
		earth_normal->pixels);	// pointer to the start of data
	
	Image* earth_spec = loadBMP("assets/earthspec.bmp");
	glGenTextures(1, &texture_id_earth_spec);
	glBindTexture(GL_TEXTURE_2D, texture_id_earth_spec);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,	// target
		0,						// level = 0 base, no mipmaps
		GL_RGB,					// how the data will be stored
		earth_spec->width,		// width of the image
		earth_spec->height,		// height of the image
		0,						// border
		GL_RGB,					// format of original data
		GL_UNSIGNED_BYTE,		// type of data
		earth_spec->pixels);	// pointer to the start of data

	
	Image* cloud_trans = loadBMP("assets/cloudstrans.bmp");
	glGenTextures(1, &texture_id_clouds_trans);
	glBindTexture(GL_TEXTURE_2D, texture_id_clouds_trans);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,	// target
		0,						// level = 0 base, no mipmaps
		GL_RGB,					// how the data will be stored
		cloud_trans->width,		// width of the image
		cloud_trans->height,	// height of the image
		0,						// border
		GL_RGB,					// format of original data
		GL_UNSIGNED_BYTE,		// type of data
		cloud_trans->pixels);	// pointer to the start of data
	
	Image* rings_trans = loadBMP("assets/saturn_rings.bmp");
	glGenTextures(1, &texture_rings);
	glBindTexture(GL_TEXTURE_2D, texture_rings);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,	// target
		0,						// level = 0 base, no mipmaps
		GL_RGB,					// how the data will be stored
		rings_trans->width,		// width of the image
		rings_trans->height,	// height of the image
		0,						// border
		GL_RGB,					// format of original data
		GL_UNSIGNED_BYTE,		// type of data
		rings_trans->pixels);	// pointer to the start of data
	
	//Textures principals

	Image* image_sun = loadBMP("assets/sun.bmp");
	glGenTextures(1, &texture_sun);
	glBindTexture(GL_TEXTURE_2D, texture_sun);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, // target
		0,						// level = 0 base, no mipmap
		GL_RGB,					// how the data will be stored
		image_sun->width,		// width of the image
		image_sun->height,		// height of the image
		0,						// border
		GL_RGB,					// format of original data
		GL_UNSIGNED_BYTE,		// type of data
		image_sun->pixels);		// pointer to the start of data

	//Terra
	Image* image = loadBMP("assets/earthmap1k.bmp");
	glGenTextures(1, &texture_earth);
	glBindTexture(GL_TEXTURE_2D, texture_earth);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, // target
		0,						// level = 0 base, no mipmap
		GL_RGB,					// how the data will be stored
		image->width,			// width of the image
		image->height,			// height of the image
		0,						// border
		GL_RGB,					// format of original data
		GL_UNSIGNED_BYTE,		// type of data
		image->pixels);			// pointer to the start of data
	
	//Lluna
	Image* image2 = loadBMP("assets/moonmap1k.bmp");
	glGenTextures(1, &texture_moon);
	glBindTexture(GL_TEXTURE_2D, texture_moon);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, // target
		0,						// level = 0 base, no mipmap
		GL_RGB,					// how the data will be stored
		image2->width,			// width of the image
		image2->height,			// height of the image
		0,						//border
		GL_RGB,					// format of original data
		GL_UNSIGNED_BYTE,		// type of data
		image2->pixels);		// pointer to the start of data
	
	Image* image_clouds = loadBMP("assets/clouds.bmp");
	glGenTextures(1, &texture_clouds);
	glBindTexture(GL_TEXTURE_2D, texture_clouds);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, // target
		0,						// level = 0 base, no mipmap
		GL_RGB,					// how the data will be stored
		image_clouds->width,	// width of the image
		image_clouds->height,	// height of the image
		0,						// border
		GL_RGB,					// format of original data
		GL_UNSIGNED_BYTE,		// type of data
		image_clouds->pixels);	// pointer to the start of data
	
	
	Image* image_mercury = loadBMP("assets/mercury.bmp");
	glGenTextures(1, &texture_mercury);
	glBindTexture(GL_TEXTURE_2D, texture_mercury);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,	// target
		0,						// level = 0 base, no mipmap
		GL_RGB,					// how the data will be stored
		image_mercury->width,	// width of the image
		image_mercury->height,	// height of the image
		0,						//border
		GL_RGB,					// format of original data
		GL_UNSIGNED_BYTE,		// type of data
		image_mercury->pixels);	// pointer to the start of data

	Image* image_saturn = loadBMP("assets/saturn.bmp");
	glGenTextures(1, &texture_saturn);
	glBindTexture(GL_TEXTURE_2D, texture_saturn);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,	// target
		0,						// level = 0 base, no mipmap
		GL_RGB,					// how the data will be stored
		image_saturn->width,	// width of the image
		image_saturn->height,	// height of the image
		0,						//border
		GL_RGB,					// format of original data
		GL_UNSIGNED_BYTE,		// type of data
		image_saturn->pixels);	// pointer to the start of data
	
	Image* image_uranus = loadBMP("assets/uranus.bmp");
	glGenTextures(1, &texture_uranus);
	glBindTexture(GL_TEXTURE_2D, texture_uranus);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,	// target
		0,						// level = 0 base, no mipmap
		GL_RGB,					// how the data will be stored
		image_uranus->width,	// width of the image
		image_uranus->height,	// height of the image
		0,						//border
		GL_RGB,					// format of original data
		GL_UNSIGNED_BYTE,		// type of data
		image_uranus->pixels);	// pointer to the start of data

	Image* image_neptune = loadBMP("assets/neptune.bmp");
	glGenTextures(1, &texture_neptune);
	glBindTexture(GL_TEXTURE_2D, texture_neptune);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,	// target
		0,						// level = 0 base, no mipmap
		GL_RGB,					// how the data will be stored
		image_neptune->width,	// width of the image
		image_neptune->height,	// height of the image
		0,						//border
		GL_RGB,					// format of original data
		GL_UNSIGNED_BYTE,		// type of data
		image_neptune->pixels);	// pointer to the start of data

	Image* image_star = loadBMP("assets/star.bmp");
	glGenTextures(1, &texture_star);
	glBindTexture(GL_TEXTURE_2D, texture_star);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,	// target
		0,						// level = 0 base, no mipmap
		GL_RGB,					// how the data will be stored
		image_star->width,		// width of the image
		image_star->height,		// height of the image
		0,						//border
		GL_RGB,					// format of original data
		GL_UNSIGNED_BYTE,		// type of data
		image_star->pixels);	// pointer to the start of data
	
	bool ret = tinyobj::LoadObj(shapes, filename);
	bool ret1 = tinyobj::LoadObj(shapes_star, filename1);
	bool ret2 = tinyobj::LoadObj(shapes_plane, filename2);

	if (ret && ret1 && ret2) {
		cout << "OBJ File: " << filename << " succesfully loaded\n";
	}
	else {
		cout << "OBJ File: " << filename << " cannot be found or is not a valid OBJ\n";
	}

 	//**********************
	// CODE TO LOAD EVERYTHING INTO MEMORY
	//**********************

	//load the shader
	Shader simpleShader("src/shader.vert", "src/shader.frag");
	g_simpleShader = simpleShader.program;

	// Create the VAO where we store all geometry (stored in g_Vao)
	g_Vao[0] = gl_createAndBindVAO();

	//SPHERE-----------------------------------------------------------------
	//create vertex buffer for positions, colors, and indices, and bind them to shader
	gl_createAndBindAttribute(&(shapes[0].mesh.positions[0]), sizeof(float) * shapes[0].mesh.positions.size(), g_simpleShader, "a_vertex", 3);
	gl_createAndBindAttribute(&(shapes[0].mesh.texcoords[0]), shapes[0].mesh.texcoords.size() * sizeof(GLfloat), g_simpleShader, "a_uv", 2);
	gl_createIndexBuffer(&(shapes[0].mesh.indices[0]), sizeof(unsigned int) * shapes[0].mesh.indices.size());
	gl_createAndBindAttribute(&(shapes[0].mesh.normals[0]), shapes[0].mesh.normals.size() * sizeof(float), g_simpleShader, "a_normal", 3);

	//unbind everything
	gl_unbindVAO();


	//STAR-----------------------------------------------------------------
	g_Vao[1] = gl_createAndBindVAO();

	//create vertex buffer for positions, colors, and indices, and bind them to shader
	gl_createAndBindAttribute(&(shapes_star[0].mesh.positions[0]), sizeof(float)* shapes_star[0].mesh.positions.size(), g_simpleShader, "a_vertex", 3);
	gl_createAndBindAttribute(&(shapes_star[0].mesh.texcoords[0]), shapes_star[0].mesh.texcoords.size() * sizeof(GLfloat), g_simpleShader, "a_uv", 2);
	gl_createIndexBuffer(&(shapes_star[0].mesh.indices[0]), sizeof(unsigned int)* shapes_star[0].mesh.indices.size());
	gl_createAndBindAttribute(&(shapes_star[0].mesh.normals[0]), shapes_star[0].mesh.normals.size() * sizeof(float), g_simpleShader, "a_normal", 3);
	
	//unbind everything
	gl_unbindVAO();


	//PLANE-----------------------------------------------------------------
	g_Vao[2] = gl_createAndBindVAO();

	//create vertex buffer for positions, colors, and indices, and bind them to shader
	gl_createAndBindAttribute(&(shapes_plane[0].mesh.positions[0]), sizeof(float) * shapes_plane[0].mesh.positions.size(), g_simpleShader, "a_vertex", 3);
	gl_createAndBindAttribute(&(shapes_plane[0].mesh.texcoords[0]), shapes_plane[0].mesh.texcoords.size() * sizeof(GLfloat), g_simpleShader, "a_uv", 2);
	gl_createIndexBuffer(&(shapes_plane[0].mesh.indices[0]), sizeof(unsigned int) * shapes_plane[0].mesh.indices.size());
	gl_createAndBindAttribute(&(shapes_plane[0].mesh.normals[0]), shapes_plane[0].mesh.normals.size() * sizeof(float), g_simpleShader, "a_normal", 3);
	
	//Unbind everything
	gl_unbindVAO();

	//Store number of triangles (use in draw())
	g_NumTriangles = shapes[0].mesh.indices.size() / 3;
	g_NumTriangles_star = shapes_star[0].mesh.indices.size() / 3;
	g_NumTriangles_plane = shapes_plane[0].mesh.indices.size() / 3;

	//Generar SKYBOX-----------------------------------------------------------------
	Shader simpleShader_sky("src/shader_sky.vert", "src/shader_sky.frag");
		g_simpleShader_sky = simpleShader_sky.program;
		bool ret_sky = tinyobj::LoadObj(shapes_sky, filename);

		g_Vao_sky = gl_createAndBindVAO();
		gl_createAndBindAttribute(&(shapes_sky[0].mesh.positions[0]),
		shapes_sky[0].mesh.positions.size() * sizeof(float), g_simpleShader_sky,"a_vertex", 3);
		gl_createIndexBuffer(&(shapes_sky[0].mesh.indices[0]),
		shapes_sky[0].mesh.indices.size() * sizeof(unsigned int));
		gl_createAndBindAttribute(&(shapes_sky[0].mesh.texcoords[0]),
		shapes_sky[0].mesh.texcoords.size() * sizeof(GLfloat), g_simpleShader_sky,"a_uv", 2);
		gl_unbindVAO();
		g_NumTriangles_sky = shapes_sky[0].mesh.indices.size() / 3;

	Image* image_sky = loadBMP("assets/milkyway.bmp");
	glGenTextures(1, &texture_id_sky);
	glBindTexture(GL_TEXTURE_2D, texture_id_sky);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_sky->width, image_sky->height, 0,GL_RGB, GL_UNSIGNED_BYTE, image_sky->pixels);

}

// ------------------------------------------------------------------------------------------
// This function actually draws to screen and called non-stop, in a loop
// ------------------------------------------------------------------------------------------
void draw()
{	
	//Implementar SKYBOX (perspectiva i moviment)---------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glUseProgram(g_simpleShader_sky);
	gl_bindVAO(g_Vao_sky);

	GLuint u_model = glGetUniformLocation(g_simpleShader_sky, "u_model");
	GLuint u_view = glGetUniformLocation(g_simpleShader_sky, "u_view");
	GLuint u_projection = glGetUniformLocation(g_simpleShader_sky, "u_projection");

	//Set MVP
	mat4 model_matrix = translate(mat4(1.0f), cameraPosit);
	mat4 view_matrix = lookAt(cameraPosit, cameraCenter, vec3(0, 1, 0));
	mat4 projection_matrix = glm::perspective(120.0f, (float)(g_ViewportWidth / g_ViewportHeight), 0.1f, 50.0f);

	//Send all values to shader
	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	GLuint u_texture_sky = glGetUniformLocation(g_simpleShader_sky, "u_texture");
	glUniform1i(u_texture_sky, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_sky);
	gl_bindVAO(g_Vao_sky);
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_sky, GL_UNSIGNED_INT, 0);

	//Actualitzar informació al shader.frag----------------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(g_simpleShader);

	GLuint light_loc = glGetUniformLocation(g_simpleShader, "u_light_dir");
	glUniform3f(light_loc, g_light_dir.x, g_light_dir.y, g_light_dir.z);

	GLuint cam_pos_loc = glGetUniformLocation(g_simpleShader, "u_cam_pos");
	// this is the camera position, eye/cameraPos
	glUniform3f(cam_pos_loc, cameraPos.x, cameraPos.y, cameraPos.z);

	GLuint ambient_loc = glGetUniformLocation(g_simpleShader, "u_ambient");
	glUniform3f(ambient_loc, 0.1, 0.1, 0.1); // grey shadows

	GLuint diffuse_loc = glGetUniformLocation(g_simpleShader, "u_diffuse");
	glUniform3f(diffuse_loc, 1.0, 1.0, 1.0); // white light

	GLuint specular_loc = glGetUniformLocation(g_simpleShader, "u_specular");
	glUniform3f(specular_loc, 1.0, 1.0, 1.0);

	GLuint shininess_loc = glGetUniformLocation(g_simpleShader, "u_shininess");
	glUniform1f(shininess_loc, 20.0);
	
	//Rotació dels planetes
	rota2 += 0.005;
	rota1 += 0.01;
	rota_merc += 0.06;
	rota_sat += 0.03;
	rota_ura += 0.05;
	rota_nept += 0.04;

	if (rota1 == 365) {
		rota1 = 0;
	}
	if (rota2 == 365) {
		rota2 = 0;
	}
	if (rota_merc == 365) {
		rota_merc = 0;
	}
	if (rota_sat == 365) {
		rota_sat = 0;
	}
	if (rota_ura == 365) {
		rota_ura = 0;
	}
	if (rota_nept == 365) {
		rota_nept = 0;
	}

	//Activate shader
	GLuint colorLoc = glGetUniformLocation(g_simpleShader, "u_color");
	GLuint model_loc = glGetUniformLocation(g_simpleShader, "u_model");
	GLuint projection_loc = glGetUniformLocation(g_simpleShader, "u_projection");
	GLuint view_loc = glGetUniformLocation(g_simpleShader, "u_view");
	
	//Set perspective
	mat4 projection_matrix1 = perspective(
		90.0f,	//field of view
		1.0f,	//Aspect ratio
		0.1f,	//Near plane (distance)
		50.0f	//far plane  (distance)
	);

	view_matrix = glm::lookAt(
		cameraPosit,
		cameraPosit + cameraFront,
		cameraUp
	);
	
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix1));

	//començem dibuixant les esferes.
	gl_bindVAO(g_Vao[0]);
	
	//--------------------------------------------------------------------------
	//SUN
	mat4 sun = translate(mat4(1.0f), vec3(0.0f, 0.0f, -2.0f));
	sun = rotate(sun, rota2, vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(sun));

	GLuint sunny = glGetUniformLocation(g_simpleShader, "sun");
	GLuint earth = glGetUniformLocation(g_simpleShader, "earth");
	GLuint u_alpha = glGetUniformLocation(g_simpleShader, "u_alpha");
	
	glUniform1i(sunny, true);
	glUniform1i(earth, false);
	glUniform1f(u_alpha, 1.0);

	GLuint u_texture_sun = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texture_sun, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_sun);
	
	//draw to screen.
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);
	

	//--------------------------------------------------------------------------
	//MERCURY
	mat4 mercury = translate(mat4(1.0f), vec3(0.0f, 0.0f, -2.0f));
	mercury = rotate(mercury, rota_merc, vec3(0.0f, 1.0f, 0.0f));
	mercury = translate(mercury, vec3(0.0f, 0.0f, -1.3f));
	mercury = mercury * sun;
	mercury = scale(mercury, vec3(0.25f, 0.250f, 0.250f));

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(mercury));
	
	glUniform1i(sunny, false);

	GLuint u_texture_mercury = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texture_mercury, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_mercury);

	//draw to screen.
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);


	//--------------------------------------------------------------------------
	//TERRA:
	mat4 model = translate(mat4(1.0f), vec3(0.0f, 0.0f, -4.0f));
	model = rotate(model, rota1, vec3(0.0f, 1.0f, 0.0f));
	model = translate(model, vec3(0.0f, 0.0f, -5.0f));
	model = model * sun;
	model = scale(model, vec3(0.5f, 0.50f, 0.50f));
	
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

	//És la terra
	glUniform1i(earth, true);

	//textura base
	GLuint u_texture = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_earth);
	
	//textura relieve
	GLuint u_texture_normal = glGetUniformLocation(g_simpleShader, "u_texture_normal");
	glUniform1i(u_texture_normal, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_id_earth_normal);

	//textura mar
	GLuint u_texture_spec = glGetUniformLocation(g_simpleShader, "u_texture_spec");
	glUniform1i(u_texture_spec, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture_id_earth_spec);

	//Draw to screen
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);


	//--------------------------------------------------------------------------
	//LLUNA
	mat4 model1 = rotate(mat4(1.0f), rota2, vec3(0.0f, 1.0f, 0.0f));
	model1 = translate(model1, vec3(3.0f, 1.0f, 0.0f));
	model1 = model * model1;
	model1 = scale(model1, vec3(0.50f, 0.50f, 0.50f));

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model1));

	//No és la terra
	glUniform1i(earth, false);

	//textura base
	GLuint u_texture2 = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texture2, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_moon);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);


	//--------------------------------------------------------------------------
	//CLOUDS
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	mat4 clouds = model;
	clouds = scale(clouds, vec3(1.1f, 1.1f, 1.1f));
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(clouds));

	GLuint cloud = glGetUniformLocation(g_simpleShader, "clouds");

	//Verifica que siguin els núvols i transparents
	glUniform1i(cloud, true);
	glUniform1f(u_alpha, 0.5);

	//textura base 
	GLuint u_texture_clouds = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texture_clouds, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_clouds);
	
	//textura transparent
	GLuint u_texture_trans = glGetUniformLocation(g_simpleShader, "u_texture_trans");
	glUniform1i(u_texture_trans, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_id_clouds_trans);

	//Draw to screen 
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);


	
	//--------------------------------------------------------------------------
	//SATURN
	mat4 saturn = translate(mat4(1.0f), vec3(0.0f, 0.0f, -2.0f));
	saturn = rotate(saturn, rota_sat, vec3(0.0f, 1.0f, 0.0f));
	saturn = translate(saturn, vec3(0.0f, 0.0f, -10.0f));
	saturn = saturn * sun;
	saturn = scale(saturn, vec3(1.3f, 1.3f, 1.3f));

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(saturn));

	glUniform1i(cloud, false);
	glUniform1f(u_alpha, 1.0);

	//textura base
	GLuint u_texture_saturn = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texture_saturn, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_saturn);

	//draw to screen.
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);

	

	//--------------------------------------------------------------------------
	//URANUS
	mat4 uranus = translate(mat4(1.0f), vec3(0.0f, 0.0f, -2.0f));
	uranus = rotate(uranus, rota_ura, vec3(0.0f, 1.0f, 0.0f));
	uranus = translate(uranus, vec3(0.0f, 0.0f, -15.0f));
	uranus = uranus * sun;
	uranus = scale(uranus, vec3(1.3f, 1.3f, 1.3f));

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(uranus));

	//textura base
	GLuint u_texture_uranus = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texture_uranus, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_uranus);

	//draw to screen.
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);



	//--------------------------------------------------------------------------
	//NEPTUNE
	mat4 neptune = translate(mat4(1.0f), vec3(0.0f, 0.0f, -2.0f));
	neptune = rotate(neptune, rota_nept, vec3(0.0f, 1.0f, 0.0f));
	neptune = translate(neptune, vec3(0.0f, 0.0f, -20.0f));
	neptune = neptune * sun;
	neptune = scale(neptune, vec3(1.3f, 1.3f, 1.3f));

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(neptune));

	GLuint u_texture_neptune = glGetUniformLocation(g_simpleShader, "u_texture");
	// bind the sampler to the texture unit 0
	glUniform1i(u_texture_neptune, 0);
	// activate texture unit 0 and bin the texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_neptune);

	//draw to screen.
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);



	//--------------------------------------------------------------------------
	//RINGS
	gl_bindVAO(g_Vao[2]); //És un plà

	mat4 rings = translate(mat4(1.0f), vec3(0.0f, 0.0f, -2.0f));
	rings = rotate(rings, rota_sat, vec3(0.0f, 1.0f, 0.0f));
	rings = translate(rings, vec3(0.0f, 0.0f, -10.0f));
	rings = rings * sun;
	rings = scale(rings, vec3(5.5f, 5.5f, 5.5f));

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(rings));
	GLuint ring = glGetUniformLocation(g_simpleShader, "rings");

	//indiquem que son els anells
	glUniform1i(ring, true);
	
	//textura_base
	GLuint u_texture_rings = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texture_rings, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_rings);

	//draw to screen.
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);


	
	//--------------------------------------------------------------------------
	//STAR
	gl_bindVAO(g_Vao[1]); //És la estrella 
	mat4 star = translate(mat4(1.0f), vec3(0.0f, 3.0f, -2.0f));
	star = rotate(star, rota1, vec3(0.0f, 1.0f, 0.0f));
	star = scale(star, vec3(0.3f, 0.3f, 0.3f));

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(star));
	
	//indica que no son els anells i que té certa transparencia
	glUniform1i(ring, false);
	glUniform1f(u_alpha, 0.5);
	
	GLuint u_texture_star = glGetUniformLocation(g_simpleShader, "u_texture");
	glUniform1i(u_texture_star, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_star);

	//draw to screen.
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_star, GL_UNSIGNED_INT, 0);

}

// ------------------------------------------------------------------------------------------
// This function is called every time you press a screen
// ------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	const float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPosit += cameraSpeed * cameraFront;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPosit -= cameraSpeed * cameraFront;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPosit -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPosit += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	//quit
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
	//reload
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		load();

}

// ------------------------------------------------------------------------------------------
// This function is called every time you click the mouse
// ------------------------------------------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        cout << "Left mouse down at" << mouse_x << ", " << mouse_y << endl;
    }
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		cout << "Left mouse down at" << mouse_x << ", " << mouse_y << endl;
	}
}
// ------------------------------------------------------------------------------------------
// This function moves the camera in the same direction the mouse does
// ------------------------------------------------------------------------------------------
void mause_callback(GLFWwindow* window, double x, double y) {
	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}
	float xoffset = x - lastX;
	float yoffset = lastY - y;
	lastX = x;
	lastY = y;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	_yaw += xoffset;
	_pitch += yoffset;

	if (_pitch > 180.0f)
		_pitch = 180.0f;
	if (_pitch < -180.0f)
		_pitch = -180.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	direction.y = sin(glm::radians(_pitch));
	direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	cameraFront = glm::normalize(direction);
}


int main(void)
{	
	if (!glfwInit())return -1;
	//setup window and boring stuff, defined in glfunctions.cpp
	GLFWwindow* window;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(g_ViewportWidth, g_ViewportHeight, "Hello OpenGL!", NULL, NULL);
	if (!window) {glfwTerminate();	return -1;}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();


	//input callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mause_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	
	

	load();
    // Loop until the user closes the window
	while (!glfwWindowShouldClose(window))//&& !glfwWindowShouldClose(window2))
	{
		draw();
		
		// Swap front and back buffers
		glfwSwapBuffers(window);
		// Poll for and process events
		glfwPollEvents();
		//mouse position must be tracked constantly (callbacks do not give accurate delta)
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
	}
	
	//-------------FRAME BUFFER------------
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);


	unsigned int framebufferTexture;
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_ViewportWidth, g_ViewportHeight, 0, GL_UNSIGNED_BYTE, NULL,NULL);
	
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,framebufferTexture, NULL);
	
	//-------------BLOOM TEXTURE------------
	unsigned int bloomTexture;
	glGenTextures(1, &bloomTexture);
	glBindTexture(GL_TEXTURE_2D, bloomTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, g_ViewportWidth, g_ViewportHeight, 0, GL_UNSIGNED_BYTE, NULL, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bloomTexture, NULL);
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	//Eliminar la finestra quan ha acabat el programa
	glfwDestroyWindow(window);
	glfwTerminate();
    
    return 0;
}