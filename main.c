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

/* textures */
#include "textures/grass.h"
//#include "textures/wall.h"

void on_resize(int x, int y);
void draw_scene();
void on_key(unsigned char k, int x, int y);
void on_key_up(unsigned char k, int x, int y);
void on_idle();
void quit(unsigned char k, int x, int y);
void user_left(unsigned char k, int x, int y);
void user_right(unsigned char k, int x, int y);
void user_rot_stop(unsigned char k, int x, int y);
void user_advance(unsigned char k, int x, int y);
void user_back(unsigned char k, int x, int y);
void user_stop(unsigned char k, int x, int y);

typedef void (*key_callback_t)(unsigned char k, int x, int y);

static key_callback_t callbacks[UCHAR_MAX];
static key_callback_t callbacks_up[UCHAR_MAX];

static sun_t sun;
static object_t flag;
object_t user;
#define N_ENEMIES 4
static object_t enemies[N_ENEMIES];
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
  callbacks['n'] = &user_left;
  callbacks['m'] = &user_right;
  callbacks['k'] = &user_advance;
  callbacks['l'] = &user_back;

  callbacks_up['n'] = &user_rot_stop;
  callbacks_up['m'] = &user_rot_stop;
  callbacks_up['k'] = &user_stop;
  callbacks_up['l'] = &user_stop;

  //glEnable(GL_CCW);
  //  glEnable(GL_CULL_FACE);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  
  GLfloat black[4] = { 0, 0, 0, 1 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

  glBindTexture(GL_TEXTURE_2D, FIELD_TEX);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gimp_image.width, gimp_image.height,
               0, GL_RGB, GL_UNSIGNED_BYTE, gimp_image.pixel_data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  /*
  glBindTexture(GL_TEXTURE_2D, WALL_TEX);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wall_tex.width, wall_tex.height,
               0, GL_RGB, GL_UNSIGNED_BYTE, wall_tex.pixel_data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
*/
  sun_init(&sun);
  field_init(&field);

  flag_init(&flag);
  object_init(&user);
  user.strategy = &user_strategy;

  GLfloat enemy_conf[N_ENEMIES][3] = {
    { MIN_X + 10*MX, -NEAR,  20 },
    { MAX_X - 10*MX, -NEAR, 30 },
    { MIN_X + (FS/2)*MX, -FAR, 15 },
    { MIN_X, -FAR, 18 }
  };

  for (i = 0; i < N_ENEMIES; i++)
    object_init_follower(&enemies[i], enemy_conf[i][0], enemy_conf[i][1],
                         enemy_conf[i][2]);

  glutMainLoop();

  return 0;
}

void on_resize(int w, int h)
{
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glFrustum(MIN_X, MAX_X, MIN_Y, MAX_Y, NEAR, FAR);
  gluPerspective(45, w/h, NEAR, FAR);
}

void draw_scene()
{
  int i;

  /* the sky is blue */
  glClearColor(0.0f, 0.0f, 0.6f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // take user position and translate
  object_move(&user);
  // sun
  let_there_be_light(&sun);
  // field
  field_draw(&field);
  // flag
  (*(flag.draw))(&flag);

  for (i = 0; i < N_ENEMIES; i++)
    if (enemies[i].draw)
      (*(enemies[i].draw))(&enemies[i]);

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

/* 
 * End the game
 */
void end_game(int winner)
{
  int i;
  /* disable (almost) all events */
  glutIdleFunc(NULL);
  glutKeyboardUpFunc(NULL);
  for (i = 0; i < UCHAR_MAX; i++)
    if (i != 'q') /* we must still be able to exit */
      callbacks[i] = NULL;
  if (winner)
    {
      /* green light */
      sun.diffuse[0] = 0.4;
      sun.diffuse[1] = 1.0;
      sun.diffuse[2] = 0.4;      
    }
  else
    {
      /* red light */
      sun.diffuse[0] = 1.0;
      sun.diffuse[1] = 0.4;
      sun.diffuse[2] = 0.4;
    }
}

void on_idle() 
{
  int i;
  object_update_position(&user);
  object_update_position(&flag);
  for (i = 0; i < N_ENEMIES; i++)
    object_update_position(&enemies[i]);

  /* detect a collision */
  if (object_collide(&user, &flag))
    end_game(1);
  else
    {
      for (i = 0; i < N_ENEMIES; i++)
        if (object_collide(&user, &enemies[i]))
          end_game(0);
    }

  glutPostRedisplay();
}

void quit(unsigned char k, int x, int y) 
{
  exit(0);
}

void user_left(unsigned char k, int x, int y)
{
  object_set_rot_speed(&user, 60);
}

void user_right(unsigned char k, int x, int y)
{
  object_set_rot_speed(&user, -60);
}

void user_rot_stop(unsigned char k, int x, int y)
{
  object_set_rot_speed(&user, 0);
}

void user_advance(unsigned char k, int x, int y)
{
  object_set_speed(&user, 50);
}

void user_back(unsigned char k, int x, int y)
{
  object_set_speed(&user, -50);
}

void user_stop(unsigned char k, int x, int y)
{
  object_set_speed(&user, 0);
}
