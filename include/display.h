#ifndef DISPLAY_H
#define DISPLAY_H

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

/*--------------- PROTOTYPES DES FONCTIONS ---------------*/
void initWindow()
void initGame();
/* type : "j":joueur | "e":ennemi | "p":projectile | "o":obstacle | "f":fond du niveau */
/* libérer l'espace mémoire de GLuint *textureID : void freeTexture(GLuint *textureID) */
void loadTexture(char type, GLuint *textureID);
void freeTexture(GLuint *textureID);

#endif
