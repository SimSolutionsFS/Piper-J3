#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
#include <XPLMMenus.h>
#include <acfutils/log.h>

#include "misc.h"

int showCovers;
int showGroundClutter;

int getCovers(void* inRefcon) { 
	return showCovers; 
}
void setCovers(void* inRefcon, int inValue) { 
	showCovers = inValue; 
}
int getShowGroundClutter(void* inRefcon) { 
	return showGroundClutter; 
}
void setShowGroundClutter(void* inRefcon, int inValue) { 
	showGroundClutter = inValue; 
}

XPLMDataRef drefShowTiedowns = XPLMRegisterDataAccessor("J3/Ground/ShowCovers", xplmType_Int, true, getCovers, setCovers, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
XPLMDataRef drefShowGroundClutter = XPLMRegisterDataAccessor("J3/Ground/ShowClutter", xplmType_Int, true, getShowGroundClutter, setShowGroundClutter, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
XPLMDataRef drefBatteryPowered = XPLMFindDataRef("sim/cockpit/electrical/battery_on");

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

int toggleClutter(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon) {
	if (inPhase == 0) {
		if (showGroundClutter == 1) { 
			showGroundClutter = 0; 
		}
		else { 
			showGroundClutter = 1; 
		}
	}

	return 1;
}

XPLMCommandRef commandCovers = XPLMCreateCommand("J3/Ground/ToggleCovers", "Toggle Covers");
XPLMCommandRef commandToggleClutter = XPLMCreateCommand("J3/Ground/ToggleClutter", "Toggle Ground Clutter");

void miscStart() {
	logMsg("Registering commands...");
	XPLMRegisterCommandHandler(commandCovers, toggleCovers, 1, nullptr);
	XPLMRegisterCommandHandler(commandToggleClutter, toggleClutter, 1, nullptr);

	logMsg("Adding items to aircraft menu...");
	XPLMMenuID aircraft_menu = XPLMFindAircraftMenu();
	if (aircraft_menu) {
		XPLMAppendMenuItemWithCommand(aircraft_menu, "Toggle Covers", commandCovers);
		XPLMAppendMenuItemWithCommand(aircraft_menu, "Toggle Ground Clutter", commandToggleClutter);
	}
}

void miscRefresh() {
	XPLMSetDatai(drefBatteryPowered, 1);
}

void miscStop() {
	logMsg("Unregistering commands...");
	XPLMUnregisterCommandHandler(commandCovers, toggleCovers, 1, nullptr);
	XPLMUnregisterCommandHandler(commandToggleClutter, toggleClutter, 1, nullptr);
}