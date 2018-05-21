#ifndef DATA_STRUCT_H /* si la variable n'est pas définie, le code est pris en compte */
#define DATA_STRUCT_H /* on défini la variable mentionnée ci-dessus */

typedef struct{
  float x,y;
}Coord;

typedef struct object{
  Coord min, max;
  char type; /* 'j':player - 'e':ennemy - 'o':obstacle - 'l':end line of the level */
  GLuint *textureID; /* les textures seront chargées lors du lancement du jeu, et stockées dans la structure world. Plutôt que de tester à chaque affichage quelle texture doit être utilisée,
						on le fera une bonne fois pour toute à la création de l'objet, et on stockera le pointeur vers la texture ici */
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
	GLuint *levelTexture;
	GLuint *endTexture;
}World;

#endif
