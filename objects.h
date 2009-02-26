#ifndef OBJECTS_H
#define OBJECTS_H

#include <GL/glut.h>

struct _object_t;

typedef void (*object_draw_t)(struct _object_t *u);
typedef void (*move_strategy_t)(struct _object_t *t, double sec_delta);

typedef struct _object_t
{
  GLfloat angle;
  GLfloat x;
  GLfloat z;
  /* speeds are in m/s */
  GLfloat sx; /* current speed along the x axis */
  GLfloat sz; /* current speed along the z axis */
  GLfloat rot_speed; /* in deg/s */
  size_t old_time; /* time of the last movement */

  object_draw_t draw;
  move_strategy_t strategy; /* how will this object move? */

  /* for the sentinel strategy */
  GLfloat origin_x;
  GLfloat origin_z;
  GLfloat max_dist; /* max distance from the user */
} object_t;

extern object_t user; /* main.c */

void object_init(object_t *u);
/* do the two objects collide? */
int object_collide(object_t *u, object_t *u2);
/* update the object's position */
void object_update_position(object_t *u);
void object_set_rot_speed(object_t *u, GLfloat speed);
void object_set_speed(object_t *u, GLfloat speed);
void object_move(object_t *u);

void flag_init(object_t *f);

/* strategies */
void user_strategy(object_t *u, double delta);
/* walk around, if the user is near try to catch it */
void sentinel_strategy(object_t *u, double delta);
/* just go after the user */
void follow_strategy(object_t *u, double delta);

void object_init_follower(object_t *u, GLfloat x, GLfloat z, GLfloat speed);

#endif /* OBJECTS_H */
