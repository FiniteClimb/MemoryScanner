/*
Memory Scanner (C / Linux)
Copyright (c) 2025 FiniteClimb
License: Research and Learning Only – see LICENSE file for full details
*/

#include "targetcomp.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globaltypes.h"

int searchtarget(struct TargetInfo target, struct MapInfo *map[],
                 struct HitInfo *hit[], char *readmem[], int region_count,
                 int *hit_count) {
  int h = 0;

  int startbytes = 0;
  int endbytes = 0;

  *hit_count = 0;

  for (int i = 0; i < region_count; ++i) {
    if (readmem[i] == NULL) continue;

    for (long k = 0; k < map[i]->size - target.size; ++k) {
      if (memcmp(target.data.s, readmem[i] + k, target.size)) continue;

      startbytes = k < 22 ? 0 : k - 22;
      endbytes = k + 22 > map[i]->size ? map[i]->size : k + 22;

      memcpy(hit[h]->value.s, target.data.s, target.size);

      registerhit(hit, map, i, k, h);

      memcpy(hit[h]->bytes, readmem[i] + startbytes, endbytes - startbytes + 1);
      ++h;
      if (h == MAX_HIT_COUNT) break;
    }
    if (h == MAX_HIT_COUNT) break;
  }
  *hit_count = h;

  if (!h) {
    printf("No hits found.");
    return 1;
  }
  printf("\n");
  return 0;
}

void registerhit(struct HitInfo *hit[], struct MapInfo *map[], int mapregion,
                 int offset, int hitcount) {
  hit[hitcount]->location = map[mapregion]->start + offset;

  hit[hitcount]->region = &map[mapregion]->name[0];

  hit[hitcount]->regstart = map[mapregion]->start;
  hit[hitcount]->regend = map[mapregion]->end;
}

int mallochits(struct HitInfo *hit[]) {
  for (int i = 0; i < MAX_HIT_COUNT; ++i) {
    hit[i] = malloc(sizeof(struct HitInfo));
    if (!hit[i]) {
      perror("Failed to malloc hit space");
      return -1;
    }
  }
  return 0;
}

void freehits(struct HitInfo *hit[], int hitcount) {
  for (int i = 0; i < hitcount; ++i) {
    free(hit[i]);
  }
}

// RESCAN_TYPE==0 compare relatively, RESCAN_TYPE==1 compare to target.
int rescanhits(struct TargetInfo target, struct HitInfo *hit[], FILE *memfile,
               int hitcount, int RESCAN_TYPE) {
  int change = 0;

  int bytestart = 0;

  unsigned char tempbytes[target.size];

  recheckbytes(hit, memfile, 0, hitcount);

  printf("hitcount:%i\n", hitcount);

  for (int i = 0; i < hitcount; ++i) {
    bytestart = hit[i]->location - hit[i]->regstart;

    bytestart = bytestart < 22 ? bytestart : 22;

    if (RESCAN_TYPE == 0) {
      memcpy(tempbytes, &hit[i]->bytes[bytestart], target.size);

      change = memcmp(tempbytes, &hit[i]->bytes[bytestart], target.size);
    } else
      change = memcmp(&target.data.s, &hit[i]->bytes[bytestart], target.size);

    if (change > 0) {
      printf("New value is lower RESCAN_TYPE=%i\n", RESCAN_TYPE);
      hit[i]->changed = -1;
    }
    if (change < 0) {
      printf("New value is higher RESCAN_TYPE=%i\n", RESCAN_TYPE);
      hit[i]->changed = 1;
    }
    if (change == 0) {
      printf("New value is the same RESCAN_TYPE=%i\n", RESCAN_TYPE);
      hit[i]->changed = 0;
    }
  }

  return 0;
}

// 3=All 2=Unchanged 0=Changed  -1=Lower 1=Higher
int keeptype(struct HitInfo *hit[], int *hitcount, int type) {
  if (type == 3) return 0;

  int desiredhit[*hitcount];

  memset(desiredhit, '\0', sizeof(desiredhit));

  int k = 0;

  printf("Trying to fix keeptype\n");
  printf("Current hit count:%i\n", *hitcount);

  for (int i = 0; i < *hitcount; ++i) {
    printf("Changed value of hit %i=%i\n", i, hit[i]->changed);
  }

  for (int i = 0; i < *hitcount; ++i) {
    switch (type) {
      case 0:
        if (hit[i]->changed == 1 || hit[i]->changed == -1) {
          desiredhit[k] = i;
          ++k;
        }
        break;

      case -1:
        if (hit[i]->changed == -1) {
          desiredhit[k] = i;
          ++k;
        }
        break;

      case 1:
        if (hit[i]->changed == 1) {
          desiredhit[k] = i;
          ++k;
        }
        break;

      case 2:
        if (hit[i]->changed == 0) {
          desiredhit[k] = i;
          ++k;
        }
        break;
    }
  }
  printf("Hit count of interest=%i\n", k);

  for (int i = 0; i < *hitcount; ++i) {
    printf("desiredhit[%i]=%i\n", i, desiredhit[i]);
  }

  if (!k) {
    (printf("No more hits left\n"));
    return -1;
  }

  for (int i = 0; i < k; ++i) {
    memcpy(hit[i], hit[desiredhit[i]], sizeof(struct HitInfo));
  }
  *hitcount = k;

  return 0;
}

void writenewvalue(struct TargetInfo target, struct HitInfo *hit[],
                   FILE *memfile, int edit_index) {
  fseek(memfile, hit[edit_index]->location, SEEK_SET);
  fwrite(target.data.s, 1, target.size, memfile);

  recheckbytes(hit, memfile, edit_index, 1);

  hit[edit_index]->changed = 1;
}

void recheckbytes(struct HitInfo *hit[], FILE *memfile, int START_FROM,
                  int LOOP_TIMES) {
  LOOP_TIMES += START_FROM;

  for (int i = 0; i + START_FROM < LOOP_TIMES; ++i) {
    long readamount = 0;
    long bytestart = 0;
    long byteend = 0;

    bytestart = hit[START_FROM + i]->location - hit[START_FROM + i]->regstart;
    byteend = hit[START_FROM + i]->regend - hit[START_FROM + i]->location;

    bytestart = bytestart < 22 ? bytestart : 22;
    byteend = byteend < 22 ? byteend : 22;

    errno = 0;

    if (fseek(memfile, hit[START_FROM + i]->location - bytestart, SEEK_SET))
      printf("failed fseek on rescan\n\n");
    readamount =
        fread(hit[START_FROM + i]->bytes, 1, byteend + bytestart + 1, memfile);

    if (readamount != byteend + bytestart + 1)
      printf("Failed to rescan hit %i at %s\n", START_FROM,
             hit[START_FROM + i]->region);
  }
}

void resetchangedvalue(struct HitInfo *hit[], int hitcount) {
  for (int i = 0; i < hitcount; ++i) {
    hit[i]->changed = 0;
  }
}