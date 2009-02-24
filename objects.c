#include "objects.h"
#include "geometry.h"

#include <math.h>

void flag_init(flag_t *f)
{
  f->x = 3*MX;
  //  f->z = -FAR + 10*MZ;
  f->z = NEAR + 10*MZ;
}

void flag_draw(flag_t *f)
{
  glPushMatrix();
  glColor3f(1.0, 0.0, 0.0);
  glTranslatef(f->x, MIN_Y + MY, f->z);
  glScalef(MX, 10 * MY, MZ);
  glutSolidCube(1);
  glPopMatrix();
}

void user_init(user_t *u)
{
  u->angle = 0.0;
  u->x = 0.0;
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
  glRotatef(-u->angle, 0.0, 1.0, 0.0);
  glTranslatef(-u->x, MIN_Y + 2 * MY, -u->z);
}
