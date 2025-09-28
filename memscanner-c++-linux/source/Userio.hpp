#include "types.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::vector;

class Userio {
  template <typename T> void readValue(vector<unsigned char> &target) {
    T value;
    string linebuf;
    std::getline(cin, linebuf);
    value = std::stoi(linebuf);
    target.resize(sizeof(T));
    memcpy(target.data(), &value, sizeof(value));
  }

public:
  string targetType;
  vector<unsigned char> targetValue;

  Userio(const int argc, char **argv) {
    if (checkArgCount(argc, argv) == -1)
      exit(-1);
    printf("hello. This is memscanner++. You can look for certain values in "
           "memory and edit them.");
    getTargetType();
    getTargetValue();
  };

  int checkArgCount(const int argc, char **argv);

  void printHits(vector<HitInfo> &Hits);
  void printMaps(const vector<MapRegion> &MapRegions);
  void printRawBytes(const vector<unsigned char> &bytebuf);

  void getTargetType();
  void getTargetValue();
  vector<unsigned char> getValue();

  string getifRescan();

  string getRescanType();

  int getEditIndex(vector<HitInfo> &Hits);
  vector<unsigned char> getNewValue();

  string getIfContinue();
};