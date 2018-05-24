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
	
	obj1->next=NULL;
	obj2->next=NULL;
	
	world->end=NULL;
	world->player=obj2;
	world->ennemies=NULL;
	world->obstacles=obj1;
	world->projectiles=NULL;
	world->position=WINDOW_WIDTH/2;
	/* FIN TEST */
	
	loadAllTextures(world);
	
	/* TEST */
	obj1->textureID=world->obstacleTexture;
	obj2->textureID=world->ennemyTexture;
	/* FIN TEST */
	
	return world;

}

void gameLoop(World *world){
	int loop = 1;
	while(loop) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
		
		/* Actions */
		moveForwardPlayer(world->player);
		
		/* Boucle traitant les evenements */
		loop=eventLoop(world);
		
		/* Placer ici le code de dessin */
		displayAll(world);
		
		
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
			break;
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
					if(world->player->min.y-PIXEL_SIZE>=0)
						moveObject(world->player, 'N');
				}
				else if(e.key.keysym.sym==274){ /* flèche du bas */
					if(world->player->max.y+PIXEL_SIZE<=WINDOW_HEIGHT)
						moveObject(world->player, 'S');
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

void ppmToWorld(World *world, Picture *p)
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
