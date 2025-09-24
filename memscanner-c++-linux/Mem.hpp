#ifndef MEM_HPP
#define MEM_HPP

#include "types.hpp"
#include <string>
#include <unistd.h>
#include <vector>

using std::string;
using std::vector;

class Mem {

public:
  Mem(const string &pid, vector<MapRegion> &Maps) {
    int pidnum = std::stoi(pid);
    ReadAllMem(Maps, pidnum);
  }

  void ReadAllMem(vector<MapRegion> &Maps, const int pid);
  // template<typename T>vector<unsigned char>WriteMem(unsinged long TARGET_LOC,
  // vector<unsigned char>NEW_VALUE)
};

#endif