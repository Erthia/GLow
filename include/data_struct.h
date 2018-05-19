#ifndef DATA_STRUCT_H /* si la variable n'est pas définie, le code est pris en compte */
#define DATA_STRUCT_H /* on défini la variable mentionnée ci-dessus */

typedef struct{
  float x,y;
}Coord;

typedef struct{
  Coord min, max;
  char type; /* 'j':player - 'e':ennemy - 'o':obstacle */
  GLuint *textureID;
}Object;

typedef struct{
  Coord min, max;
  char dir; /* 'N':North - 'S':South - 'E':East - 'W':West */
}Projectile;

#endif
