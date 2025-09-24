/*
Memory Scanner (C / Linux)
Copyright (c) 2025 FiniteClimb
License: Research and Learning Only – see LICENSE file for full details
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globaltypes.h"
#include "mapfile.h"
#include "memfile.h"
#include "targetcomp.h"
#include "userio.h"

void initialcleanup(struct HitInfo* hit[], struct MapInfo* map[], int hitcount,
                    int regioncount);

void freeall(struct MapInfo* map[], struct HitInfo* hit[], int inithitcount,
             int regioncount);

void check_linux();

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Correct usage: ./[program] [pid]");
    return 1;
  }

  int pid = atoi(argv[1]);

  struct TargetInfo target = {0};

  struct MapInfo* map[MAX_MAP_REGION];
  char* readmem[MAX_MAP_REGION];
  struct HitInfo* hit[MAX_HIT_COUNT];

  FILE* memfile = NULL;
  FILE* mapfile = NULL;

  int region_count = 0;

  int initialhit_count = 0;
  int hit_count = 0;

  int edit_index = 0;

  int restart = 0;

  if (openmapfile(&mapfile, pid)) return 1;
  if (openmemfile(&memfile, pid)) return 1;

  introduction();

  while (TRUE) {
    gettargettype(&target);
    getvalue(&target, target.data.s);

    if (mallocmapstruct(map)) return 1;
    if (mallochits(hit)) return 1;

    for (int i = 0; i < MAX_HIT_COUNT; ++i) hit[i]->changed = 0;

    if (openmapfile(&mapfile, pid)) return 1;
    if (openmemfile(&memfile, pid)) return 1;

    parsemapfile(map, mapfile, &region_count);
    printmaps(map, region_count);

    if (mallocreadmem(map, readmem, region_count)) return 1;

    if (scanmemfile(map, readmem, memfile)) return 1;

    if (searchtarget(target, map, hit, readmem, region_count, &hit_count) ==
        NO_HITS) {
      freeall(map, hit, MAX_HIT_COUNT, MAX_MAP_REGION);

      if (restartfromzero(NO_HITS))
        continue;
      else
        return 1;
    }

    freereadmem(readmem, region_count);

    initialhit_count = hit_count;

    initialcleanup(hit, map, initialhit_count, region_count);

    while (TRUE) {
      printhits(hit, hit_count);

      resetchangedvalue(hit, hit_count);

      if (!rescan()) {
        edit_index = editindex(hit_count);
        getnewvalue(&target);
        writenewvalue(target, hit, memfile, edit_index);
        printhits(hit, hit_count);

        if (!returntolasthits())
          break;
        else
          continue;
      };

      if (openmemfile(&memfile, pid)) return 1;

      if (typeofrescan() == 'd') {
        if (openmemfile(&memfile, pid)) return 1;
        rescanhits(target, hit, memfile, hit_count, RELATIVE_COMPARE);
        printhits(hit, hit_count);

        if (keeptype(hit, &hit_count, keepwhich()) == -1) {
          break;
        }
      } else {
        getvalue(&target, target.data.s);
        if (openmemfile(&memfile, pid)) return 1;
        rescanhits(target, hit, memfile, hit_count, TARGET_COMPARE);
        if (keeptype(hit, &hit_count, 2) == -1) break;
      }
    }

    freeall(map, hit, initialhit_count, region_count);

    if (!hit_count)
      restart = restartfromzero(NO_HITS);
    else
      restart = restartfromzero(DEFAULT);

    if (restart) continue;

    break;
  }

  fclose(mapfile);
  fclose(memfile);

  return 0;
}

void initialcleanup(struct HitInfo* hit[], struct MapInfo* map[], int hitcount,
                    int regioncount) {
  for (int i = 0; hitcount + i < MAX_HIT_COUNT; ++i) free(hit[hitcount + i]);

  for (int i = 0; regioncount + i < MAX_MAP_REGION; ++i)
    free(map[regioncount + i]);
}

void freeall(struct MapInfo* map[], struct HitInfo* hit[], int inithitcount,
             int regioncount) {
  for (int i = 0; i < inithitcount; ++i) free(hit[i]);

  for (int i = 0; i < regioncount; ++i) free(map[i]);
}

void check_linux() {
#ifndef __linux__
    printf("This program is linux-only\n");
    exit(1);
#endif
}