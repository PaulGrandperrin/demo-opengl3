/*
ming++ main.cpp graphicEngine.cpp glew.c -o 3D.exe -L/media/Vista/Program\ Files/CodeBlocks/MinGW/lib/ -I/media/Vista/Program\ Files/CodeBlocks/MinGW/include/ -L/media/Vista/Program\ Files/CodeBlocks/SDL-1.2.13/lib/ -I/media/Vista/Program\ Files/CodeBlocks/SDL-1.2.13/include/ -lSDLmain -lSDL.dll -lopengl32 -lDevIL -w -DGLEW_STATIC
*/
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <math.h>
#define PI 3.141592653589793238462

#include "graphicEngine.hpp"

using namespace std;


int main(int argc, char *argv[])
{
	GraphicEngine Gengine(1280,800,false);
	GEint mesh;
	Gengine.loadMesh(&mesh,"meshes/demon.obj");
	unsigned int programId=Gengine.loadProgram("shaders/vs.glsl",NULL,"shaders/ps.glsl");
	Gengine.setShowCursor(false);


	float last_time=0,time=0;
	SDL_Event event;
	bool quit=false;
	float a=0,b=0,c=0,s=1,l=0,v=0;

	SDL_EnableKeyRepeat(10, 10);

	int nbFrame=0;
	char title[100];

	while( !quit)
	{
		SDL_PollEvent(&event);
		Gengine.setCameraPosition(-sin(v)*3,0,-cos(v)*3,0,0,-v);
		Gengine.setMeshAttribute(programId,0,-0.2,0,a,b,c,s,s,s);
		Gengine.setLightDir(sin(l),0,cos(l));
		Gengine.render(0);

		//calcul du nombre de fps
		time=SDL_GetTicks();
		nbFrame++;
		if(time-last_time>=1000.0)
		{
			sprintf(title,"%i fps",int(nbFrame));
			Gengine.setWindowTitle(title);
			last_time=time;
			nbFrame=0;
		}


		switch(event.type)
		{
			case SDL_QUIT:
				quit=true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit=true;
						break;
					case SDLK_LEFT:
						b+=0.1;
						break;
					case SDLK_RIGHT:
						b-=0.1;
						break;
					case SDLK_UP:
						a+=0.1;
						break;
					case SDLK_DOWN:
						a-=0.1;
						break;
					case SDLK_w:
						c+=0.1;
						break;
					case SDLK_x:
						c-=0.1;
						break;
					case SDLK_q:
						l+=0.1;
						break;
					case SDLK_s:
						l-=0.1;
						break;
					case SDLK_a:
						v+=0.1;
						break;
					case SDLK_z:
						v-=0.1;
						break;
					case SDLK_KP_PLUS:
						s*=1.05;
						break;
					case SDLK_KP_MINUS:
						s/=1.05;
						break;
					default:break;
				}
				break;
					case SDL_KEYUP:
						switch(event.key.keysym.sym)
						{
							default:break;
						}
						break;
							default:break;
		}
	}


	return 0;


}
/*

ModelViewMatrixId = glGetUniformLocation(program, "ModelViewMatrix"); //OK
	ProjectionMatrixId = glGetUniformLocation(program, "ProjectionMatrix"); //OK
	NormalMatrixId = glGetUniformLocation(program, "NormalMatrix"); //OK





	//On bind les attributs de vertex

	GLuint positionId = glGetAttribLocation (program, "VertexPosition") ;
	GLuint normalId = glGetAttribLocation (program, "VertexNormal") ;
	glBindAttribLocation(program,0,"VertexPosition");
	glBindAttribLocation(program,1,"VertexNormal");


	//fin de bindage

	//VAO

	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);


	//on stock
	//glBindTexture(GL_TEXTURE_2D,textId1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	//glVertexPointer(3, GL_DOUBLE, 0, BUFFER_OFFSET(0));
	//glNormalPointer(GL_DOUBLE, 0, BUFFER_OFFSET(test.triangles.size()*3*(3*sizeof(double))));
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);

	glEnableVertexAttribArray(0); //position
	glVertexAttribPointer((GLuint)0,3,GL_DOUBLE,GL_FALSE,0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1); //normales
	glVertexAttribPointer((GLuint)1,3,GL_DOUBLE,GL_TRUE,0, BUFFER_OFFSET(test.positions.size()*3*sizeof(double)));



	//On a fini de stocker

	glBindVertexArray(0);

	float a=0,b=0,s=1;


	bool quit=false,shader=false;

	SDL_Event event;
	SDL_EnableKeyRepeat(10,10);

	do
	{
		//  draw(a,b,s,vbo,vao,test.triangles.size(),program);
		SDL_PollEvent(&event);
		a+=0.01;
		}while(event.type!=SDL_QUIT);



		while(SDL_WaitEvent(&event) and !quit)
		{
			switch(event.type)
			{
				case SDL_QUIT:
					quit=true;;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							quit=true;
							break;
						case SDLK_LEFT:
							a+=0.1;
							break;
						case SDLK_RIGHT:
							a-=0.1;
							break;
						case SDLK_UP:
							b+=0.1;
							break;
						case SDLK_DOWN:
							b-=0.1;
							break;
						case SDLK_KP_PLUS:
							s*=1.05;
							break;
						case SDLK_KP_MINUS:
							s/=1.05;
							break;
						default:break;
						}
						break;
						case SDL_KEYUP:
							switch(event.key.keysym.sym)
							{
								case SDLK_s:
									shader=!shader;
									break;
								default:break;
								}
								break;
								default:break;
								}
								//        draw(a,b,s,vbo,vao,test.triangles.size(),program);
								}


								glDeleteProgram(program);
								//    glDeleteBuffers(1, &vbo);

								SDL_Quit();
								return 0;
								}



								void draw(float a,float b,float s,GLuint vbo,GLuint vao,int nb_triangle,GLuint program)
								{


									glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
									//if (!program)
									//   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
									//else
									//   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

									glUseProgram(program);

									float ModelViewMatrix[4][4]=
									{
										{1.0, 0.0, 0.0,0.0},
										{0.0, 1.0, 0.0,0.0},
										{0.0, 0.0, 1.0,0.0},
										{0.0, 0.0, 0.0,1.0}
										};




										// -------------- ICI commencent les probl�mes ----------------------------------------

										float camrotate[3]={0.0,0.0,0.0};
										float camposition[3]={0.0,0.0,-10.0};


										float camrotateX[4][4]=
										{
											{cos(camrotate[0]), sin(camrotate[0]), 0.0,0.0},
											{-sin(camrotate[0]), cos(camrotate[0]), 0.0,0.0},
											{0.0, 0.0, 1.0,0.0},
											{0.0, 0.0, 0.0,1.0}
											};

											float camrotateY[4][4]=
											{
												{cos(camrotate[1]), sin(camrotate[1]), 0.0,0.0},
												{-sin(camrotate[1]), cos(camrotate[1]), 0.0,0.0},
												{0.0, 0.0, 1.0,0.0},
												{0.0, 0.0, 0.0,1.0}
												};

												float camrotateZ[4][4]=
												{
													{cos(camrotate[2]), sin(camrotate[2]), 0.0,0.0},
													{-sin(camrotate[2]), cos(camrotate[2]), 0.0,0.0},
													{0.0, 0.0, 1.0,0.0},
													{0.0, 0.0, 0.0,1.0}
													};

													float camtranslation[4][4]=
													{
														{1.0, 0.0, 0.0,0.0},
														{0.0, 1.0, 0.0,0.0},
														{0.0, 0.0, 1.0,0.0},
														{camposition[0], camposition[1], camposition[2],1.0}
														};


														// ------------ ICI finissent les probl�mes ------------------------------------

														float rotationy[4][4]=
														{
															{cos(a), sin(a), 0.0,0.0},
															{-sin(a), cos(a), 0.0,0.0},
															{0.0, 0.0, 1.0,0.0},
															{0.0, 0.0, 0.0,1.0}
															};


															float rotationx[4][4]=
															{
																{1.0, 0.0, 0.0,0.0},
																{0.0, cos(b), sin(b),0.0},
																{0.0, -sin(b), cos(b),0.0},
																{0.0, 0.0, 0.0,1.0}
																};

																float rotationz[4][4]=
																{
																	{cos(a), 0.0, -sin(a),0.0},
																	{0.0, 1.0, 0.0,0.0},
																	{sin(a), 0.0, cos(a),0.0},
																	{0.0, 0.0, 0.0,1.0}
																	};

																	float scale[4][4]=
																	{
																		{s, 0.0, 0.0,0.0},
																		{0.0, s, 0.0,0.0},
																		{0.0, 0.0, s,0.0},
																		{0.0, 0.0, 0.0,1.0}
																		};

																		float translation[4][4]=
																		{
																			{1.0, 0.0, 0.0,0.0},
																			{0.0, 1.0, 0.0,0.0},
																			{0.0, 0.0, 1.0,0.0},
																			{0.0, -10.0, 0.0,1.0}
																			};




																			// matmul(CameraMatrix,ModelViewMatrix);
																			//matmul(CameraMatrixEye,ModelViewMatrix);

																			//matmul(scale,ModelViewMatrix);
																			//matmul(rotationy,ModelViewMatrix);
																			//matmul(translation,ModelViewMatrix);

																			//glMatrixMode( GL_PROJECTION );
																			//glLoadIdentity();
																			//glLoadMatrixf((GLfloat*)ProjectionMatrix);
																			//gluPerspective(70,(double)winWidth/winHeight,1,1000);

																			//glMatrixMode(GL_MODELVIEW);
																			//glLoadIdentity();
																			//glLoadMatrixf((GLfloat*)CameraMatrix);

																			matmul(camrotateX,ModelViewMatrix);
																			matmul(camrotateY,ModelViewMatrix);
																			matmul(camrotateZ,ModelViewMatrix);
																			matmul(camtranslation,ModelViewMatrix);
																			//glTranslatef(-eyeX,-eyeY,-eyeZ);
																			//gluLookAt(5.0,5.0,5.0,0.0,0.0,0.0,0.0,0.0,1.0);
																			//glRotatef(b,1,0,0);
																			//glRotatef(0,0,1,0);
																			//glRotatef(a,0,0,1);
																			//glTranslatef(0,0,-10);


																			matmul(rotationx,ModelViewMatrix);
																			matmul(rotationz,ModelViewMatrix);

																			matmul(scale,ModelViewMatrix);

																			matmul(translation,ModelViewMatrix);
																			//glTranslated(2,-1,0.0);

																			//glRotated(90,1,0,0);
																			//glRotated(a,0,1,0);
																			//glRotated(b,0,0,1);

																			//glScaled(s,s,s);

																			float NormalMatrix[3][3];
																			float ModelViewMatrix3x3[3][3]=
																			{
																				{ModelViewMatrix[0][0],ModelViewMatrix[0][1],ModelViewMatrix[0][2]},
																				{ModelViewMatrix[1][0],ModelViewMatrix[1][1],ModelViewMatrix[1][2]},
																				{ModelViewMatrix[2][0],ModelViewMatrix[2][1],ModelViewMatrix[2][2]}
																				};
																				//inverseM3(ModelViewMatrix3x3,NormalMatrix);


																				glUniformMatrix4fv(ModelViewMatrixId, 1, 0, (GLfloat*) ModelViewMatrix);
																				glUniformMatrix4fv(ProjectionMatrixId, 1, 0, (GLfloat*) ProjectionMatrix);
																				glUniformMatrix3fv(NormalMatrixId, 1, 0, (GLfloat*) ModelViewMatrix3x3);


																				//on utilise le vao
																				glBindVertexArray(vao);

																				glDrawElements(GL_TRIANGLES, nb_triangle*3, GL_UNSIGNED_INT,BUFFER_OFFSET(0));

																				glBindVertexArray(0);

																				//glUseProgram(0);
																				glFlush();
																				SDL_GL_SwapBuffers();
																				}


																				*/


