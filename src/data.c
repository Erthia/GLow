#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "../include/data_struct.h"

Object *initObject(Coord min, Coord max, char type, GLuint *textureID)
{
  Object *o = malloc(sizeof(Object));

  if (o == NULL)
    return NULL;
  o->type = type;
  o->textureID = textureID;
  o->next = NULL;
  o->min.x = min.x;
  o->min.y = min.y;
  o->max.x = max.x;
  o->max.y = max.y;

  return o;
}

Object *addObject(Object *o, Object *liste)
{
  if (liste == NULL)
    return o;
  else
    {
      Object *tmp = liste;
      while(tmp->next != NULL)
	tmp=tmp->next;
      tmp->next = o;
      return liste;
    }
}

Projectile *initProjectile(Coord min, Coord max, char dir)
{
  Projectile *p = malloc(sizeof(Projectile));

  if (p == NULL)
    return p;
  p->min.x = min.x;
  p->min.y = min.y;
  p->max.x = max.x;
  p->max.y = max.y;
  p->dir = dir;

  return p;
}

Projectile *addProjectile(Projectile *p, Projectile *liste)
{
  if (liste != NULL)
    p->next = liste;
  return p;
}