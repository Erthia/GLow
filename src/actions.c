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

/* gère les rencontres avec les projectiles, et les suppressions d'éléments en découlant */
void meetProjectiles(World *world, Projectile **projList){
	if(*projList==NULL) return; /* dernier projectile traité */
	/* gère la rencontre avec les obstacles */
	projKilledbyObs(projList, world->obstacles); 
	
	if(*projList==NULL) return; /* dernier projectile traité */
	/* gère la rencontre avec les autres projectiles */
	projKilledbyProj(projList, &(world->projectiles)); 
	
	if(*projList==NULL) return; /* dernier projectile traité */
	/* gère la rencontre avec les ennemis */
	ennemyKilled(projList, &(world->ennemies));
	
	
	if(*projList==NULL) return; /* dernier projectile traité */
	/* gère la sortie de terrain des projectiles */
	projOut(world, projList);
	
	if(*projList==NULL) return; /* dernier projectile traité */
	return meetProjectiles(world, &((*projList)->next));
}

/* gère la sortie de terrain du projectile */
void projOut(World *world, Projectile **proj){
	int pixelSize=WINDOW_HEIGHT/world->ppm->height;
	if(
		(*proj)->min.x<world->position-WINDOW_WIDTH/2 ||
		(*proj)->max.x>world->position+WINDOW_WIDTH-WINDOW_WIDTH/2 ||
		(*proj)->min.y<0 ||
		(*proj)->min.y>WINDOW_WIDTH
	)
	deleteProj(proj);
}

/* gère la rencontre d'un projectile donné avec les obstacles */
/* proj supposé non NULL */
void projKilledbyObs(Projectile **proj, Object *obsList){
	if(obsList==NULL) return;
	if(colide((*proj)->min, (*proj)->max, obsList->min, obsList->max)==1){
		deleteProj(proj);
		return;
	}
	if(obsList==NULL) return;
	return projKilledbyObs(proj, obsList->next);	
}

/* gère la rencontre d'un projectile donné avec les ennemis */
/* proj supposé non NULL */
void ennemyKilled(Projectile **proj, Object **ennemies){
	if(*ennemies==NULL) return;
	if(colide((*ennemies)->min, (*ennemies)->max, (*proj)->min, (*proj)->max)==1){
		deleteEnnemi(ennemies);
		deleteProj(proj);
		return;
	}
	if(*ennemies==NULL) return;
	ennemyKilled(proj, &((*ennemies)->next));	
}

/* gère la rencontre d'un projectile donné avec les autre projectiles */
/* proj supposé non NULL */
void projKilledbyProj(Projectile **proj, Projectile **list){
	if(*list==NULL) return;
	if(
		colide((*list)->min, (*list)->max, (*proj)->min, (*proj)->max)==1 &&
		(*list)->min.x!=(*proj)->min.x
	){
		deleteProj(list);
		deleteProj(proj);
		return;
	}
	
	return projKilledbyProj(proj, &(*list)->next);	
}

void deleteProj(Projectile **proj){
	Projectile *temp;
	temp=(*proj)->next;
	free(*proj);
	(*proj)=temp;
	
	return;
}

void deleteEnnemi(Object **ennemies){
	Object *temp;
	temp=(*ennemies)->next;
	free(*ennemies);
	(*ennemies)=temp;
	
	return;
}

/* fait bouger et tirer les ennemis */
void moveEnnemies(World *world){
	
}
