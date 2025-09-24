#include "types.hpp"
#include <fstream>
#include <vector>

using std::ifstream;
using std::vector;

class Maps {

  ifstream MapFile;
  void openMaps(const string pid);
  void readMaps();

public:
  vector<MapRegion> mapRegions;
  Maps(const string pid) {
    openMaps(pid);
    readMaps();
  }
  const vector<MapRegion> &GetMaps() const { return mapRegions; }
};