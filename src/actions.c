#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_image.h>

#include "../include/actions.h"
#include "../include/data_struct.h"
#include "../include/data.h"

/* dir : 'N':North - 'S':South - 'E':East - 'W':West
 * Return EXIT_FAILURE if obj is NULL, or dir invalid
 * Do not manage the possible exit of the level map */
int moveObject(Object *obj, char dir, int speed){
	if(obj==NULL) return EXIT_FAILURE;
	switch(dir){
		case 'N':
			obj->min.y-=speed;
			obj->max.y-=speed;
		break;
		case 'S':
			obj->min.y+=speed;
			obj->max.y+=speed;
		break;
		case 'E':
			obj->min.x+=speed;
			obj->max.x+=speed;
		break;
		case 'W':
			obj->min.x-=speed;
			obj->max.x-=speed;
		break;
		
		default:
			return EXIT_FAILURE;
		break;
	}
	return EXIT_SUCCESS;
}

/*
  Fonction de test des collisions 
  retourne 1 si les objets entrent en collision
  retourne 0 sinon
*/
int colide(Coord minObj1, Coord maxObj1, Coord minObj2, Coord maxObj2)
{
  float widthObj1 = maxObj1.x - minObj1.x;
  float heightObj1 = maxObj1.y - minObj1.y;
  float widthObj2 = maxObj2.x - minObj2.x;
  float heightObj2 = maxObj2.y - minObj2.y;

  float	maxLeft = fmaxf(minObj1.x, minObj2.x);
  float	minRight = fminf(minObj1.x + widthObj1, minObj2.x + widthObj2);
  float	maxBottom = fmaxf(minObj1.y, minObj2.y);
  float	minTop = fminf(minObj1.y + heightObj1, minObj2.y + heightObj2);

  if (maxLeft < minRight && maxBottom < minTop)
    return 1;
  return 0;
}

void moveForwardProjectiles(World *world, Projectile *list){
	if(list==NULL) return;
	if(list->dir=='E'){
		list->min.x+=PROJ_SPEED;
		list->max.x+=PROJ_SPEED;
	}
	else if(list->dir=='W'){
		list->min.x-=PROJ_SPEED;
		list->max.x-=PROJ_SPEED;
	}
	return moveForwardProjectiles(world, list->next);
}

/* retourne 1 si un des éléments de la liste entre en collision avec le.a joueur.se
 * retourne 0 sinon */
int colideList(Object *player, Object *list){
	if(list==NULL) return 0;
	if(colide(player->min, player->max, list->min, list->max)==0)
		return colideList(player, list->next);
	return 1;
}

/* retourne 1 si un des éléments de la liste entre en collision avec le.a joueur.se
 * retourne 0 sinon */
int colideProjList(Object *player, Projectile *list){
	if(list==NULL) return 0;
	if(colide(player->min, player->max, list->min, list->max)==0)
		return colideProjList(player, list->next);
	return 1;
}

/* retourne 1 si le.a joueur.se MEURT
 * retourne 0 sinon */
int isDeathEnd(World *world){
	int res=0;
	if(
		colideList(world->player, world->obstacles)!=0 ||
		colideProjList(world->player, world->projectiles)!=0 ||
		colideList(world->player, world->ennemies)!=0
	)
		res=1;
	return res;
}

void setPlayer(World *world){
	int pixelSize=(int)WINDOW_HEIGHT/(world->ppm->height);
	
	Coord playerMin, playerMax;
	playerMin.x=2*pixelSize;
	playerMin.y=WINDOW_HEIGHT/2-pixelSize/2;
	playerMax.x=playerMin.x+pixelSize;
	playerMax.y=playerMin.y+pixelSize;
	Object *player=initObject(playerMin, playerMax, 'j', world->playerTexture);
	world->player=addObject(player, world->player);
}

void fire(World *world, Object *obj, char dir){
	int pixelSize=WINDOW_HEIGHT/world->ppm->height;
	Coord coordProjMin, coordProjMax;
	coordProjMin.x=obj->min.x+pixelSize;
	coordProjMin.y=obj->min.y;
	coordProjMax.x=coordProjMin.x+pixelSize;
	coordProjMax.y=coordProjMin.y+pixelSize;
	Projectile *proj=initProjectile(coordProjMin, coordProjMax, dir);
	world->projectiles=addProjectile(proj, world->projectiles);
}

/* supprime les projectiles rencontrant un obstacle */
void projKilledByWall(World *world, Projectile **projList){
	if(*projList==NULL) return;
	projKilled(projList, world->obstacles);
	if(*projList==NULL) return;
	return projKilledByWall(world, &((*projList)->next));
}

/* proj supposé non NULL */
void projKilled(Projectile **proj, Object *obsList){
	if(obsList==NULL) return;
	if(colide((*proj)->min, (*proj)->max, obsList->min, obsList->max)==1){
		Projectile *temp;
		temp=(*proj)->next;
		free(*proj);
		(*proj)=temp;
		return;
	}
	return projKilled(proj, obsList->next);	
}

/* supprime les ennemis rencontrant un projectile */
void ennemiesKilled(World *world, Object **eList){
	if(*eList==NULL) return;
	ennemyKilled(eList, &(world->projectiles));
	if(*eList==NULL) return;
	return ennemiesKilled(world, &((*eList)->next));
}

void ennemyKilled(Object **ennemy, Projectile **projList){
	if(*projList==NULL) return;
	if(colide((*ennemy)->min, (*ennemy)->max, (*projList)->min, (*projList)->max)==1){
		Object *temp;
		temp=(*ennemy)->next;
		free(*ennemy);
		(*ennemy)=temp;
		
		Projectile *tempB;
		tempB=(*projList)->next;
		free(*projList);
		(*projList)=tempB;
		return;
	}
	return ennemyKilled(ennemy, &((*projList)->next));	
}
