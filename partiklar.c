// OpenGL 3 conversion 2013.
//gcc partiklar.c common/*.c common/Linux/MicroGlut.c -lGL -lX11 -lm -o partiklar -I common -I common/Linux

#include <stdlib.h>


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

mat4 projectionMatrix;
mat4 viewMatrix;
GLfloat ratio;

GLuint* drawShader, vertexArray, vertexBuffer;


GLfloat vertices[] = {
	-1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f
};

// Drawing routine
void Display()
{

	// Clear framebuffer & zbuffer
	glClearColor(0.1, 0.1, 0.3, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Enable Z-buffering
	glEnable(GL_DEPTH_TEST);
	// Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glUseProgram(drawShader);
	viewMatrix = lookAt(0,0,2, 0,0,0, 0,1,0);
	glUniformMatrix4fv(glGetUniformLocation(drawShader, "modelViewMatrix"), 1, GL_TRUE, viewMatrix.m);
	
	glBindVertexArray(vertexArray);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);	

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
	
	
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(glGetAttribLocation(drawShader, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
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
	glutInitWindowSize(600, 600);
	
	glutInitContextVersion(3, 2);
	glutCreateWindow("Struta Hårt");

	glutDisplayFunc(Display);

	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	
	Init();
	
	glutMainLoop();
	exit(0);
}