/*
glTexCoord2i(1,1);
glNormal3f(0.0,1.0,0.0);
glColor3ub(255,255,255); //face blanche
glVertex3d(1,1,1);
glTexCoord2i(1,0);
glVertex3d(1,1,-1);
glTexCoord2i(0,0);
glVertex3d(-1,1,-1);
glTexCoord2i(0,1);
glVertex3d(-1,1,1);

glNormal3f(1.0,0.0,0.0);
glColor3ub(0,0,255); //bleu
glTexCoord2i(1,1);
glVertex3d(1,-1,1);
glTexCoord2i(1,0);
glVertex3d(1,-1,-1);
glTexCoord2i(0,0);
glVertex3d(1,1,-1);
glTexCoord2i(0,1);
glVertex3d(1,1,1);

glNormal3f(0.0,-1.0,0.0);
glColor3ub(255,0,0); //rouge
glTexCoord2i(1,1);
glVertex3d(-1,-1,1);
glTexCoord2i(1,0);
glVertex3d(-1,-1,-1);
glTexCoord2i(0,0);
glVertex3d(1,-1,-1);
glTexCoord2i(0,1);
glVertex3d(1,-1,1);

glNormal3f(-1.0,0.0,0.0);
glColor3ub(0,255,0); //vert
glTexCoord2i(1,1);
glVertex3d(-1,1,1);
glTexCoord2i(1,0);
glVertex3d(-1,1,-1);
glTexCoord2i(0,0);
glVertex3d(-1,-1,-1);
glTexCoord2i(0,1);
glVertex3d(-1,-1,1);

glNormal3f(0.0,0.0,-1);
glColor3ub(255,255,0); //jaune
glTexCoord2i(1,1);
glVertex3d(1,1,-1);
glTexCoord2i(1,0);
glVertex3d(1,-1,-1);
glTexCoord2i(0,0);
glVertex3d(-1,-1,-1);
glTexCoord2i(0,1);
glVertex3d(-1,1,-1);

glNormal3f(0.0,0.0,1.0);
glColor3ub(0,255,255); //cyan
glTexCoord2i(1,0);
glVertex3d(1,-1,1);
glTexCoord2i(1,1);
glVertex3d(1,1,1);
glTexCoord2i(0,1);
glVertex3d(-1,1,1);
glTexCoord2i(0,0);
glVertex3d(-1,-1,1);
*/

