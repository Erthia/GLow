#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_image.h>

#include "../include/game.h"
#include "../include/data_struct.h"
#include "../include/display.h"

World *initGame(){
	/* Initialisation de la SDL */
	if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
		exit(1);
	}
	
	initWindow();
	
	/* TEST */
	World *world=malloc(sizeof(World));
	Object *obj1=malloc(sizeof(Object));
	Object *obj2=malloc(sizeof(Object));
	obj1->min.x=50;
	obj1->min.y=50;
	obj1->max.x=150;
	obj1->max.y=150;
	
	obj2->min.x=150;
	obj2->min.y=50;
	obj2->max.x=250;
	obj2->max.y=150;
	
	obj1->next=obj2;
	obj2->next=NULL;
	
	world->objects=obj1;
	
	Projectile *proj=malloc(sizeof(Projectile));
	proj->min.x=300;
	proj->min.y=50;
	proj->max.x=400;
	proj->max.y=150;
	
	world->projectiles=proj;
	/* FIN TEST */
	
	loadAllTextures(world);
	
	/* TEST */
	obj1->textureID=world->obstacleTexture;
	obj2->textureID=world->ennemyTexture;
	/* FIN TEST */

	glClearColor(0.5, 0.5, 0.5 ,1.0); /* couleur du fond (sert pour les tests) */
	
	return world;

}

void exitGame(World *world){
	SDL_Quit(); /* Liberation des ressources associées à la SDL */
	/* TEST */
	freeTexture(world->obstacleTexture);
	free(world->objects);
	free(world);
	/* FIN TEST */
}
