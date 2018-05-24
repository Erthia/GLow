#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "../include/data_struct.h"
#include "../include/display.h"
#include "../include/data.h"

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

/* FINI */
void deleteObjects(Object *liste)
{
	if(liste != NULL){
		Object *next = liste->next;
		free(liste);
		deleteObjects(next);
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

/* FINI */
void deleteProjectiles(Projectile *liste)
{
	if(liste!=NULL){
		Projectile *next = liste->next;
		free(liste);
		deleteProjectiles(next);
	}
}

World *initWorld()
{
  World *w = malloc(sizeof(World));
  
  w->position = WINDOW_WIDTH/2;
  w->projectiles = NULL;
  w->player = NULL;
  w->ennemies = NULL;
  w->obstacles = NULL;
  w->end = NULL;
  loadAllTextures(w);

  return w;
}

/* FINI */
void deleteWorld(World *w)
{
  deleteProjectiles(w->projectiles);
  deleteObjects(w->player);
  deleteObjects(w->ennemies);
  deleteObjects(w->obstacles);
  deleteObjects(w->end);
  glDeleteTextures(1, w->obstacleTexture);
  glDeleteTextures(1, w->ennemyTexture);
  glDeleteTextures(1, w->projectileTexture);
  glDeleteTextures(1, w->playerTexture);
  glDeleteTextures(1, w->levelTexture);
  glDeleteTextures(1, w->endLineTexture);
  glDeleteTextures(1, w->endLineTexture);
  glDeleteTextures(1, w->happyEndTexture);
  glDeleteTextures(1, w->deathEndTexture);

  free(w);
}

void setPlayer(World *world){
	Coord playerMin, playerMax;
	playerMin.x=WINDOW_WIDTH/8;
	playerMin.y=WINDOW_HEIGHT/2-PIXEL_SIZE/2;
	playerMax.x=playerMin.x+PIXEL_SIZE;
	playerMax.y=playerMin.y+PIXEL_SIZE;
	Object *player=initObject(playerMin, playerMax, 'j', world->playerTexture);
	world->player=addObject(player, world->player);
}

Picture *initPicture(int width, int height)
{
  Picture *p = malloc(sizeof(Picture));

  p->height = height;
  p->width = width;
  p->pixels = malloc(width*height*sizeof(Pixel*));

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
  fscanf(fichier, "%s %d %d %d", magicNumber, &width, &height, &maxColorValue);

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
