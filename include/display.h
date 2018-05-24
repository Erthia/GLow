#ifndef DISPLAY_H
#define DISPLAY_H

#include "../include/data_struct.h"

/*--------------- PROTOTYPES DES FONCTIONS ---------------*/
void initWindow();
/* type : "j":joueur | "e":ennemi | "p":projectile | "o":obstacle | "b":fond du niveau | "l":fin du niveau */
GLuint *loadTexture(char *type);
void loadAllTextures(World *world);
void freeTexture(GLuint *textureID);
void displayAll(World *world);
void displayBackground(GLuint *textureID);
void displayObjects(Object *obj);
void displayProjectiles(World world);
void drawObjectBlock(Object *obj);
void drawProjectile(Projectile *proj, GLuint texture);
void initCamera(); /* inutilisée */
void displayEnd(World *world, int end);

#endif
