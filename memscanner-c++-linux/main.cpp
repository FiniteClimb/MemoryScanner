#include "Maps.hpp"
#include "Mem.hpp"
#include "Userio.hpp"
#include "ValComp.hpp"
#include "types.hpp"

int main(int argc, char **argv) {
  int pid = atoi(argv[1]);

  while (true) {
    Userio UserioObj(argc, argv);
    Maps MapsObj(argv[1]);
    Mem MemObj(argv[1], MapsObj.mapRegions);
    ValComp ValCompObj(MapsObj.mapRegions, UserioObj.targetValue);

    UserioObj.printRawBytes(UserioObj.targetValue);
    UserioObj.printMaps(MapsObj.mapRegions);

    while (true) {

      UserioObj.printHits(ValCompObj.Hits);

      if (UserioObj.getifRescan() == "edit") {
        // Edit which index?
        // Freeze? Implement a bit later after everything is finished.
        // New value?
        // Written new value.
        // Go back to last list of hits or restart (keep frozen values)?
        // If go back:
        continue;
        break;
      }
      string rescan_type;
      rescan_type = UserioObj.getRescanType();

      MemObj.ReadAllMem(MapsObj.mapRegions);

      int COMPARE_TYPE;
      if (rescan_type == "specific") {
        COMPARE_TYPE = TARGET_COMPARE;
        UserioObj.getTargetValue();
        for (unsigned long i = 0; i < ValCompObj.Hits.size(); ++i)
          ValCompObj.refreshBytes(ValCompObj.Hits[i]);
      } else
        COMPARE_TYPE = RELATIVE_COMPARE;

      ValCompObj.rescanHits(UserioObj.targetValue, COMPARE_TYPE);
      ValCompObj.EvaluateHits(rescan_type, UserioObj.targetValue);

      for (unsigned long i = 0; i < ValCompObj.Hits.size(); ++i)
        ValCompObj.refreshBytes(ValCompObj.Hits[i]);

      continue;
    }
    continue;
  }
}