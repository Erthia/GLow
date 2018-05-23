#ifndef DISPLAY_H
#define DISPLAY_H

#include "../include/data_struct.h"

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;
/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;
/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/*--------------- PROTOTYPES DES FONCTIONS ---------------*/
void initWindow();
/* type : "j":joueur | "e":ennemi | "p":projectile | "o":obstacle | "b":fond du niveau | "l":fin du niveau */
GLuint *loadTexture(char type);
void loadAllTextures(World *world);
void freeTexture(GLuint *textureID);
void displayAll(World *world);
void displayBackground(World world);
void displayObjects(Object *obj);
void displayProjectiles(World world);
void drawObjectBlock(Object *obj);
void drawProjectile(Projectile *proj, GLuint texture);

#endif
