#include <stdio.h>
#include <SDL/SDL_image.h>
#include "../include/display.h"

#define EXT ".jpg"

void displayAll(World *world){
	displayLevel(world);
	displayObjects(world);
	displayProjectiles(world);
}

/* type : "j":joueur | "e":ennemi | "p":projectile | "o":obstacle | "f":fond du niveau */
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
