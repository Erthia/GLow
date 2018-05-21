#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_image.h>
#include <string.h>

#include "../include/display.h"
#include "../include/data_struct.h"
#define EXT ".jpg"

void initWindow(){
	/* Ouverture d'une fenêtre et création d'un contexte OpenGL */
	if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER)) {
		fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
		exit(1);
	}
	/* Titre de la fenêtre */
	SDL_WM_SetCaption("GLow", NULL);
}

void displayLoop(World *world){
	int loop = 1;
	while(loop) {
		/* Récupération du temps au début de la boucle */
		Uint32 startTime = SDL_GetTicks();
    
		/* Placer ici le code de dessin */
		glClear(GL_COLOR_BUFFER_BIT);
		/* TEST */
		displayObjects(*world);
		drawProjectile(world->projectiles, *world->projectileTexture);
		/* FIN TEST */
    
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
		SDL_GL_SwapBuffers();
		/* Calcul du temps écoulé */
		Uint32 elapsedTime = SDL_GetTicks() - startTime;
		/* Si trop peu de temps s'est écoulé, on met en pause le programme */
		if(elapsedTime < FRAMERATE_MILLISECONDS) {
			SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
		}
		
	}
}

/* FINI */
/* type : "j":joueur | "e":ennemi | "p":projectile | "o":obstacle | "b":fond du niveau | "l":fin du niveau */
/* libérer l'espace mémoire de GLuint * : void freeTexture(GLuint *textureID) */
GLuint *loadTexture(char type){
	SDL_Surface* textureData;
	char fileName[50]="elements/";
	strcat(fileName, &type);
	strcat(fileName, EXT);
	
	
	GLuint *textureID=malloc(sizeof(GLuint));
	if(textureID==NULL){
		fprintf(stderr, "Malloc of textureID in fonction loadTexture failed\n");
		exit(1);
	}
	
	/* chargement des données de la texture en RAM */
	textureData=IMG_Load(fileName);
	/* TEST */printf(fileName);
	if(textureData==NULL){
		fprintf(stderr, "Texture loading in memory failed\n");
		exit(1);
	}
	glGenTextures(1, textureID); /* Initialisation de la texture */
	
	/* Utilisation d'un point de bind pour modifier les paramètres de la texture */
	glBindTexture(GL_TEXTURE_2D, *textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* filtre de minification */
	
	/* Envoi des données à la carte graphique */
	glTexImage2D(
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
	
	return textureID;
}

/* FINI */
void loadAllTextures(World *world){
	world->obstacleTexture=loadTexture('o');
	world->ennemyTexture=loadTexture('e');
	world->projectileTexture=loadTexture('p');
	world->playerTexture=loadTexture('j');
	world->levelTexture=loadTexture('b');
	world->endTexture=loadTexture('l');
}

void freeTexture(GLuint *textureID){
	glDeleteTextures(1, textureID);
}

/* PAS FINI
 * PPM :
 * joueur : rouge
 * obstacles : vert
 * ennemis : bleu
 * projectiles : noir 
void displayAll(World *world){
	glClear(GL_COLOR_BUFFER_BIT);
	displayLevel(world);
	displayObjects(*world);
	displayProjectiles(world);
}*/

void displayLevel(World *world){
	
}

/* FINI */
void displayObjects(World world){
	/* parcourir la liste chaînée */
	Object *pObj=world.objects;
	while(pObj!=NULL){
		drawObjectBlock(pObj);
		pObj=pObj->next;
	}
}

/* FINI */
void drawObjectBlock(Object *obj){
	glEnable(GL_TEXTURE_2D); /* active la fonctionnalité de texturing */
	glBindTexture(GL_TEXTURE_2D, *obj->textureID); /* on bind la texture pour pouvoir l'utiliser */
	glBegin(GL_POLYGON);
		glTexCoord2f(0,0);
		glVertex2f(-1+2.*obj->min.x/WINDOW_WIDTH, -(-1+2.*obj->min.y/WINDOW_HEIGHT));
		glTexCoord2f(1,0);
		glVertex2f(-1+2.*obj->max.x/WINDOW_WIDTH, -(-1+2.*obj->min.y/WINDOW_HEIGHT));
		glTexCoord2f(1,1);
		glVertex2f(-1+2.*obj->max.x/WINDOW_WIDTH, -(-1+2.*obj->max.y/WINDOW_HEIGHT));
		glTexCoord2f(0,1);
		glVertex2f(-1+2.*obj->min.x/WINDOW_WIDTH, -(-1+2.*obj->max.y/WINDOW_HEIGHT));
	glEnd();
	glDisable(GL_TEXTURE_2D); /* désactive la fonctionnalité de texturing */
	glBindTexture(GL_TEXTURE_2D, 0); /* débind la texture */
}

/* FINI */
void drawProjectile(Projectile *proj, GLuint texture){
	glEnable(GL_TEXTURE_2D); /* active la fonctionnalité de texturing */
	glBindTexture(GL_TEXTURE_2D, texture); /* on bind la texture pour pouvoir l'utiliser */
	glBegin(GL_POLYGON);
		glTexCoord2f(0,0);
		glVertex2f(-1+2.*proj->min.x/WINDOW_WIDTH, -(-1+2.*proj->min.y/WINDOW_HEIGHT));
		glTexCoord2f(1,0);
		glVertex2f(-1+2.*proj->max.x/WINDOW_WIDTH, -(-1+2.*proj->min.y/WINDOW_HEIGHT));
		glTexCoord2f(1,1);
		glVertex2f(-1+2.*proj->max.x/WINDOW_WIDTH, -(-1+2.*proj->max.y/WINDOW_HEIGHT));
		glTexCoord2f(0,1);
		glVertex2f(-1+2.*proj->min.x/WINDOW_WIDTH, -(-1+2.*proj->max.y/WINDOW_HEIGHT));
	glEnd();
	glDisable(GL_TEXTURE_2D); /* désactive la fonctionnalité de texturing */
	glBindTexture(GL_TEXTURE_2D, 0); /* débind la texture */
}
