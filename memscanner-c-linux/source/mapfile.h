/*
Memory Scanner (C / Linux)
Copyright (c) 2025 FiniteClimb
License: Research and Learning Only – see LICENSE file for full details
*/

#ifndef MAP_H
#define MAP_H

#include <stdio.h>

#include "globaltypes.h"

int openmapfile(FILE **mapfile, int pid);

int mallocmapstruct(struct MapInfo *map[]);

void parsemapfile(struct MapInfo *map[], FILE *mapfile, int *region_count);

void freemapstruct(struct MapInfo *map[], int regioncount);

#endif
