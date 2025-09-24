#ifndef VALCOMP_HPP
#define VALCOMP_HPP

#include "types.hpp"

struct ValComp {
  void initTargetScan(vector<MapRegion> &Maps,
                      const vector<unsigned char> &targetValue);
  void refreshBytes(HitInfo &Hit);
  HitInfo MakeHit(MapRegion &hit_Region, unsigned long RELATIVE_lOCATION);

public:
  vector<HitInfo> Hits;

  ValComp(vector<MapRegion> &Maps, vector<unsigned char> &targetValue) {
    initTargetScan(Maps, targetValue);
  }
  void EvaluateHits(string KEEP_TYPE, vector<unsigned char> &targetValue);
  void rescanHits(vector<unsigned char> &targetValue, int COMPARE_TYPE);
};

#endif