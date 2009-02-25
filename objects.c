#include "objects.h"
#include "geometry.h"
#include "field.h"

#include <math.h>
#include <stdio.h>

void flag_init(flag_t *f)
{
  f->x = MIN_X + (FS/2) * MX;
  f->z = -(NEAR + 50*MZ);
}

void flag_draw(flag_t *f)
{
  glPushMatrix();
  glColor3f(1.0, 0.0, 0.0);
  glTranslatef(f->x, MIN_Y + field_height(&field, f->x, f->z), f->z);
  glScalef(2*MX, 10*MY, 2*MZ);
  glutSolidCube(1);
  glPopMatrix();
}

void user_init(user_t *u)
{
  u->angle = 0.0;
  u->x = MIN_X + (FS/2)*MX;
  u->z = -NEAR;
  u->sx = u->sz = 0.0;
  u->old_time = 0;
}

void user_set_speed(user_t *u, GLfloat speed)
{
  double angle = (u->angle / 180) * M_PI + M_PI/2;
  u->sx = speed * cos(angle);
  u->sz = speed * sin(angle);
}

void user_update_position(user_t *u)
{
  size_t current_time = glutGet(GLUT_ELAPSED_TIME);
  size_t delta = 0;
  double secs;
  double angle;
  GLfloat xmeters, zmeters;

  if (u->old_time != 0) /* not the first time */
    delta = current_time - u->old_time; 
  u->old_time = current_time;

  secs = delta/1000.0;
  xmeters = secs * u->sx;
  zmeters = secs * u->sz;

  //  printf("%f, %f,%f\n", secs, xmeters, zmeters);

  /*
     degree -> radians
     0 user degress are 90 real degrees
     TODO: make angle variations depend on speed
  */
  //  angle = (u->angle / 180) * M_PI + M_PI/2;
  //u->x += xmeters * MX * cos(angle);
  //u->z += -zmeters * MZ * sin(angle);
  u->x += xmeters * MX;
  u->z += -zmeters * MZ;

  /* block on the walls */
  if (u->x > MAX_X-MX)
    u->x = MAX_X-MX;
  if (u->x < MIN_X+MX)
    u->x = MIN_X+MX;
  if (u->z < -FAR+MZ)
    u->z = -FAR+MZ;
  if (u->z > -NEAR-MZ)
    u->z = -NEAR-MZ;
}

void user_move(user_t *u)
{
  float h = field_height(&field, u->x, u->z);

  //printf("(%d,%d) h = %f\n", x, z, h);

  glRotatef(-u->angle, 0.0, 1.0, 0.0);
  glTranslatef(-u->x, - (h + 1.80) * MY, -u->z);
}