/*
float F[3]=
{   (centerX-eyeX),
(centerY-eyeY),
(centerZ-eyeZ)
};
float FN[3]=
{
	F[0]/norme(F),
	F[1]/norme(F),
	F[2]/norme(F)
	};
	float UP[3]=
	{
		upX,
		upY,
		upZ
		};
		float UPP[3]=
		{
			UP[0]/norme(UP),
			UP[1]/norme(UP),
			UP[2]/norme(UP)
			};
			float SN[3]=
			{
				FN[0]*UPP[0],
				FN[1]*UPP[1],
				FN[2]*UPP[2]
				};
				float UN[3]=
				{
					SN[0]*FN[0],
					SN[1]*FN[1],
					SN[2]*FN[2]
					};

					float CameraMatrix[4][4]=
					{
						//  {SN[0], SN[1], SN[2],0.0},
						// {UN[0], UN[1], UN[2],0.0},
						// {-FN[0], -FN[1], -FN[2],0.0},
						//{0.0, 0.0, 0.0,1.0}

						{SN[0], UN[0], FN[0],0.0},
						{SN[1], UN[1], FN[1],0.0},
						{SN[2], UN[2], FN[2],0.0},
						{0.0, 0.0, 0.0,1.0}
					};

					float CameraMatrixEye[4][4]=
					{
						{1.0, 0.0, 0.0,0.0},
						{0.0, 1.0, 0.0,0.0},
						{0.0, 0.0, 1.0,0.0},
						{-eyeX, -eyeY, -eyeZ,1.0}
					};
					*/

