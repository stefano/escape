#include "sun.h"

void sun_init(sun_t *s)
{
  s->ambient[0] = 0.2;
  s->ambient[1] = 0.2;
  s->ambient[2] = 0.2;
  s->ambient[3] = 1.0;

  s->diffuse[0] = 0.8;
  s->diffuse[1] = 0.8;
  s->diffuse[2] = 0.8;
  s->diffuse[3] = 1.0;

  s->specular[0] = 1.0;
  s->specular[1] = 1.0;
  s->specular[2] = 1.0;
  s->specular[3] = 1.0;

  s->position[0] = 0.5;
  s->position[1] = 0.9;
  s->position[2] = -1;
  s->position[3] = 1.0;
}

void let_there_be_light(sun_t *s)
{
  glLightfv(GL_LIGHT0, GL_AMBIENT, s->ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, s->diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, s->specular);

  glLightfv(GL_LIGHT0, GL_POSITION, s->position);
  glEnable(GL_LIGHT0);
}