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
int moveObject(Object *obj, char dir, int pixel_size){
	if(obj==NULL) return EXIT_FAILURE;
	switch(dir){
		case 'N':
			obj->min.y-=pixel_size;
			obj->max.y-=pixel_size;
		break;
		case 'S':
			obj->min.y+=pixel_size;
			obj->max.y+=pixel_size;
		break;
		case 'E':
			obj->min.x+=pixel_size;
			obj->max.x+=pixel_size;
		break;
		case 'W':
			obj->min.x-=pixel_size;
			obj->max.x-=pixel_size;
		break;
		
		default:
			return EXIT_FAILURE;
		break;
	}
	return EXIT_SUCCESS;
}

/* dir : 'N':North - 'S':South - 'E':East - 'W':West
 * Return EXIT_FAILURE if proj is NULL, or dir invalid
 * Do not manage the possible exit of the level map */
int moveProjectile(Projectile *proj, char dir, int pixel_size){
	if(proj==NULL) return EXIT_FAILURE;
	switch(dir){
		case 'N':
			proj->min.y+=pixel_size;
			proj->max.y+=pixel_size;
		break;
		case 'S':
			proj->min.y-=pixel_size;
			proj->max.y-=pixel_size;
		break;
		case 'E':
			proj->min.x+=pixel_size;
			proj->max.x+=pixel_size;
		break;
		case 'W':
			proj->min.x-=pixel_size;
			proj->max.x-=pixel_size;
		break;
		
		default:
			return EXIT_FAILURE;
		break;
	}
	return EXIT_SUCCESS;
}

/*
  Fonction de test des collisions 
  retourne 1 si les objets entrent en collisions
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

void moveForwardPlayer(Object *player){
	player->min.x+=SPEED;
	player->max.x+=SPEED;
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
