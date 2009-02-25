#include "objects.h"
#include "geometry.h"
#include "field.h"

#include <math.h>
#include <stdio.h>

void flag_init(flag_t *f)
{
  f->x = MIN_X + (FS/2) * MX;
  f->z = -(NEAR + 200*MZ);
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
}

void user_a(user_t *u, GLfloat nmeters)
{
  /*
     degree -> radians
     0 user degress are 90 real degrees
  */
  double angle = (u->angle / 180) * M_PI + M_PI/2;
  u->x += nmeters * MX * cos(angle);
  u->z += -nmeters * MZ * sin(angle);
}

void user_move(user_t *u)
{
  float h = field_height(&field, u->x, u->z);

  //printf("(%d,%d) h = %f\n", x, z, h);

  glRotatef(-u->angle, 0.0, 1.0, 0.0);
  glTranslatef(-u->x, - (h + 1.80) * MY, -u->z);
}
