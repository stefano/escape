#ifndef FIELD_H
#define FIELD_H

#define FS 1024
#define FIELD_TEX 1

typedef struct _field_t
{
  float height[FS][FS];
  /* pre-computed normals */
  float normals[FS][FS][3];
  /* pre-computed vertexes */
  float v[FS][FS][3];
  unsigned int field_display_list;
} field_t;

void field_init(field_t *f);
void field_draw(field_t *f);
/* get height at position (x,z) */
float field_height(field_t *f, float x, float z);
/* asjust the speed to the inclination */
//void field_inclination(field_t *f, float x, float z, float *sx, float *sz);

extern field_t field;

#endif /* FIELD_H */
