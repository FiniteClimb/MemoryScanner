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
      if (rescan_type == "all")
        continue;
      if (rescan_type == "specific")
        UserioObj.getTargetValue();

      MemObj.ReadAllMem(MapsObj.mapRegions, pid);

      cout << "testt\n";
      ValCompObj.rescanHits(MapsObj.mapRegions, UserioObj.targetValue);
      cout << "anothertest\n";
      ValCompObj.EvaluateHits(rescan_type, UserioObj.targetValue);
      continue;
    }
    continue;
  }
}