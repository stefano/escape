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

void object_init(object_t *u)
{
  u->angle = 0.0;
  u->x = MIN_X + (FS/2)*MX;
  u->z = -NEAR;
  u->sx = u->sz = u->rot_speed = 0.0;
  u->old_time = 0;
  u->draw = NULL;
  u->strategy = NULL;
}

void object_set_rot_speed(object_t *u, GLfloat speed)
{
  u->rot_speed = speed;
}

void object_set_speed(object_t *u, GLfloat speed)
{
  /*
     degree -> radians
     0 object degress are 90 real degrees
     TODO: make angle variations depend on speed
  */
  double angle = (u->angle / 180) * M_PI + M_PI/2;
  u->sx = speed * cos(angle);
  u->sz = speed * sin(angle);
}

void object_update_position(object_t *u)
{
  size_t current_time = glutGet(GLUT_ELAPSED_TIME);
  size_t delta = 0;

  if (u->old_time != 0) /* not the first time */
    delta = current_time - u->old_time; 
  u->old_time = current_time;

  if (u->strategy)
    (*(u->strategy))(u, delta/1000.0);

}

void object_move(object_t *u)
{
  float h = field_height(&field, u->x, u->z);
  glRotatef(-u->angle, 0.0, 1.0, 0.0);
  glTranslatef(-u->x, - (h + 1.80) * MY, -u->z);
}

void user_strategy(object_t *u, double delta)
{
  GLfloat xmeters = delta * u->sx;
  GLfloat zmeters = delta * u->sz;
  GLfloat rad = delta * u->rot_speed;

  u->angle += rad;
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

void sentinel_strategy(object_t *u, double delta)
{
  
}

void follow_strategy(object_t *u, double delta)
{
  /* distance vector */
  GLfloat x = user.x - u->x;
  GLfloat z = user.z - u->z;
  /* normalize */
  GLfloat len = sqrt(x*x+z*z);
  /* don't move when on the target */
  if (len != 0)
    {
      x /= len;
      z /= len;

      /* sx is treated as the speed tangent to the distance vector */
    
      GLfloat xmeters = delta * u->sx * x;
      GLfloat zmeters = delta * u->sx * z;

      u->x += xmeters * MX;
      u->z += zmeters * MZ;
    }
}

void draw_sphere(object_t *u)
{
  GLfloat r = 2;
  glPushMatrix();
  glColor3f(0.0, 0.0, 1.0);
  glTranslatef(u->x, MIN_Y + r * MY + field_height(&field, u->x, u->z), u->z);
  glScalef(r * MX, r * MY, r * MZ);
  glutSolidSphere(1, 42, 42);
  glPopMatrix();
}

void object_init_follower(object_t *u, GLfloat x, GLfloat z, GLfloat speed)
{
  object_init(u);
  u->draw = &draw_sphere;
  u->strategy = &follow_strategy;
  u->x = x;
  u->z = z;
  u->sx = speed;
}
