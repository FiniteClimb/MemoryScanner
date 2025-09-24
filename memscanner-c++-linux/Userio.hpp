#include "types.hpp"

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
    checkArgCount(argc, argv);
    printf("hello. This is memscanner++ version 0.0.1-dev. It probably doesn't "
           "work lol. Good luck.\n");
    getTargetType();
    getTargetValue();
  };

  int checkArgCount(const int argc, char **argv);

  void printHits(const vector<HitInfo> &Hits);
  void printMaps(const vector<MapRegion> &MapRegions);
  void printRawBytes(const vector<unsigned char> &bytebuf);

  void getTargetType();
  void getTargetValue();

  string getifRescan();

  string getRescanType();
  // string getNewTarget();

  // string getifFreeze();
  // string getNewValue();
};