	// OpenGL 3 conversion 2013.
//gcc partiklar.c common/*.c common/Linux/MicroGlut.c -lGL -lX11 -lm -o partiklar -I common -I common/Linux

//http://nullprogram.com/blog/2014/06/29/

#include <stdlib.h>

//Sebbe gör något jefligt konstigt

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifdef __APPLE__
// Mac
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#ifdef WIN32
// MS
		#include <windows.h>
		#include <stdio.h>
		#include <GL/glew.h>
		#include <GL/glut.h>
	#else
// Linux
		#include <stdio.h>
		#include <GL/gl.h>
		#include "MicroGlut.h"
//		#include <GL/glut.h>
	#endif
#endif

#include "VectorUtils3.h"
#include "GL_utilities.h"
#include "loadobj.h"
#include "zpr.h"

#define W 256
#define H 256

mat4 projectionMatrix;
mat4 viewMatrix;
GLfloat ratio;

//GLuint* drawShader, createShader, vertexArray, vertexBuffer;

//Osäker på om dessa ska deklareras som pointers eller ej
GLuint drawShader, createShader, vertexArray, vertexBuffer;

FBOstruct *fbo1, *fbo2;

GLfloat vertices[] = {
	-1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f
};

void initTextures(){
	
}

// Drawing routine
void Display()
{

	// Clear framebuffer & zbuffer
	glClearColor(0.1, 0.1, 0.3, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);
	// Enable backface culling
	glEnable(GL_CULL_FACE);
	// Enable Z-buffering
	glCullFace(GL_BACK);

	
  
	glUseProgram(drawShader);
	viewMatrix = lookAt(0,0,2, 0,0,0, 0,1,0);
	glUniformMatrix4fv(glGetUniformLocation(drawShader, "modelViewMatrix"), 1, GL_TRUE, viewMatrix.m);

	//första fbo
	
	

	//koppla shader - skapa partiklar

	//sätta position

	glFlush();
	
	glutSwapBuffers();
}


void Init()
{



	drawShader = loadShaders("draw.vert", "draw.frag");
	printError("init shaders");
	glUseProgram(drawShader);
	//viewMatrix = lookAt(0,0,2, 0,0,0, 0,1,0);
	projectionMatrix = perspective(90, 1.0, 0.1, 1000); // It would be silly to upload an uninitialized matrix
	glUniformMatrix4fv(glGetUniformLocation(drawShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(drawShader, "modelViewMatrix"), 1, GL_TRUE, viewMatrix.m);

	createShader = loadShaders("createParts.vert", "createParts.frag");
	printError("init shaders");
	
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(glGetAttribLocation(drawShader, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);

	//fixa dose frame buffers
	fbo1 = initFBO(W, H, 0);
	fbo2 = initFBO(W, H, 0);

	initTextures();

}

void Reshape(int h, int w)
{
	glViewport(0, 0, w, h);
	ratio = (GLfloat) w / (GLfloat) h;
	projectionMatrix = perspective(90, ratio, 0.1, 1000);
	glUniformMatrix4fv(glGetUniformLocation(drawShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m); 
}


void Idle()
{
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(W,H);
	
	glutInitContextVersion(3, 2);
	glutCreateWindow("Struta Hårt");

	glutDisplayFunc(Display);

	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	
	Init();
	
	glutMainLoop();
	exit(0);
}




