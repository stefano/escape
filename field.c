#include "field.h"
#include "geometry.h"

#include <GL/glut.h>
#include <stdio.h>

void field_init(field_t *f)
{
  FILE *map = fopen("field.map", "r");
  int error = 1;
  unsigned char bytemap[FS][FS];

  if (map)
    {
      size_t res = fread(&(bytemap[0][0]), sizeof(unsigned char), 
                         FS * FS, map);
      if (res == FS*FS) 
        error = 0;
    }
  if (error)
    {
      int i, j;
      for (i = 0; i < FS; i++) 
        for (j = 0; j < FS; j++)
          f->height[i][j] = 0.0;
    }
  else
    {
      int i,j;
      for (i = 0; i < FS; i++) 
        for (j = 0; j < FS; j++)
          f->height[i][j] = bytemap[i][j] / 10.0; // meters 
    }
}

void field_draw(field_t *f)
{
  int i,j;
  
  glColor3f(0.0, 1.0, 0.0);
  glBegin(GL_QUADS);

  for (i = 0; i < FS-1; i++)
    for (j = 0; j < FS-1; j++) 
      {
        glVertex3f(MIN_X + i*MX, MIN_Y + f->height[i][j], -(NEAR + j*MZ));
        glVertex3f(MIN_X + i*MX, MIN_Y + f->height[i][j+1], 
                   -(NEAR + (j+1)*MZ));
        glVertex3f(MIN_X + (i+1)*MX, MIN_Y + f->height[i+1][j+1], 
                   -(NEAR + (j+1)*MZ));
        glVertex3f(MIN_X + (i+1)*MX, MIN_Y + f->height[i+1][j], 
                   -(NEAR + j*MZ));
      }
  
  glEnd();
}

float field_height(field_t *f, float x, float z)
{
  int ix, iz;

  ix = x / MX;
  iz = (-z) / MZ;

  if (ix >= 0 && ix < FS && iz >= 0 && iz < FS)
    return field.height[ix][iz];
  return 0.0;
}
