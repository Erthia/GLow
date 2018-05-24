#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "../include/data_struct.h"
#include "../include/display.h"

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

void deleteObjects(Object *liste)
{
  Object *next = liste->next;
  free(liste);
  if(next != NULL)
    deleteObjects(next);
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

void deleteProjectiles(Projectile *liste)
{
  Projectile *next = liste->next;
  free(liste);
  if(next != NULL)
    deleteProjectiles(next);
}

World *initWorld(GLuint *obstacleTexture, GLuint *ennemyTexture, GLuint *projectileTexture, GLuint *playerTexture, GLuint *levelTexture, GLuint *endTexture)
{
  World *w = malloc(sizeof(World));
  
  w->position = WINDOW_WIDTH/2;
  w->projectiles = NULL;
  w->player = NULL;
  w->ennemies = NULL;
  w->obstacles = NULL;
  w->end = NULL;
  w->obstacleTexture = obstacleTexture;
  w->ennemyTexture = ennemyTexture;
  w->projectileTexture = projectileTexture;
  w->playerTexture = playerTexture;
  w->levelTexture = levelTexture;
  w->endTexture = endTexture;

  return w;
}

void deleteWorld(World *w)
{
  deleteProjectiles(w->projectiles);
  deleteObjects(w->player);
  deleteObjects(w->ennemies);
  deleteObjects(w->obstacles);
  deleteObjects(w->end);
  free(w);
}

Picture *initPicture(int width, int height)
{
  Picture *p = malloc(sizeof(Picture));

  p->height = height;
  p->width = width;
  p->pixels = malloc(w*h*sizeof(Pixel*));

  return p;
}

Picture *openPicture(char *fileName)
{
  int width, height, maxColorValue;
  char magicNumber[3];
  int r, g, b;
  FILE *fichier = fopen(fileName, "r");
  
  if (fichier == NULL)
    return NULL;
  fscanf(fichier, "%s %d %d %d %d", magicNumber, &width, &height, &maxColorValue);

  Picture *p = initPicture(width, height);
  for (int i = 0; i<((p->width)*(p->height)); i++)
    {
      fscanf(fichier, "%d %d %d", &r, &g, &b);
      p->pixels[i].r = r;
      p->pixels[i].g = g;
      p->pixels[i].b = b;
    }
  fclose(fichier);
  return p;
}
