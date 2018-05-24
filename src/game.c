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
	ppmToWorld("elements/level1.ppm", world);
	printf("%d\n",world->ppm->height);
	setPlayer(world);
	
	return world;
}

void gameLoop(World *world){
	int loop = 1;
	while(loop) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		/* ACTIONS */
		moveForwardPlayer(world->player);
		
		/* Boucle traitant les evenements */
		loop=eventLoop(world);
		
		/* Placer ici le code de dessin */
		if(isHappyEnd(world->player, world->end)==1){
			happyEnd(world);
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
				if(e.key.keysym.sym==32){ /* barre espace */

				}
				else if(e.key.keysym.sym==273){ /* flèche du haut */
					if(world->player->min.y-WINDOW_HEIGHT/world->ppm->height>=0)
						moveObject(world->player, 'N', WINDOW_HEIGHT/world->ppm->height);
				}
				else if(e.key.keysym.sym==274){ /* flèche du bas */
					if(world->player->max.y+WINDOW_HEIGHT/world->ppm->height<=WINDOW_HEIGHT)
						moveObject(world->player, 'S', WINDOW_HEIGHT/world->ppm->height);
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

void happyEnd(World *world){
	int loop=1;
	glMatrixMode(GL_MODELVIEW); /* select the current matrix */
	glLoadIdentity(); /* the current matrix becomes the identity matrix */
	displayBackground(world->happyEndTexture);
	SDL_GL_SwapBuffers();
	while(loop){
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			/* L'utilisateur ferme la fenêtre : */
			if(e.type == SDL_QUIT)
				loop = 0;
		}
	}
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
          world->obstacles = addObject(
                                       initObject(min, max, 'e', world->obstacleTexture),
                                       world->obstacles);
        }
      else //Pixel noir
        {
	  xtab = i%(p->width);
          ytab = i/(p->width);
          min.x = xtab*(WINDOW_HEIGHT/(p->height));
          min.y	= ytab*(WINDOW_HEIGHT/(p->height));
          max.x	= (1+xtab)*(WINDOW_HEIGHT/(p->height));
          max.y	= (1+ytab)*(WINDOW_HEIGHT/(p->height));
          world->obstacles = addObject(
                                       initObject(min, max, 'l', world->obstacleTexture),
                                       world->obstacles);
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
