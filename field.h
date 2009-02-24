#ifndef FIELD_H
#define FIELD_H

#define FS 1024

typedef struct _field_t
{
  unsigned char height[FS][FS];
} field_t;

void field_init(field_t *f);
void field_draw(field_t *f);

#endif /* FIELD_H */
