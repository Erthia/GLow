#ifndef ACTIONS_H
#define ACTIONS_H

#include "../include/data_struct.h"

/*--------------- PROTOTYPES DES FONCTIONS ---------------*/

/* dir : 'N':North - 'S':South - 'E':East - 'W':West
 * Return EXIT_FAILURE if obj is NULL, or dir invalid
 * Do not manage the possible exit of the level map */
int moveObject(Object *obj, char dir);
/* dir : 'N':North - 'S':South - 'E':East - 'W':West
 * Return EXIT_FAILURE if proj is NULL, or dir invalid
 * Do not manage the possible exit of the level map */
int moveProjectile(Projectile *proj, char dir);
int colide(Coord minObj1, Coord maxObj1, Coord minObj2, Coord maxObj2);
/*
  Fonction de test des collisions
  retourne 1 si les objets entrent en collisions
  retourne 0 sinon
*/
void moveForwardPlayer(Object *player);
/* return 1 if this is the end
 * return 0 if isn't */
int isHappyEnd(Object *player, Object *end);

#endif
