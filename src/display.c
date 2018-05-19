#include <stdio.h>
#include <SDL/SDL_image.h>

#define EXT ".jpg"

/* libérer l'espace mémoire de GLuint *textureID : void freeTexture(GLuint *textureID) */
void loadObjectTexture(char objectType, GLuint *textureID){
	SDL_Surface* textureData;
	char fileName[10];
	
	fileName[0]=object.type;
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
