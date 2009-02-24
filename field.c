#include "field.h"
#include "geometry.h"

#include <GL/glut.h>

void field_init(field_t *f)
{
}

void field_draw(field_t *f)
{
  // a plain field for the moment
  glColor3f(0.1, 0.8, 0.1);
  glBegin(GL_QUADS);
  /*  glVertex3f(MIN_X * FAR, MIN_Y + 1.0, NEAR);
  glVertex3f(MIN_X * FAR, MIN_Y + 1.0, FAR);
  glVertex3f(MAX_X * FAR, MIN_Y + 1.0, FAR);
  glVertex3f(MAX_X * FAR, MIN_Y + 1.0, NEAR);*/
  glVertex3f(MIN_X * FAR, MIN_Y, NEAR);
  glVertex3f(MIN_X * FAR, MIN_Y, -FAR);
  glVertex3f(MAX_X * FAR, MIN_Y, -FAR);
  glVertex3f(MAX_X * FAR, MIN_Y, NEAR);
  
  glEnd();
}
