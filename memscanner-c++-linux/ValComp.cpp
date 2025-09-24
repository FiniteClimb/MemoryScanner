#include "ValComp.hpp"
#include "types.hpp"
#include <algorithm>
#include <cstring>

using std::remove_if;

void ValComp::initTargetScan(vector<MapRegion> &Maps,
                             const vector<unsigned char> &targetValue) {

  int cmpresult;
  for (unsigned long i = 0; i < Maps.size(); ++i) {
    for (unsigned long k = 0; k < Maps[i].readbuff.size();
         k += targetValue.size()) {
      cmpresult =
          memcmp(targetValue.data(), &Maps[i].readbuff[k], targetValue.size());

      if (cmpresult == 0)
        Hits.push_back(MakeHit(Maps[i], k));
    }
  }
}

HitInfo ValComp::MakeHit(MapRegion &hit_Region, unsigned long REL_LOC) {
  HitInfo TempHit;

  TempHit.abs_location = hit_Region.start + REL_LOC;
  TempHit.rel_location = REL_LOC;
  TempHit.hit_region = &hit_Region;
  TempHit.bytes_around.resize(49);

  std::fill_n(TempHit.bytes_around.data(), TempHit.bytes_around.size(), 0xfd);
  refreshBytes(hit_Region, TempHit);
  return TempHit;
}

void ValComp::rescanHits(vector<MapRegion> &Maps,
                         vector<unsigned char> &targetValue) {
  int cmpresult;

  for (unsigned long i = 0; i < Hits.size(); ++i) {
    cmpresult = memcmp(&Hits[i].bytes_around[24],
                       &Hits[i].hit_region->readbuff[Hits[i].rel_location],
                       targetValue.size());
    if (cmpresult == 0)
      Hits[i].relative_to_prev_value = 0;
    if (cmpresult < 0)
      Hits[i].relative_to_prev_value = 1;
    if (cmpresult > 0)
      Hits[i].relative_to_prev_value = -1;

    refreshBytes(*Hits[i].hit_region, Hits[i]);
  }
}

void ValComp::refreshBytes(MapRegion &Map, HitInfo &Hit) {

  unsigned long start_bytes =
      Hit.rel_location - 24 < 0 ? 0 : Hit.rel_location - 24;
  unsigned long end_bytes = Hit.rel_location + 24 > Map.readbuff.size()
                                ? Map.readbuff.size()
                                : Hit.rel_location + 24;
  memcpy(Hit.bytes_around.data(), &Map.readbuff[start_bytes],
         end_bytes - start_bytes + 1);
}

void ValComp::EvaluateHits(string KEEP_TYPE,
                           vector<unsigned char> &targetValue) {

  if (KEEP_TYPE == "higher")
    Hits.erase(remove_if(Hits.begin(), Hits.end(),
                         [](const HitInfo &Hit) {
                           return Hit.relative_to_prev_value <= 0;
                         }),
               Hits.end());

  if (KEEP_TYPE == "lower")

    Hits.erase(remove_if(Hits.begin(), Hits.end(),
                         [](const HitInfo &Hit) {
                           return Hit.relative_to_prev_value >= 0;
                         }),
               Hits.end());

  if (KEEP_TYPE == "unchanged")
    Hits.erase(remove_if(Hits.begin(), Hits.end(),
                         [](const HitInfo &Hit) {
                           return Hit.relative_to_prev_value != 0;
                         }),
               Hits.end());

  if (KEEP_TYPE == "specific")
    Hits.erase(remove_if(Hits.begin(), Hits.end(),
                         [targetValue](const HitInfo &Hit) {
                           return memcmp(
                               &Hit.bytes_around[24],
                               &Hit.hit_region->readbuff[Hit.rel_location],
                               targetValue.size());
                           ;
                         }),
               Hits.end());

  if (KEEP_TYPE == "changed")
    Hits.erase(remove_if(Hits.begin(), Hits.end(),
                         [](const HitInfo &Hit) {
                           return Hit.relative_to_prev_value == 0;
                         }),
               Hits.end());
  if (KEEP_TYPE == "all")
    for (int i = 0; i < Hits.size(); ++i)
      Hits[i].relative_to_prev_value = 0;
  return;
}