#ifndef DATA_H
#define DATA_H

Object *initObject(Coord min, Coord max, char type, GLuint *textureID);
Object *addObject(Object *o, Object *liste);
void deleteObjects(Object *liste);
Projectile *initProjectile(Coord min, Coord max, char dir);
Projectile *addProjectile(Projectile *p, Projectile *liste);
void deleteProjectiles(Projectile *liste);
World *initWorld();
void deleteWorld(World *w);
Picture *openPicture(char *fileName);
Picture *initPicture(int width, int height);
Picture *openPicture(char *fileName);
void pictureToWorld(World *world, Picture *p);
void ppmToWorld(char *filename, World *world);

#endif
