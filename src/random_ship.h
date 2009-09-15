#pragma once
#define MAX_CELL_X  13 //must be odd
#define MAX_CELL_Y  10
#include "actor.h"

struct random_ship
{
	float pos[2];
	float oldpos[2];
	int cells[MAX_CELL_X][MAX_CELL_Y];
	int w, h;
	float rot;

};

typedef struct random_ship random_ship_t;

void SHIP_render(const struct actor_t* e);
void SHIP_generate(struct actor_t* a, int seed, int x, int y);
