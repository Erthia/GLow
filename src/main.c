#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "../include/data_struct.h"
#include "../include/display.h"

int main(int argc, char** argv) {
	
	World *world=initGame();
	displayLoop(world);
	exitGame(world);
  
	return EXIT_SUCCESS;
}
