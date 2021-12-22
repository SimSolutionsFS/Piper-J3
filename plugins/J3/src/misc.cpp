#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
#include <XPLMMenus.h>
#include <acfutils/log.h>

#include "misc.h"

int showCovers;

int getCovers(void* inRefcon) { 
	return showCovers; 
}
void setCovers(void* inRefcon, int inValue) { 
	showCovers = inValue; 
}

XPLMDataRef drefShowTiedowns = XPLMRegisterDataAccessor("J3/Ground/ShowCovers", xplmType_Int, true, getCovers, setCovers, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

int toggleCovers(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon) {
	if (inPhase == 0) {
		if (showCovers == 1) { 
			showCovers = 0; 
		}
		else { 
			showCovers = 1; 
		}
	}

	return 1;
}

XPLMCommandRef commandCovers = XPLMCreateCommand("J3/Ground/ToggleCovers", "Toggle Covers");

void miscStart() {
	logMsg("Registering commands...");
	XPLMRegisterCommandHandler(commandCovers, toggleCovers, 1, nullptr);

	logMsg("Adding items to aircraft menu...");
	XPLMMenuID aircraft_menu = XPLMFindAircraftMenu();
	if (aircraft_menu) {
		XPLMAppendMenuItemWithCommand(aircraft_menu, "Toggle Covers", commandCovers);
	}
}

void miscStop() {
	logMsg("Unregistering commands...");
	XPLMUnregisterCommandHandler(commandCovers, toggleCovers, 1, nullptr);
}