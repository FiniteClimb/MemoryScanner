/*
Memory Scanner (C / Linux)
Copyright (c) 2025 FiniteClimb
License: Research and Learning Only – see LICENSE file for full details
*/

#ifndef TARGETCOMP_H
#define TARGETCOMP_H

#include <stdio.h>

#include "globaltypes.h"

int mallochits(struct HitInfo *hit[]);

int searchtarget(struct TargetInfo target, struct MapInfo *map[],
                 struct HitInfo *hit[], char *readmem[], int region_count,
                 int *hit_count);

void freehits(struct HitInfo *hit[], int hitcount);

int rescanhits(struct TargetInfo target, struct HitInfo *hit[], FILE *memfile,
               int hitcount, int RESCAN_TYPE);

void registerhit(struct HitInfo *hit[], struct MapInfo *map[], int mapregion,
                 int offset, int count);

int keeptype(struct HitInfo *hit[], int *hitcount, int type);

void writenewvalue(struct TargetInfo target, struct HitInfo *hit[],
                   FILE *filemem, int edit_index);

void recheckbytes(struct HitInfo *hit[], FILE *memfile, int START_FROM,
                  int LOOP_TIMES);

void resetchangedvalue(struct HitInfo *hit[], int hitcount);

#endif