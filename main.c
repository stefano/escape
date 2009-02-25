/*
 * Author: Dissegna Stefano
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <GL/glut.h>

#include "geometry.h"
#include "field.h"
#include "sun.h"
#include "objects.h"

void on_resize(int x, int y);
void draw_scene();
void on_key(unsigned char k, int x, int y);
void on_key_up(unsigned char k, int x, int y);
void on_idle();
void quit(unsigned char k, int x, int y);
void left(unsigned char k, int x, int y);
void right(unsigned char k, int x, int y);
void up(unsigned char k, int x, int y);
void down(unsigned char k, int x, int y);
void user_left(unsigned char k, int x, int y);
void user_right(unsigned char k, int x, int y);
void user_advance(unsigned char k, int x, int y);
void user_back(unsigned char k, int x, int y);
void user_advance_stop(unsigned char k, int x, int y);
void user_back_stop(unsigned char k, int x, int y);

typedef void (*key_callback_t)(unsigned char k, int x, int y);

static key_callback_t callbacks[UCHAR_MAX];
static key_callback_t callbacks_up[UCHAR_MAX];

static sun_t sun;
static flag_t flag;
static user_t user;
field_t field;

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
  glutKeyboardUpFunc(&on_key_up);
  glutIdleFunc(&on_idle);

  for (i = 0; i < UCHAR_MAX; i++)
    callbacks[i] = NULL;

  callbacks['q'] = &quit;
  callbacks['s'] = &left;
  callbacks['x'] = &right;
  callbacks['a'] = &up;
  callbacks['z'] = &down;
  callbacks['n'] = &user_left;
  callbacks['m'] = &user_right;
  callbacks['k'] = &user_advance;
  callbacks['l'] = &user_back;

  callbacks_up['k'] = &user_advance_stop;
  callbacks_up['l'] = &user_back_stop;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  
  GLfloat black[4] = { 0, 0, 0, 1 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

  sun_init(&sun);
  field_init(&field);

  flag_init(&flag);
  user_init(&user);

  glutMainLoop();

  return 0;
}

void on_resize(int w, int h)
{
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glFrustum(MIN_X, MAX_X, MIN_Y, MAX_Y, NEAR, FAR);
  gluPerspective(30, w/h, NEAR, FAR);
}

float RY = 0.0;
float RX = 0.0;
float Z = -4.0;

void draw_scene()
{
  // the sky is blue
  glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // take user position and translate
  user_move(&user);
  // sun
  let_there_be_light(&sun);
  // field
  field_draw(&field);
  // flag
  flag_draw(&flag);

  // example cube
  /*  glTranslatef(0, 0, Z);
  glRotatef(RY, 0.0, 1.0, 0.0);
  glRotatef(RX, 1.0, 0.0, 0.0);

  glColor3f(1.0, 0.0, 0.0);
  glutSolidCube(1);
*/
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

void on_key_up(unsigned char k, int x, int y)
{
  key_callback_t f = callbacks_up[k];
  if (f) 
    {
      (*f)(k, x, y);
      glutPostRedisplay();
    }
}

void on_idle() 
{
  user_update_position(&user);
}

void quit(unsigned char k, int x, int y) 
{
  exit(0);
}

void left(unsigned char k, int x, int y)
{
  //  sun.position[0] += 10;
  //printf("x:%f\n", sun.position[0]);
}

void right(unsigned char k, int x, int y)
{
  sun.position[2] -= 10;
  printf("z:%f\n", sun.position[2]);
}

void up(unsigned char k, int x, int y)
{
  RX += 10;
}

void down(unsigned char k, int x, int y)
{
  RX -= 10;
}

void user_left(unsigned char k, int x, int y)
{
  user.angle += 10;
}

void user_right(unsigned char k, int x, int y)
{
  user.angle -= 10;
}

void user_advance(unsigned char k, int x, int y)
{
  user_set_speed(&user, 5);
  //  user_a(&user, 1);
}

void user_advance_stop(unsigned char k, int x, int y)
{
  user_set_speed(&user, 0);
  //  user_a(&user, 1);
}


void user_back(unsigned char k, int x, int y)
{
  //user_a(&user, -1);
  user_set_speed(&user, -5);
}

void user_back_stop(unsigned char k, int x, int y)
{
  //user_a(&user, -1);
  user_set_speed(&user, 0);
}
