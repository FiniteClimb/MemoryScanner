#include "Userio.hpp"
#include "types.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

using std::cin;
using std::cout;
using std::getline;
using std::string;

int Userio::checkArgCount(const int argc, char **argv) {
  if (argc != 2) {
    printf("Correct usage: ./%s [pid]", argv[0]);
    return 1;
  }
  return 0;
}

void Userio::getTargetType() {

  cout << "Give target type (int, long, float, double, str):";
  string inputbuf;

  while (true) {
    getline(cin, inputbuf);
    if (inputbuf == "int" || inputbuf == "long" || inputbuf == "float" ||
        inputbuf == "double" || inputbuf == "str") {
      targetType = inputbuf;
      break;
    } else
      cout << "Invalid, try again:";
  }
}

void Userio::getTargetValue() {
  cout << "Give target value:";

  if (targetType == "int")
    readValue<int>(targetValue);
  else if (targetType == "long")
    readValue<long>(targetValue);
  else if (targetType == "float")
    readValue<float>(targetValue);
  else if (targetType == "double")
    readValue<double>(targetValue);
  else if (targetType == "str") {
    string temps;
    getline(cin, temps);
    targetValue.assign(temps.begin(), temps.end());
  } else
    throw std::runtime_error("Invalid type taken from getTargetType.");
}

void Userio::printMaps(const vector<MapRegion> &MapRegions) {
  for (unsigned long i = 0; i < MapRegions.size(); ++i) {
    cout << MapRegions[i];
  }
  cout << "Map regions count:" << MapRegions.size() << "\n";
}

void Userio::printRawBytes(const vector<unsigned char> &bytebuf) {
  for (unsigned long i = 0; i < bytebuf.size(); ++i) {
    printf("%02x ", bytebuf[i]);
  }
  cout << "bytebuf size:" << bytebuf.size() << "\n";
}

void Userio::printHits(const vector<HitInfo> &Hits) {
  if (Hits.size() == 0) {
    cout << "No hits.\n";
  }
  for (unsigned long i = 0; i < Hits.size(); ++i) {
    cout << Hits[i];
  }
  cout << "Hit count:" << std::dec << Hits.size() << "\n";
}

string Userio::getifRescan() {
  cout << "Would you like to rescan or move to edit? (rescan OR edit):";
  string strbuffer;
  while (true) {
    getline(cin, strbuffer);
    if (strbuffer == "rescan" || strbuffer == "edit")
      return strbuffer;
    cout << "Invalid. Try again (rescan OR edit):";
  }
}

string Userio::getRescanType() {
  cout << "Give rescan type (lower, same, higher, changed, all, specific)";
  string strbuffer;
  while (true) {
    getline(cin, strbuffer);
    if (strbuffer == "lower" || strbuffer == "same" || strbuffer == "higher" ||
        strbuffer == "changed" || strbuffer == "all" || strbuffer == "specific")
      return strbuffer;

    cout << "Invalid. Please just write it correctly:";
  }
}