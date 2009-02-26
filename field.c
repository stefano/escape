#include "field.h"
#include "geometry.h"

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>

void vec_copy(float *v1, float *v2, size_t len)
{
  int i;
  for (i = 0; i < len; i++)
    v2[i] = v1[i];
}

void find_normal(float o[], float v1[], float v2[], float res[])
{
  int i;
  float a[3];
  float b[3];
  vec_copy(v1, a, 3);
  vec_copy(v2, b, 3);

  for (i = 0; i < 3; i++) 
    {
      a[i] -= o[i];
      b[i] -= o[i];
    }

  res[0] = a[1]*b[2] - a[2]*b[1];
  res[1] = a[2]*b[0] - a[0]*b[2];
  res[2] = a[0]*b[1] - a[1]*b[0];

  float len = sqrt(res[0]*res[0]+res[1]*res[1]+res[2]*res[2]);
  res[0] /= len;
  res[1] /= len;
  res[2] /= len;
}

void vec_avg(float **v, size_t n, float *res)
{
  int i, j;
  for (i = 0; i < n; i++)
    for (j = 0; j < 3; j++)
      res[j] += v[i][j];
  for (j = 0; j < 3; j++)
    res[j] /= n;
}

void field_init(field_t *f)
{
  FILE *map = fopen("field.map", "r");
  int error = 1;
  unsigned char bytemap[FS][FS];
  int i,j;

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

  /* find vertexes */
  for (i = 0; i < FS; i++)
    for (j = 0; j < FS; j++) 
      {
        float v[3] = { MIN_X + i*MX, MIN_Y + (f->height[i][j])*MY,
                       -(NEAR + j*MZ) };
        vec_copy(v, f->v[i][j], 3);

      }
  /* find normals */
  float *norms = malloc(sizeof(float)*FS*FS*3); /* doesn't fit on the stack */
  for (i = 0; i < FS; i++)
    for (j = 0; j < FS; j++) 
      {
        float norm[3];
        float *v2, *v3;
        if (j == FS-1)
          v2 = f->v[i][j-1];
        else
          v2 = f->v[i][j+1];          
        if (i == FS-1)
          v3 = f->v[i-1][j];
        else
          v3 = f->v[i+1][j];
        find_normal(f->v[i][j], v3, v2, norm);
        vec_copy(norm, norms+(i*FS+j*3), 3);
      }

  /* take the avarage */
  for (i = 0; i < FS; i++)
    for (j = 0; j < FS; j++) 
      {

        if (i == 0 || i == FS-1 || j == 0 || j == FS-1) /* border */
          vec_copy(norms+(i*FS+j*3), f->normals[i][j], 3);
        else
          {
            float *v[4] = { norms+(i*FS+j), norms+(i*FS+j-1), 
                            norms+((i-1)*FS+j), norms+((i-1)*FS+j-1) };
            vec_avg(v, 4, f->normals[i][j]);
          }
      }
  free(norms);
}

void field_draw(field_t *f)
{
  int i,j;

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, FIELD_TEX);
  
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_QUADS);

  for (i = 0; i < FS-1; i++)
    for (j = 0; j < FS-1; j++) 
      {
        glNormal3fv(f->normals[i][j]);
        glTexCoord2f(0, 0);
        glVertex3fv(f->v[i][j]);
        glNormal3fv(f->normals[i][j+1]);
        glTexCoord2f(0, 1);
        glVertex3fv(f->v[i][j+1]);
        glNormal3fv(f->normals[i+1][j+1]);
        glTexCoord2f(1, 1);
        glVertex3fv(f->v[i+1][j+1]);
        glNormal3fv(f->normals[i+1][j]);
        glTexCoord2f(1, 0);
        glVertex3fv(f->v[i+1][j]);
      }
  glEnd();
  glDisable(GL_TEXTURE_2D);

  /* walls */
  glColor3f(0.5, 0.5, 0.5); /* gray */
  //glBindTexture(GL_TEXTURE_2D, WALL_TEX);
  glBegin(GL_QUADS);

  /* left */
  glNormal3f(1, 0, 0);
  //glTexCoord2f(0, 0);
  glVertex3f(MIN_X, MIN_Y, -NEAR);
  //glTexCoord2f(0, MAX_Y - MIN_Y);
  glVertex3f(MIN_X, MAX_Y, -NEAR);
  //glTexCoord2f(FAR - NEAR, MAX_Y - MIN_Y);
  glVertex3f(MIN_X, MAX_Y, -FAR);
  //  glTexCoord2f(FAR - NEAR, 0);
  glVertex3f(MIN_X, MIN_Y, -FAR);
  
  /* front */
  glNormal3f(0, 0, 1);
  glVertex3f(MIN_X, MIN_Y, -FAR);
  glVertex3f(MIN_X, MAX_Y, -FAR);
  glVertex3f(MAX_X, MAX_Y, -FAR);
  glVertex3f(MAX_X, MIN_Y, -FAR);
  
  /* right */
  glNormal3f(-1, 0, 0);
  glVertex3f(MAX_X, MIN_Y, -NEAR);
  glVertex3f(MAX_X, MAX_Y, -NEAR);
  glVertex3f(MAX_X, MAX_Y, -FAR);
  glVertex3f(MAX_X, MIN_Y, -FAR);

  /* back */
  glNormal3f(0, 0, -1);
  glVertex3f(MIN_X, MIN_Y, -NEAR);
  glVertex3f(MIN_X, MAX_Y, -NEAR);
  glVertex3f(MAX_X, MAX_Y, -NEAR);
  glVertex3f(MAX_X, MIN_Y, -NEAR);
  
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
