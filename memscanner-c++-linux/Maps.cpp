#include "Maps.hpp"
#include "types.hpp"
#include <sstream>
#include <stdexcept>
#include <string>

using std::getline;
using std::runtime_error;
using std::string;

void Maps::openMaps(const string pid) {
  string path = ("/proc/" + pid + "/maps");
  MapFile.open(path);
  if (!MapFile.is_open())
    throw runtime_error("Failed to open maps file.");
}

void Maps::readMaps() {
  string line;

  while (getline(MapFile, line)) {
    std::istringstream splitline(line);

    string MemoryAddresses, perms, name, trash;

    splitline >> MemoryAddresses >> perms >> trash >> trash >> trash >> name;

    if (perms[0] == '-' || perms[1] == '-' || perms[2] == 'x' ||
        perms[3] == 's')
      continue;
    if (name.find("/lib/") != std::string::npos)
      continue;

    if (name.empty())
      name = "UNNAMED_REGION";

    string startStr = MemoryAddresses.substr(0, MemoryAddresses.find('-'));
    string endStr = MemoryAddresses.substr(MemoryAddresses.find('-') + 1);

    unsigned long start = stoul(startStr, nullptr, 16);
    unsigned long end = stoul(endStr, nullptr, 16);

    MapRegion TempMap = {start, end, end - start, {}, name};
    TempMap.readbuff.resize(TempMap.size);
    mapRegions.push_back(TempMap);
  }
}