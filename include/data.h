#ifndef DATA_H
#define DATA_H

Object *initObject(Coord min, Coord max, char type, GLuint *textureID);
Object *addObject(Object *o, Object *liste);
void deleteObjects(Object *liste);
Projectile *initProjectile(Coord min, Coord max, char dir);
Projectile *addProjectile(Projectile *p, Projectile *liste);
void deleteProjectiles(Projectile *liste);
World *initWorld(GLuint *obstacleTexture, GLuint *ennemyTexture, GLuint *projectileTexture, GLuint *playerTexture, GLuint *levelTexture, GLuint *endTexture);
void deleteWorld(World *w);

#endif
