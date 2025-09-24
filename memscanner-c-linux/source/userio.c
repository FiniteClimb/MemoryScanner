/*
Memory Scanner (C / Linux)
Copyright (c) 2025 FiniteClimb
License: Research and Learning Only – see LICENSE file for full details
*/

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globaltypes.h"

void introduction() {
  printf(
      "Hello, welcome to memory scanner in c for linux. Hopefully it works.\n");
}

void gettargettype(struct TargetInfo* target) {
  printf(
      "What type of value would you like to search for? ([s]tring, [i]nteger, "
      "[l]ong, [f]loat, [d]ouble):");
  char targettype[10];
  int again = 1;
  while (again) {
    again = 0;

    fgets(targettype, sizeof(targettype), stdin);

    if (*targettype == 's') target->type = TYPE_STRING;
    if (*targettype == 'd') target->type = TYPE_DOUBLE;
    if (*targettype == 'i') target->type = TYPE_INT;
    if (*targettype == 'f') target->type = TYPE_FLOAT;
    if (*targettype == 'l') target->type = TYPE_LONG;

    if (target->type == 0) {
      printf("Try again:");
      again = 1;
    }
  }
}

void getvalue(struct TargetInfo* target, char* destination) {
  char unparsedtarget[MAX_TARGET_SIZE];

  struct TargetInfo temp;

  printf("Give desired value:");

  char* endpoint;

  while (TRUE) {
    errno = 0;
    memset(unparsedtarget, '\0', sizeof(unparsedtarget));
    fgets(unparsedtarget, MAX_TARGET_SIZE - 1, stdin);
    if (!strchr(unparsedtarget, '\n')) {
      continue;
    }
    printf("why\n");

    memset(strchr(unparsedtarget, '\n'), '\0', 1);
    switch (target->type) {
      case TYPE_LONG:
        temp.data.l = strtol(unparsedtarget, &endpoint, 10);
        target->size = 8;
        break;

      case TYPE_INT:
        temp.data.l = strtol(unparsedtarget, &endpoint, 10);
        target->size = 4;
        if (!(INT_MIN <= temp.data.l && temp.data.l <= INT_MAX)) {
          printf("Outside of target scope. Try again too big/small: ");
          continue;
        }
        break;

      case TYPE_FLOAT:
        temp.data.f = strtof(unparsedtarget, &endpoint);
        target->size = 4;
        break;

      case TYPE_DOUBLE:
        temp.data.d = strtod(unparsedtarget, &endpoint);
        target->size = 8;

        break;

      case TYPE_STRING:
        memcpy(temp.data.s, unparsedtarget, MAX_TARGET_SIZE);
        target->size = strlen(temp.data.s);
    }

    if (errno == ERANGE) {
      printf("Outside of target scope. Try again:");
      continue;
    }
    if (endpoint == unparsedtarget) {
      printf("Write the value properly:");
      continue;
    }

    memcpy(destination, &temp.data.s, target->size);
    break;
  }
}

int rescan() {
  while (TRUE) {
    printf("Would you like to rescan?(1/y, 0/n(just move to editing)):");
    char rescanbuf[5];
    int rescanintbuf = 0;
    fgets(rescanbuf, sizeof(rescanbuf), stdin);
    sscanf(rescanbuf, "%i", &rescanintbuf);

    if (*rescanbuf == 'y' || rescanintbuf == 1) return 1;
    if (*rescanbuf == 'n' || rescanintbuf == 0) return 0;

    printf("Invalid input for rescan. Try again\n");

    continue;
  }
}

char typeofrescan() {
  printf("What type of rescan will you do?\n");
  printf("d for tagging hits based on same or different\n");
  printf("OR v for rescanning previous hits for a specific value:");

  char typebuf[3];
  char typeofscan[1];

  while (TRUE) {
    fgets(typebuf, sizeof(typebuf), stdin);
    sscanf(typebuf, "%c", typeofscan);
    if (*typeofscan == 'd') return 'd';
    if (*typeofscan == 'v') return 'v';

    printf("Invalid type of rescan. Try again.\n");
    continue;
  }
}

int editindex(int hitcount) {
  int edit_index = 0;
  char indexbuf[8];

  printf("Which index will you edit?:");

  while (TRUE) {
    fgets(indexbuf, sizeof(indexbuf), stdin);
    sscanf(indexbuf, "%i", &edit_index);
    if (!(edit_index < 0 || edit_index >= hitcount)) return edit_index;

    printf("Invalid index. Try again:");
    continue;
  }
}

void printhits(struct HitInfo* hit[], int hit_count) {
  for (int i = 0; i < hit_count; ++i) {
    if (hit[i]->changed != 0) printf("\033[31m");
    if (hit[i]->changed == 1) printf("*HIGHER* ");
    if (hit[i]->changed == -1) printf("*LOWER* ");

    printf("%i || at %lx in region %s\n", i, hit[i]->location, hit[i]->region);
    for (int k = 0; k < sizeof(hit[0]->bytes); ++k) {
      if (k == 22) printf("*");  // Assumes we are not near border of a region.
      printf("%0x ", hit[i]->bytes[k]);
    }
    printf("\033[0m\n\n");
  }
}

void printmaps(struct MapInfo* map[], int region_count) {
  for (int i = 0; i < region_count; ++i) {
    printf("%i || %lx to %lx at %s\n", i, map[i]->start, map[i]->end,
           map[i]->name);
  }
}

int keepwhich() {
  printf(
      "Which values would you like to keep? (3=All 2=Unchanged 0=Changed "
      "-1=Lower 1=Higher):");

  int keep = 0;
  char buf[5];

  while (TRUE) {
    fgets(buf, sizeof(buf), stdin);
    sscanf(buf, "%i", &keep);

    if (keep >= -1 && keep <= 3) return keep;
    printf("try again:");

    continue;
  }
}

int nohitsrestart() {
  char inputbuffer[5];
  int getvalueagain = 0;
  printf("No hits. Search for another value? 1=yes 0=no");

  while (TRUE) {
    fgets(inputbuffer, sizeof(inputbuffer), stdin);
    sscanf(inputbuffer, "%i", &getvalueagain);

    if (getvalueagain == 0 || getvalueagain == 1) return getvalueagain;

    printf("Invalid answer. Try again:");
    continue;
  }
}

void getnewvalue(struct TargetInfo* target) {
  printf("Will now go into edit mode.\n");
  getvalue(target, target->data.s);
}

int restartfromzero(int REASON) {
  if (REASON == NO_HITS) printf("No more hits.\n");

  int restart = 0;
  char inputbuffer[5];

  printf(
      "Would you like to start from the very beginning? (1 for yes, 0 for "
      "no):");

  while (1) {
    fgets(inputbuffer, sizeof(inputbuffer), stdin);
    sscanf(inputbuffer, "%i", &restart);

    if (restart == 1 || restart == 0) return restart;

    printf("Just please write 0 or 1:");
  }
  return 0;
}

int returntolasthits() {
  printf("Would you like to return to the list of hits? 1=Yes 0=No");
  int returnto = 0;
  char inputbuf[4];

  while (1) {
    fgets(inputbuf, sizeof(inputbuf), stdin);
    sscanf(inputbuf, "%i", &returnto);

    if (returnto == 1) return 1;
    if (returnto == 0) return 0;

    printf("1 or 0:");
    continue;
  }
}