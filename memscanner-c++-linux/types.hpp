#ifndef TYPES_HPP
#define TYPES_HPP

#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::shared_ptr;
using std::string;
using std::vector;

const int INITIAL_SCAN = 0;
const int RE_SCAN = 1;

inline void my_ljust(std::stringstream &strstream, unsigned long width,
                     char fill = ' ') {
  if (!(strstream.str().size() >= width))
    strstream << string(width - strstream.str().size(), fill);
}

struct MapRegion {
  unsigned long start;
  unsigned long end;
  unsigned long size;
  vector<unsigned char> readbuff;
  string name;
};

inline std::ostream &operator<<(std::ostream &os, const MapRegion &MapRegion) {

  std::stringstream buffer;
  buffer << "0x" << std::hex << MapRegion.start << "-" << "0x" << MapRegion.end
         << "  Size = " << std::dec << MapRegion.size;

  my_ljust(buffer, 55);

  os << buffer.str() << MapRegion.name << "\n";
  return os;
}

struct HitInfo {
  unsigned long abs_location;
  unsigned long rel_location;
  vector<unsigned char> bytes_around;
  MapRegion *hit_region;
  int relative_to_prev_value = 0; // 0 if equal. -1 if smaller. 1 if bigger.
};

inline std::ostream &operator<<(std::ostream &os, const HitInfo &HitInfo) {

  if (HitInfo.relative_to_prev_value == 0)
    os << "SAME: ";
  if (HitInfo.relative_to_prev_value == 1)
    os << "BIGGER: ";
  if (HitInfo.relative_to_prev_value == -1)
    os << "SMALLER: ";

  os << "0x" << std::hex << HitInfo.abs_location << " at  "
     << HitInfo.hit_region->name << "\n";
  for (unsigned long i = 0; i < HitInfo.bytes_around.size(); ++i) {
    if (i == 24)
      cout << "*";
    printf("%02x ", HitInfo.bytes_around[i]);
  }
  printf("\n\n");
  return os;
}

#endif