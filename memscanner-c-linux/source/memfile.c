/*
Memory Scanner (C / Linux)
Copyright (c) 2025 FiniteClimb
License: Research and Learning Only – see LICENSE file for full details
*/

#include "memfile.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globaltypes.h"

int openmemfile(FILE** memfile, int pid) {
  if (*memfile) fclose(*memfile);
  char memfileloc[30];

  snprintf(memfileloc, sizeof(memfileloc), "/proc/%i/mem", pid);

  *memfile = fopen(memfileloc, "r+");

  if (!*memfile) {
    perror("Failed to open mem:");
    return -1;
  }
  return 0;
}

int scanmemfile(struct MapInfo* map[], char* readmem[], FILE* memfile) {
  int n = 0;
  unsigned long readamount = 0;

  while (!fseek(memfile, map[n]->start, SEEK_SET)) {
    readamount = fread(readmem[n], 1, map[n]->size, memfile);

    if (readamount != map[n]->end - map[n]->start) {
      fprintf(stderr, "Failed to read %s:%s\n\n", map[n]->name,
              strerror(errno));
      readmem[n] = NULL;
      ++n;
      continue;
    }
    printf("Read amount from region %i: %lu\n", n, readamount);
    ++n;
  }
  return 0;
}

int mallocreadmem(struct MapInfo* map[], char* readmem[], int region_count) {
  for (int i = 0; i < region_count; ++i) {
    readmem[i] = malloc(map[i]->end - map[i]->start + 1);
    if (!readmem[i]) {
      perror("Failed to malloc readmem:");
      return -1;
    }
  }
  return 0;
}

void freereadmem(char* readmem[], int region_count) {
  for (int i = 0; i < region_count; ++i)
    if (readmem[i]) free(readmem[i]);
}
