/*
Memory Scanner (C / Linux)
Copyright (c) 2025 FiniteClimb
License: Research and Learning Only – see LICENSE file for full details
*/

#ifndef GLOBALTYPES_H
#define GLOBALTYPES_H

enum DataType {
  TYPE_INT = 1,
  TYPE_STRING = 2,
  TYPE_LONG = 3,
  TYPE_FLOAT = 4,
  TYPE_DOUBLE = 5
};

struct TargetInfo {
  enum DataType type;
  union {
    int i;
    long l;
    float f;
    double d;
    char s[30];
  } data;
  int size;
};

#define MAX_TARGET_SIZE 30
#define MAX_MAP_REGION 1000
#define MAX_MAP_REGION_SIZE 200
#define MAX_HIT_COUNT 1000000

struct MapInfo {
  unsigned long start;
  unsigned long end;
  unsigned long size;
  char name[250];
};

struct HitInfo {
  unsigned long location;
  char* region;
  union {
    int i;
    float f;
    long l;
    char s[30];
    double d;
  } value;

  unsigned char bytes[45];
  unsigned long regstart;
  unsigned long regend;

  int changed;
};

#define RELATIVE_COMPARE 0
#define TARGET_COMPARE 1

#define TRUE 1
#define FALSE 0

#define NO_HITS 1
#define DEFAULT 0

#endif