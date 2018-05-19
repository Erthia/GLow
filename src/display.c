#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_image.h>

#include "../include/display.h"
#define EXT ".jpg"

void initGame(){
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
}

void initWindow(){
	/* Ouverture d'une fenêtre et création d'un contexte OpenGL */
	if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER)) {
		fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
		exit(1);
	}
	/* Titre de la fenêtre */
	SDL_WM_SetCaption("GLow", NULL);
}

void displayLoop(){
	int loop = 1;
	int windowPosition=0;
	while(loop) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
    
		/* Placer ici le code de dessin */
    
		/* Echange du front et du back buffer : mise à jour de la fenêtre */
    
		/* Boucle traitant les evenements */
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			/* L'utilisateur ferme la fenêtre : */
			if(e.type == SDL_QUIT) {
				loop = 0;
				break;
			}
      
			/* Quelques exemples de traitement d'evenements : */
			switch(e.type) {
				/* Clic souris */
				case SDL_MOUSEBUTTONUP:
				printf("clic en (%d, %d)\n", e.button.x, e.button.y);
				break;
          
				/* Touche clavier */
				case SDL_KEYDOWN:
				printf("touche pressée (code = %d)\n", e.key.keysym.sym);
				break;
          
				default:
				break;
			}
		}
		/* Calcul du temps écoulé */
		Uint32 elapsedTime = SDL_GetTicks() - startTime;
		/* Si trop peu de temps s'est écoulé, on met en pause le programme */
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}
		
		windowPosition++;
	}
}

/* type : "j":joueur | "e":ennemi | "p":projectile | "o":obstacle | "f":fond du niveau | "l":fin du niveau */
/* libérer l'espace mémoire de GLuint *textureID : void freeTexture(GLuint *textureID) */
void loadTexture(char type, GLuint *textureID){
	SDL_Surface* textureData;
	char fileName[10];
	
	fileName[0]=type;
	strcat(fileName,EXT);
	
	/* chargement des données de la texture en RAM */
	if(textureData=IMG_Load(fileName)==NULL){
		fprintf(stderr, "Texture loading in memory failed");
		exit(1);
	}
	
	glGenTextures(1, textureID); /* Initialisation de la texture */
	
	/* Utilisation d'un point de bind pour modifier les paramètres de la texture */
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* filtre de minification */
	
	/* Envoi des données à la carte graphique */
	gltexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		textureData->w,
		textureData->h,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		textureData->pixels
	);
	
	glBindTexture(GL_TEXTURE_2D, 0); /* on debind la texture */
	SDL_FreeSurface(textureData); /* libération espace mémoire */
}

void freeTexture(GLuint *textureID){
	glDeleteTextures(1, textureID);
}

void displayAll(World *world){
	glClear(GL_COLOR_BUFFER_BIT);
	displayLevel(world);
	displayObjects(world);
	displayProjectiles(world);
}

/* PPM :
 * joueur : rouge
 * obstacles : vert
 * ennemis : bleu
 * projectiles : noir */
void displayLevel(World *world){
	
}
