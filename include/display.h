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
void displayLoop(World *world);
/* type : "j":joueur | "e":ennemi | "p":projectile | "o":obstacle | "f":fond du niveau */
/* libérer l'espace mémoire de GLuint *textureID : void freeTexture(GLuint *textureID) */
GLuint loadTexture(char type);
void loadAllTextures(World world);
void freeTexture(GLuint *textureID);
void displayAll(World *world);
void displayLevel(World *world);
void displayObjects(World world);
void drawObjectBlock(Object *obj);

#endif
