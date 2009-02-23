/*
 * Author: Dissegna Stefano
 */

#include <stdio.h>
#include <stdlib.h>
#include "GL/glut.h"

void on_resize(int x, int y);
void draw_scene();
void on_key(unsigned char k, int x, int y);
void on_idle();

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
	
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Escape");
	
  glutReshapeFunc(&on_resize);
  glutDisplayFunc(&draw_scene);
  glutKeyboardFunc(&on_key);
  glutIdleFunc(&on_idle);

  glutMainLoop();

  return 0;
}

void on_resize(int w, int h)
{
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1000, 1000, 0, 1000, 2, -1000);
}

void draw_scene()
{
  // the sky is blue
  glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
	
  glutSwapBuffers();
}

void on_key(unsigned char k, int x, int y)
{
  glutPostRedisplay();
}

void on_idle() 
{
}
