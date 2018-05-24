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
  p->next=NULL;

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
  w->ppm = NULL;
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
  ppmFree(w->ppm);
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


Picture *initPicture(int width, int height)
{
  Picture *p = malloc(sizeof(Picture));

  p->height = height;
  p->width = width;
  p->pixels = malloc(width*height*sizeof(Pixel));

  return p;
}

/* retourne NULL si l'ouverture de fichier échoue */
Picture *openPicture(char *fileName)
{
  int width, height, maxColorValue;
  char magicNumber[3];
  int r, g, b;
  FILE *fichier = fopen(fileName, "r");
  
  if (fichier == NULL)
    return NULL;
  fscanf(fichier, "%s", magicNumber);
  fscanf(fichier, "%d", &width);
  fscanf(fichier, "%d", &height);
  fscanf(fichier, "%d", &maxColorValue);

  Picture *p = initPicture(width, height);
  for (int i = 0; i<((p->width)*(p->height)); i++)
    {
      fscanf(fichier, "%d", &r);
      fscanf(fichier, "%d", &g);
      fscanf(fichier, "%d", &b);
      p->pixels[i].r = r;
      p->pixels[i].g = g;
      p->pixels[i].b = b;
    }
  fclose(fichier);
  return p;
}

void pictureToWorld(World *world, Picture *p)
{
  int xtab;
  int ytab;
  Coord min;
  Coord max;
  
  for (int i = 0; i < ((p->width)*(p->height)); i++)
    {
      if (p->pixels[i].r == 255 && p->pixels[i].g == 255 && p->pixels[i].b == 255){} //optimisation car le blanc revient tout le temps et donc ne pas devoir tester toutes les autres valeurs si c'est du blanc
      else if (p->pixels[i].r == 255 && p->pixels[i].g == 0 && p->pixels[i].b == 0) //Pixel rouge
	{
	  xtab = i%(p->width);
	  ytab = i/(p->width);
	  min.x = xtab*(WINDOW_HEIGHT/(p->height));
	  min.y = ytab*(WINDOW_HEIGHT/(p->height));
	  max.x = (1+xtab)*(WINDOW_HEIGHT/(p->height));
	  max.y = (1+ytab)*(WINDOW_HEIGHT/(p->height));
	  world->obstacles = addObject(
				       initObject(min, max, 'o', world->obstacleTexture),
				       world->obstacles);
	}
      else if (p->pixels[i].r == 0 && p->pixels[i].g == 255 && p->pixels[i].b == 0) //Pixel vert
	{
	  xtab = i%(p->width);
          ytab = i/(p->width);
          min.x = xtab*(WINDOW_HEIGHT/(p->height));
          min.y	= ytab*(WINDOW_HEIGHT/(p->height));
          max.x	= (1+xtab)*(WINDOW_HEIGHT/(p->height));
          max.y	= (1+ytab)*(WINDOW_HEIGHT/(p->height));
          world->ennemies = addObject(
                                       initObject(min, max, 'e', world->ennemyTexture),
                                       world->ennemies);
        }
      else //Pixel noir
        {
	  xtab = i%(p->width);
          ytab = i/(p->width);
          min.x = xtab*(WINDOW_HEIGHT/(p->height));
          min.y	= ytab*(WINDOW_HEIGHT/(p->height));
          max.x	= (1+xtab)*(WINDOW_HEIGHT/(p->height));
          max.y	= (1+ytab)*(WINDOW_HEIGHT/(p->height));
          world->end = addObject(
                                       initObject(min, max, 'l', world->endLineTexture),
                                       world->end);
        }
    }
}

void ppmToWorld(char *filename, World *world){
	Picture *ppm=openPicture(filename);
	if(ppm==NULL){
		fprintf(stderr,"Opening of the ppm file failed");
		exit(1);
	}
	pictureToWorld(world, ppm);
	world->ppm=ppm;
}

void ppmFree(Picture *p)
{
  free(p->pixels);
  free(p);
}
