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
#define SPEED 5 /* pixels per image */


void initWindow(){
	/* Ouverture d'une fenêtre et création d'un contexte OpenGL */
	if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER)) {
		fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
		exit(1);
	}
	/* Titre de la fenêtre */
	SDL_WM_SetCaption("GLow", NULL);

}


/* FINI */
/* type : "j":joueur | "e":ennemi | "p":projectile | "o":obstacle | "b":fond du niveau | "l":fin du niveau */
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

/* a tester */
void displayAll(World *world){
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW); /* select the current matrix */
	glLoadIdentity(); /* the current matrix becomes the identity matrix */
	
	glTranslatef(-(-1+2.*world->position/WINDOW_WIDTH), 0, 0);
	world->position+=SPEED;

	displayBackground(*world);
	displayObjects(world->end);
	displayObjects(world->player);
	displayObjects(world->ennemies);
	displayProjectiles(*world);
	displayObjects(world->obstacles);
		
	
}

/* FINI */
void displayBackground(World world){
	glEnable(GL_TEXTURE_2D); /* active la fonctionnalité de texturing */
	glBindTexture(GL_TEXTURE_2D, *world.levelTexture); /* on bind la texture pour pouvoir l'utiliser */
	glBegin(GL_POLYGON);
		glTexCoord2f(0,0);
		glVertex2f(-1, 1);
		glTexCoord2f(1,0);
		glVertex2f(1, 1);
		glTexCoord2f(1,1);
		glVertex2f(1, -1);
		glTexCoord2f(0,1);
		glVertex2f(-1, -1);
	glEnd();
	glDisable(GL_TEXTURE_2D); /* désactive la fonctionnalité de texturing */
	glBindTexture(GL_TEXTURE_2D, 0); /* débind la texture */
}

/* FINI */
void displayObjects(Object *obj){
	while(obj!=NULL){
		drawObjectBlock(obj);
		obj=obj->next;
	}
}

/* FINI */
void displayProjectiles(World world){
	Projectile *pProj=world.projectiles;
	while(pProj!=NULL){
		drawProjectile(pProj, *world.projectileTexture);
		pProj=pProj->next;
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

/* inutilisée */
void initCamera(){
	glMatrixMode(GL_MODELVIEW);
	int centreX=-1+2.*0.5, centreY=-(-1+2.*0.5);
	gluLookAt(centreX, centreY, 0.1, centreX, centreY, 0, 0, 1, 0);
};
