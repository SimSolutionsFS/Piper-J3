#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
#include <XPLMMenus.h>
#include <acfutils/log.h>

#include "misc.h"

void miscStart() {
	logMsg("Adding items to aircraft menu...");
	XPLMMenuID aircraft_menu = XPLMFindAircraftMenu();
	if (aircraft_menu) {
		XPLMAppendMenuItemWithCommand(aircraft_menu, "Toggle Chocks/Tiedowns", nullptr);
		XPLMAppendMenuItemWithCommand(aircraft_menu, "Toggle Tablet", nullptr);
	}
}

void miscStop() {}