#ifndef DATA_STRUCT_H /* si la variable n'est pas définie, le code est pris en compte */
#define DATA_STRUCT_H /* on défini la variable mentionnée ci-dessus */

/* ---------------- GLOBAL VARIABLES ----------------*/
#define EXT ".jpg"
#define SPEED 1 /* pixels per image INT */
#define PIXEL_SIZE 50
/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 1280;
static unsigned int WINDOW_HEIGHT = 720;
/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;
/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

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
	int position;
	Projectile *projectiles;
	Object *player;
	Object *ennemies;
	Object *obstacles;
	Object *end;
	GLuint *obstacleTexture;
	GLuint *ennemyTexture;
	GLuint *projectileTexture;
	GLuint *playerTexture;
	GLuint *levelTexture;
	GLuint *endLineTexture;
	GLuint *happyEndTexture;
	GLuint *deathEndTexture;
}World;

#endif
