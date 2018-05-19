#ifndef DISPLAY_H
#define DISPLAY_H

/* type : "j":joueur | "e":ennemi | "p":projectile | "o":obstacle | "f":fond du niveau */
/* libérer l'espace mémoire de GLuint *textureID : void freeTexture(GLuint *textureID) */
void loadTexture(char type, GLuint *textureID);
void freeTexture(GLuint *textureID);

#endif
