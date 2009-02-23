#ifndef SUN_H
#define SUN_H

#include <GL/glut.h>

typedef struct _sun_t
{
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat position[4];
} sun_t;

void sun_init(sun_t *s);

void let_there_be_light(sun_t *s);

#endif /* SUN_H */
