/*
 * Author: Dissegna Stefano
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <GL/glut.h>

#include "sun.h"

void on_resize(int x, int y);
void draw_scene();
void on_key(unsigned char k, int x, int y);
void on_idle();
void quit(unsigned char k, int x, int y);
void left(unsigned char k, int x, int y);
void right(unsigned char k, int x, int y);
void up(unsigned char k, int x, int y);
void down(unsigned char k, int x, int y);

typedef void (*key_callback_t)(unsigned char k, int x, int y);

static key_callback_t callbacks[UCHAR_MAX];

static sun_t sun;

int main(int argc, char **argv)
{
  int i;

  glutInit(&argc, argv);
	
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Escape");
	
  glutReshapeFunc(&on_resize);
  glutDisplayFunc(&draw_scene);
  glutKeyboardFunc(&on_key);
  glutIdleFunc(&on_idle);

  for (i = 0; i < UCHAR_MAX; i++)
    callbacks[i] = NULL;

  callbacks['q'] = &quit;
  callbacks['s'] = &left;
  callbacks['x'] = &right;
  callbacks['a'] = &up;
  callbacks['z'] = &down;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  
  GLfloat black[4] = { 0, 0, 0, 1 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

  sun_init(&sun);

  glutMainLoop();

  return 0;
}

void on_resize(int w, int h)
{
  glViewport(0, 0, w, h);
}

float RY = 0.0;
float RX = 0.0;

void draw_scene()
{
  // the sky is blue
  glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1, 1, -1, 1, 2, 200);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  let_there_be_light(&sun);

  // take user position and translate
  // ...

  // example cube
  glTranslatef(0.0, 0.0, -4);
  glRotatef(RY, 0.0, 1.0, 0.0);
  glRotatef(RX, 1.0, 0.0, 0.0);
  
  GLfloat ambiente[4] = { 1.0, 0.0, 0.0, 1 };
  GLfloat direttiva[4] = { 1.0, 0.0, 0.0, 1 };
  GLfloat brillante[4] = { 0, 0, 0, 1 };
    
  glMateriali(GL_FRONT, GL_SHININESS, 32);
        
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambiente);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, direttiva);
  glMaterialfv(GL_FRONT, GL_SPECULAR, brillante);
  
  glutSolidCube(1);

  glutSwapBuffers();
}

void on_key(unsigned char k, int x, int y)
{
  key_callback_t f = callbacks[k];
  if (f) 
    {
      (*f)(k, x, y);
      glutPostRedisplay();
    }
}

void on_idle() 
{
}

void quit(unsigned char k, int x, int y) 
{
  exit(0);
}

void left(unsigned char k, int x, int y)
{
  RY -= 10;
}

void right(unsigned char k, int x, int y)
{
  RY += 10;
}

void up(unsigned char k, int x, int y)
{
  RX += 10;
}

void down(unsigned char k, int x, int y)
{
  RX -= 10;
}
