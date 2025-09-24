/*
Memory Scanner (C / Linux)
Copyright (c) 2025 FiniteClimb
License: Research and Learning Only – see LICENSE file for full details
*/

#ifndef MEM_H
#define MEM_H

#include <stdio.h>

#include "globaltypes.h"

int openmemfile(FILE** memfile, int pid);

int mallocreadmem(struct MapInfo* map[], char* readmem[], int region_count);

int scanmemfile(struct MapInfo* map[], char* readmem[], FILE* memfile);

void freereadmem(char* readmem[], int region_count);

#endif