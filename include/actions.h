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

#endif
