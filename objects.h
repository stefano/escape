#ifndef OBJECTS_H
#define OBJECTS_H

#include <GL/glut.h>

typedef struct _flag_t
{
  GLfloat x;
  GLfloat z;
} flag_t;

void flag_init(flag_t *f);
void flag_draw(flag_t *f);

typedef struct _user_t
{
  GLfloat angle;
  GLfloat x;
  GLfloat z;
} user_t;

void user_init(user_t *u);
void user_a(user_t *u, GLfloat meters);
void user_move(user_t *u);

#endif /* OBJECTS_H */
