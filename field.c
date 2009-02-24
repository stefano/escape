#include "field.h"
#include "geometry.h"

#include <GL/glut.h>
#include <stdio.h>

void field_init(field_t *f)
{
  FILE *f = fopen("field.map", "r");
  int error = 1;
  if (f)
    {
      size_t res = fread(&(f->height[0][0]), sizeof(unsigned char), 
                         FS * FS, f);
      if (res == FS*FS) 
        error = 0;
    }
  if (error)
    {
      int i, j;
      for (i = 0; i < FS; i++) 
        for (j = 0; j < FS; j++)
          f->height[i][j] = 0;
    }
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
