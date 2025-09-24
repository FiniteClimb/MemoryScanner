/*
Memory Scanner (C / Linux)
Copyright (c) 2025 FiniteClimb
License: Research and Learning Only – see LICENSE file for full details
*/

#ifndef USERIO_H
#define USERIO_H

void introduction();

void gettargettype(struct TargetInfo *targetinfo);

void getvalue(struct TargetInfo *target, char *destination);

void printhits(struct HitInfo *hit[], int hit_count);

int rescan();

char typeofrescan();

void printmaps(struct MapInfo *map[], int region_count);

int keepwhich();

int nohitsrestart();

int editindex(int hitcount);

void getnewvalue(struct TargetInfo *target);

int restartfromzero(int REASON);

int returntolasthits();

#endif