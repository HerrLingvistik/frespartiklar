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


static GLfloat texStart[W][H][4];
//static GLuint texName;

GLuint drawShader, createShader, vertArray, vertBuffer;

FBOstruct *fbo1, *fbo2;

GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,

		1.0f,-1.0f,0.0f,
		1.0f,1.0f,0.0f,


     -1.0f,  1.0f, 0.0f,

     -1.0f, -1.0f, 0.0f,

 		1.0f, 1.0f, 0.0f
//         ÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖJJ. Schyssta LÖKAR!!!
};

FBOstruct *initFBOTex(){

	//static GLfloat texStart[W][H][4];
	//static GLuint texName;
	
	int i; int j;
	for(i = 0; i < W; i++){
		for(j = 0; j < H; j++){
			//printf("\n%i", (int)(rand()/(float)RAND_MAX*255.0f));
			texStart[i][j][0] = (int)(rand()/(float)RAND_MAX*255.0f);
			texStart[i][j][1] = (int)(rand()/(float)RAND_MAX*255.0f);

			texStart[i][j][2] = ((float)rand()/(float)RAND_MAX)*2.0f - 1.0f;
			texStart[i][j][3] = ((float)rand()/(float)RAND_MAX)*2.0f - 1.0f;
		}
	}

	//--------------------------------

	FBOstruct *fbo = malloc(sizeof(FBOstruct));

	fbo->width = W;
	fbo->height = H;

	// create objects
	glGenFramebuffers(1, &fbo->fb); // frame buffer id
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->fb);
	glGenTextures(1, &fbo->texid);
	fprintf(stderr, "%i \n",fbo->texid);
	glBindTexture(GL_TEXTURE_2D, fbo->texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, texStart);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo->texid, 0);

	// Renderbuffer
	// initialize depth renderbuffer
    glGenRenderbuffers(1, &fbo->rb);
    glBindRenderbuffer(GL_RENDERBUFFER, fbo->rb);
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, fbo->width, fbo->height );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->rb );
    CHECK_FRAMEBUFFER_STATUS();

	fprintf(stderr, "Framebuffer object %d\n", fbo->fb);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//--------------------------------
	return fbo;
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
	//glUniform1i(glGetUniformLocation(drawShader, "texUnit"), 0);


	glBindVertexArray(vertArray);						//OBS DETTA SKA ANVÄNDAS. ANNARS BLIR DET SÄMST!!
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//första fbo
	/*
	useFBO(0L, fbo1, 0L);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	*/

	//koppla shader - skapa partiklar

	//sätta position

	glFlush();
	
	glutSwapBuffers();
}


void Init()
{

	glClearColor(0.1, 0.1, 0.3, 0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	drawShader = loadShaders("draw.vert", "draw.frag");
	printError("init shaders");

	fbo1 = initFBOTex();
	//fbo2 = initFBO(W, H, 0);

	glUseProgram(drawShader);
	//viewMatrix = lookAt(0,0,2, 0,0,0, 0,1,0);
	projectionMatrix = perspective(90, 1.0, 0.1, 1000); // It would be silly to upload an uninitialized matrix
	glUniformMatrix4fv(glGetUniformLocation(drawShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(drawShader, "modelViewMatrix"), 1, GL_TRUE, viewMatrix.m);




	glGenVertexArrays(1, &vertArray);
	glBindVertexArray(vertArray);
	glGenBuffers(1, &vertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);	
 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(glGetAttribLocation(drawShader, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);

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




