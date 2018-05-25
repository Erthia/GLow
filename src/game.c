#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_image.h>

#include "../include/game.h"
#include "../include/data_struct.h"
#include "../include/display.h"
#include "../include/actions.h"
#include "../include/data.h"

World *initGame(){
	/* Initialisation de la SDL */
	if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
		exit(1);
	}
	
	initWindow();
	World *world=initWorld();
	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	ppmToWorld("elements/level1.ppm", world);

	setPlayer(world);
	
	return world;
}

void gameLoop(World *world){
	int loop = 1;
	while(loop) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		/* ACTIONS */
		moveObject(world->player, 'E', SPEED); /* fait avancer le joueur */
		moveForwardProjectiles(world, world->projectiles); /* fait avancer tous les projectiles du jeu */
		
		/* Boucle traitant les evenements */
		loop=eventLoop(world);
		
		/* CONDITIONS + DESSINS */
		projKilledByWall(world, &(world->projectiles)); /* supprime les projectiles rencontrant un obstacle */
		ennemiesKilled(world, &(world->ennemies)); /* rencontre ennemi-projectile -> les deux meurent */
		if(colideList(world->player, world->end)==1){ /* le joueur a atteint la fin du niveau */
			displayEnd(world, 1);
			loop=0;
		}
		else if(isDeathEnd(world)==1){ /* le joueur est mort */
			displayEnd(world, 0);
			loop=0;
		}
		else displayAll(world);
		
		
		SDL_GL_SwapBuffers();
		/* Calcul du temps écoulé */
		Uint32 elapsedTime = SDL_GetTicks() - startTime;
		/* Si trop peu de temps s'est écoulé, on met en pause le programme */
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}
		
	}
}

/* Boucle traitant les evenements : fermer la fenêtre, commandes vaisseau
 * Commandes vaisseau : flèche haut -> le vaisseau monte
 * 						flèche bas  -> le vaisseau descend
 * 						espace      -> tir
 * 
 * retourne 1 : la gameLoop doit se poursuivre
 * retourne 0 : la gameLoop doit s'arrêter
 * */
int eventLoop(World *world){
	int loop=1;
	int pixelSize=WINDOW_HEIGHT/world->ppm->height;
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		/* L'utilisateur ferme la fenêtre : */
		if(e.type == SDL_QUIT) {
			loop = 0;
		}
      
		switch(e.type) {
			/* Clic souris */
			case SDL_MOUSEBUTTONUP:
				printf("clic en (%d, %d)\n", e.button.x, e.button.y);
			break;
          
			/* Touche clavier */
			case SDL_KEYDOWN:
				printf("touche pressée (code = %d)\n", e.key.keysym.sym);
				if(e.key.keysym.sym==32) /* barre espace */
					fire(world, world->player, 'E');
				else if(e.key.keysym.sym==273){ /* flèche du haut */
					if(world->player->min.y-WINDOW_HEIGHT/world->ppm->height>=0)
						moveObject(world->player, 'N', WINDOW_HEIGHT/world->ppm->height);
				}
				else if(e.key.keysym.sym==274){ /* flèche du bas */
					if(world->player->max.y+pixelSize<=WINDOW_HEIGHT)
						moveObject(world->player, 'S', pixelSize);
				}
			break;
          
			default:
			break;
			}
		}
		return loop;
}

void exitGame(World *world){
	deleteWorld(world);
	SDL_Quit(); /* Liberation des ressources associées à la SDL */
}

