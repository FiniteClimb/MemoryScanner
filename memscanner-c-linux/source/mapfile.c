/*
Memory Scanner (C / Linux)
Copyright (c) 2025 FiniteClimb
License: Research and Learning Only – see LICENSE file for full details
*/

#include "mapfile.h"

#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globaltypes.h"

int openmapfile(FILE** mapfile, int pid) {
  if (*mapfile) fclose(*mapfile);
  char mapfileloc[30];
  snprintf(mapfileloc, sizeof(mapfileloc), "/proc/%i/maps", pid);
  *mapfile = fopen(mapfileloc, "r");

  if (!*mapfile) {
    perror("Failed to open map:");
    return -1;
  }
  return 0;
}

void parsemapfile(struct MapInfo* map[], FILE* mapfile, int* region_count) {
  char mapsparse[500] = {0};
  char perms[5];

  int n = 0;

  int parsedcount = 0;

  while (fgets(mapsparse, sizeof(mapsparse), mapfile)) {
    parsedcount = sscanf(mapsparse, "%lx-%lx %s %*s %*s %*s %s", &map[n]->start,
                         &map[n]->end, perms, map[n]->name);

    if (parsedcount < 4) continue;

    if (parsedcount == 3) {
      strcpy(map[n]->name, "UNNAMED_REGION\0");
    }

    if (map[n]->start < 100000 || map[n]->end < 100000) continue;

    if (!strstr(perms, "rw")) continue;

    if (strstr(perms, "x")) continue;

    if (strstr(map[n]->name, "lib")) continue;

    map[n]->size = map[n]->end - map[n]->start + 1;

    ++n;

    if (n == MAX_MAP_REGION) break;
  }
  *region_count = n;
}

int mallocmapstruct(struct MapInfo* map[]) {
  for (int i = 0; i < MAX_MAP_REGION; ++i) {
    map[i] = malloc(MAX_MAP_REGION_SIZE);
    if (!map[i]) {
      printf("Malloc for map number %i failed.", i);
      return -1;
    }
  }
  return 0;
}

void freemapstruct(struct MapInfo* map[], int hitcount) {
  for (int i = 0; i < hitcount; ++i) {
    free(map[i]);
  }
}
