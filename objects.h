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
  /* speeds are in m/s */
  GLfloat sx; /* current speed along the x axis */
  GLfloat sz; /* current speed along the z axis */
  size_t old_time; /* time of the last movement */
} user_t;

void user_init(user_t *u);
/* update the user's position */
void user_update_position(user_t *u);
void user_set_speed(user_t *u, GLfloat speed);
void user_move(user_t *u);

#endif /* OBJECTS_H */
