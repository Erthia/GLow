#ifndef DATA_STRUCT_H /* si la variable n'est pas définie, le code est pris en compte */
#define DATA_STRUCT_H /* on défini la variable mentionnée ci-dessus */

typedef struct{
  float x,y;
}Coord;

typedef struct object{
  Coord min, max;
  char type; /* 'j':player - 'e':ennemy - 'o':obstacle */
  GLuint *textureID;
  struct object *next;
}Object;

typedef struct projectile{
  Coord min, max;
  char dir; /* 'N':North - 'S':South - 'E':East - 'W':West */
  struct projectile *next;
}Projectile;

typedef struct{
	Projectile *projectiles;
	Object *objects;
	GLuint *obstacleTexture;
	GLuint *ennemyTexture;
	GLuint *projectileTexture;
	GLuint *playerTexture;
	GLuint *backgroundTexture;
}World;

#endif
