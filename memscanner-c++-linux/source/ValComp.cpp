#include "ValComp.hpp"
#include "types.hpp"
#include <algorithm>
#include <cstring>

using std::remove_if;

void ValComp::initTargetScan(vector<MapRegion> &Maps,
                             const vector<unsigned char> &targetValue) {

  int cmpresult;
  for (unsigned long i = 0; i < Maps.size(); ++i) {
    for (unsigned long k = 0; k < Maps[i].readbuff.size(); ++k) {
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
  refreshByte(TempHit);
  return TempHit;
}

void ValComp::rescanHits(vector<unsigned char> &targetValue, int COMPARE_TYPE) {

  int cmpresult = 0;
  unsigned char *cmpwith = targetValue.data();

  for (unsigned long i = 0; i < Hits.size(); ++i) {
    if (COMPARE_TYPE == RELATIVE_COMPARE)
      cmpwith = &Hits[i].hit_region->readbuff[Hits[i].rel_location];

    for (unsigned long j = 0; j < targetValue.size(); ++j) {
      auto s1 = Hits[i].bytes_around[24 + targetValue.size() - 1 - j];
      auto s2 = cmpwith[targetValue.size() - 1 - j];

      if (s1 > s2) {
        cmpresult = 1;
        break;
      } else if (s1 < s2) {
        cmpresult = -1;
        break;
      } else
        cmpresult = 0;
    }

    if (cmpresult == 0)
      Hits[i].relative_to_prev_value = 0;
    else if (cmpresult < 0)
      Hits[i].relative_to_prev_value = 1;
    else if (cmpresult > 0)
      Hits[i].relative_to_prev_value = -1;
  }
}
void ValComp::refreshByte(HitInfo &Hit) {

  unsigned long start_bytes =
      Hit.rel_location - 24 < 0 ? 0 : Hit.rel_location - 24;
  unsigned long end_bytes =
      Hit.rel_location + 24 > Hit.hit_region->readbuff.size()
          ? Hit.hit_region->readbuff.size()
          : Hit.rel_location + 24;
  memcpy(Hit.bytes_around.data(), &Hit.hit_region->readbuff[start_bytes],
         end_bytes - start_bytes + 1);
}

void ValComp::refreshBytes() {
  for (unsigned long i = 0; i < Hits.size(); ++i) {
    unsigned long start_bytes =
        Hits[i].rel_location - 24 < 0 ? 0 : Hits[i].rel_location - 24;
    unsigned long end_bytes =
        Hits[i].rel_location + 24 > Hits[i].hit_region->readbuff.size()
            ? Hits[i].hit_region->readbuff.size()
            : Hits[i].rel_location + 24;
    memcpy(Hits[i].bytes_around.data(),
           &Hits[i].hit_region->readbuff[start_bytes],
           end_bytes - start_bytes + 1);
  }
}

void ValComp::EvaluateHits(string KEEP_TYPE,
                           vector<unsigned char> &targetValue) {

  if (KEEP_TYPE == "higher")
    Hits.erase(remove_if(Hits.begin(), Hits.end(),
                         [](const HitInfo &Hit) {
                           return Hit.relative_to_prev_value <= 0;
                         }),
               Hits.end());

  else if (KEEP_TYPE == "lower")

    Hits.erase(remove_if(Hits.begin(), Hits.end(),
                         [](const HitInfo &Hit) {
                           return Hit.relative_to_prev_value >= 0;
                         }),
               Hits.end());

  else if (KEEP_TYPE == "unchanged" || KEEP_TYPE == "specific")
    Hits.erase(remove_if(Hits.begin(), Hits.end(),
                         [](const HitInfo &Hit) {
                           return Hit.relative_to_prev_value != 0;
                         }),
               Hits.end());

  else if (KEEP_TYPE == "changed")
    Hits.erase(remove_if(Hits.begin(), Hits.end(),
                         [](const HitInfo &Hit) {
                           return Hit.relative_to_prev_value == 0;
                         }),
               Hits.end());

  else if (KEEP_TYPE == "all")
    return;
}