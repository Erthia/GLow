#ifndef ACTIONS_H
#define ACTIONS_H

#include "../include/data_struct.h"

/*--------------- PROTOTYPES DES FONCTIONS ---------------*/

/* dir : 'N':North - 'S':South - 'E':East - 'W':West
 * Return EXIT_FAILURE if obj is NULL, or dir invalid
 * Do not manage the possible exit of the level map */
int moveObject(Object *obj, char dir, int pixel_size);
/* dir : 'N':North - 'S':South - 'E':East - 'W':West
 * Return EXIT_FAILURE if proj is NULL, or dir invalid
 * Do not manage the possible exit of the level map */
int moveProjectile(Projectile *proj, char dir, int pixel_size);
int colide(Coord minObj1, Coord maxObj1, Coord minObj2, Coord maxObj2);
/* retourne 1 si un des éléments de la liste entre en collision avec le.a joueur.se
 * retourne 0 sinon */
int colideList(Object *player, Object *list);
/*
  Fonction de test des collisions
  retourne 1 si les objets entrent en collisions
  retourne 0 sinon
*/
/* retourne 1 si un des éléments de la liste entre en collision avec le.a joueur.se
 * retourne 0 sinon */
int colideProjList(Object *player, Projectile *list);
void moveForwardPlayer(Object *player);
/* retourne 1 si le.a joueur.se MEURT
 * retourne 0 sinon */
int isDeathEnd(World *world);
void setPlayer(World *world);


#endif
