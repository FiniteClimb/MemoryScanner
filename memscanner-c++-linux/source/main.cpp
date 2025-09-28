#include "Maps.hpp"
#include "Mem.hpp"
#include "Userio.hpp"
#include "ValComp.hpp"
#include "types.hpp"

int main(int argc, char **argv) {
  while (true) {
    Userio UserioObj(argc, argv);
    Maps MapsObj(argv[1]);
    Mem MemObj(argv[1], MapsObj.mapRegions);
    ValComp ValCompObj(MapsObj.mapRegions, UserioObj.targetValue);

    UserioObj.printMaps(MapsObj.mapRegions);

    while (true) {

      UserioObj.printHits(ValCompObj.Hits);

      if (UserioObj.getifRescan() == "edit") {
        int edit_index = UserioObj.getEditIndex(ValCompObj.Hits);
        MemObj.WriteValue(UserioObj.getNewValue(),
                          ValCompObj.Hits[edit_index].abs_location);

        MemObj.ReadAllMem(MapsObj.mapRegions);
        ValCompObj.refreshBytes();

        UserioObj.printHits(ValCompObj.Hits);
        if (UserioObj.getIfContinue() == "end")
          return 0;
        continue;
      }
      string rescan_type = UserioObj.getRescanType();

      MemObj.ReadAllMem(MapsObj.mapRegions);

      int COMPARE_TYPE;
      if (rescan_type == "specific") {
        COMPARE_TYPE = TARGET_COMPARE;
        UserioObj.getTargetValue();
        ValCompObj.refreshBytes();
      } else
        COMPARE_TYPE = RELATIVE_COMPARE;

      ValCompObj.rescanHits(UserioObj.targetValue, COMPARE_TYPE);
      ValCompObj.EvaluateHits(rescan_type, UserioObj.targetValue);

      ValCompObj.refreshBytes();
      continue;
    }
  }
}