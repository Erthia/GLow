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
	/* TEST */
	World *world;
	Object *obj=malloc(Object);
	obj->coord.x=0;
	obj->coord.y=3;
	
	world->objects=obj
	
	loadAllTextures(world);
	/* FIN TEST */
	
	/* ouverture du fichier ppm */
	FILE *ppm=fopen("level.ppm","r");
	if(ppm==NULL){
		fprintf(stderr, "Ouverture du fichier ppm a échouée");
		exit(1);
	}
	
	/* Initialisation de la SDL */
	if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
		return EXIT_FAILURE;
	}

	initWindow();
	
	return *world;

}

void exitGame(World *world){
	SDL_Quit(); /* Liberation des ressources associées à la SDL */
	/* TEST */
	freeTexture(world->obstacleTexture);
	free(world->objects);
	/* FIN TEST */
}
