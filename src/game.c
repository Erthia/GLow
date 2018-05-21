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
	Object *obj=malloc(sizeof(Object));
	obj->min.x=50;
	obj->min.y=50;
	obj->max.x=150;
	obj->max.y=150;
	
	obj->textureID=loadTexture('o');
	
	world->objects=obj;
	
	/* FIN TEST */
	
	/* ouverture du fichier ppm
	FILE *ppm=fopen("level.ppm","r");
	if(ppm==NULL){
		fprintf(stderr, "Ouverture du fichier ppm a échouée");
		exit(1);
	}
	* */

	
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
