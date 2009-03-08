/*
 * Author: Dissegna Stefano
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <GL/glut.h>

#include "geometry.h"
#include "field.h"
#include "sun.h"
#include "objects.h"

/* textures */
#include "textures/grass.h"

#define USER_SPEED 12

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
#define N_ENEMIES 12
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
  callbacks['s'] = &user_left;
  callbacks['x'] = &user_right;
  callbacks['a'] = &user_advance;
  callbacks['z'] = &user_back;

  callbacks_up['s'] = &user_rot_stop;
  callbacks_up['x'] = &user_rot_stop;
  callbacks_up['a'] = &user_stop;
  callbacks_up['z'] = &user_stop;

  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

  sun_init(&sun);
  field_init(&field);

  flag_init(&flag);
  object_init(&user);
  user.strategy = &user_strategy;
  
  GLfloat enemy_conf[N_ENEMIES][3] = {
    { MIN_X + 10*MX, -NEAR,  USER_SPEED/2 },
    { MAX_X - 10*MX, -NEAR, USER_SPEED/2 },
    { MIN_X + 10*MX, -FAR, USER_SPEED/2 },
    { MAX_X - 10*MX, -FAR, USER_SPEED/2 },
    { MIN_X + 10*MX, -(NEAR+(FS/2)*MZ), USER_SPEED/2 },
    { MAX_X - 10*MX,  -(NEAR+(FS/2)*MZ), USER_SPEED/2 },
    { MIN_X + (FS/4)*MX, -(NEAR+(FS/4)*MZ), USER_SPEED/2 },
    { MIN_X + (FS/4)*MX, -(FAR-(FS/4)*MZ), USER_SPEED/2 },
    { MAX_X - (FS/4)*MX, -(NEAR+(FS/4)*MZ), USER_SPEED/2 },
    { MAX_X - (FS/4)*MX, -(FAR-(FS/4)*MZ), USER_SPEED/2 },
    { MAX_X - (FS/2)*MX, -(NEAR+(FS/2)*MZ), USER_SPEED/2 },
    { MAX_X - (FS/2)*MX, -(FAR-(FS/4)*MZ), USER_SPEED/2 }
  };

  for (i = 0; i < N_ENEMIES/3; i++)
    object_init_follower(&enemies[i], enemy_conf[i][0], enemy_conf[i][1],
                         enemy_conf[i][2]);
  
  for (i = N_ENEMIES/3+1; i < N_ENEMIES; i++)
    object_init_watcher(&enemies[i], enemy_conf[i][0], enemy_conf[i][1],
                        enemy_conf[i][2]);
  
  object_init_watcher(&enemies[0], 500, -100, 5);

  glutMainLoop();

  return 0;
}

void on_resize(int w, int h)
{
  if (h == 0)
    h = 1; /* avoid division by 0 */

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
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
      sun.diffuse[0] = 0.3;
      sun.diffuse[1] = 1.0;
      sun.diffuse[2] = 0.3;
    }
  else
    {
      /* red light */
      sun.diffuse[0] = 1.0;
      sun.diffuse[1] = 0.3;
      sun.diffuse[2] = 0.3;
    }
}

void on_idle() 
{
  int i;
  object_update_position(&user);
  object_update_position(&flag);
  for (i = 0; i < N_ENEMIES; i++)
    object_update_position(&enemies[i]);

  /* detect a collision 
     if the refresh rate is too slow, a collision may go unnoticed
     because objects may "teleporte" themselves beyond the target */
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
  object_set_speed(&user, USER_SPEED);
}

void user_back(unsigned char k, int x, int y)
{
  object_set_speed(&user, -USER_SPEED);
}

void user_stop(unsigned char k, int x, int y)
{
  object_set_speed(&user, 0);
}
