#ifndef GAME_H
#define GAME_H

#include "../include/data_struct"

/*--------------- PROTOTYPES DES FONCTIONS ---------------*/
World *initGame();
void exitGame(World *world);
void gameLoop(World *world);
int eventLoop();
void setPlayer(World *world);
void happyEnd(World *world);

#endif
