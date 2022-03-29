#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
#include <XPLMMenus.h>
#include <acfutils/log.h>

#include "misc.h"

int showCovers;
int showGroundClutter;
int showGun = 1;
int showWheelPants;
int waterRudderLowered = 0;

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
int getGun(void* inRefcon) {
	return showGun;
}
void setGun(void* inRefcon, int inValue) {
	showGun = inValue;
}
int getWheelPants(void* inRefcon) {
	return showWheelPants;
}
void setWheelPants(void* inRefcon, int inValue) {
	showWheelPants = inValue;
}
int getWaterRudderLowered(void* inRefcon) {
	return waterRudderLowered;
}
void setWaterRudderLowered(void* inRefcon, int inValue) {
	waterRudderLowered = inValue;
}

XPLMDataRef drefShowGun = XPLMRegisterDataAccessor("J3/Ground/ShowGun", xplmType_Int, true, getGun, setGun, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
XPLMDataRef drefShowTiedowns = XPLMRegisterDataAccessor("J3/Ground/ShowCovers", xplmType_Int, true, getCovers, setCovers, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
XPLMDataRef drefShowGroundClutter = XPLMRegisterDataAccessor("J3/Ground/ShowClutter", xplmType_Int, true, getShowGroundClutter, setShowGroundClutter, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
XPLMDataRef drefWheelPants = XPLMRegisterDataAccessor("J3/Options/WheelPants", xplmType_Int, true, getWheelPants, setWheelPants, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
XPLMDataRef drefWaterRudder = XPLMRegisterDataAccessor("J3/WaterRudder/IsLowered", xplmType_Int, true, getWaterRudderLowered, setWaterRudderLowered, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

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
int toggleGun(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon) {
	if (inPhase == 0) {
		if (showGun == 1) { 
			showGun = 0; 
		}
		else { 
			showGun = 1; 
		}
	}

	return 1;
}
int toggleWheelPants(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon) {
	if (inPhase == 0) {
		if (showWheelPants == 1) {
			showWheelPants = 0;
		}
		else {
			showWheelPants = 1;
		}
	}

	return 1;
}

int toggleWaterRudder(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon) {
	if (inPhase == 0) {
		if (waterRudderLowered == 1) {
			waterRudderLowered = 0;
		}
		else {
			waterRudderLowered = 1;
		}
	}

	return 1;
}

XPLMCommandRef commandCovers = XPLMCreateCommand("J3/Ground/ToggleCovers", "Toggle Covers");
XPLMCommandRef commandToggleClutter = XPLMCreateCommand("J3/Ground/ToggleClutter", "Toggle Ground Clutter");
XPLMCommandRef commandToggleGun = XPLMCreateCommand("J3/Ground/ToggleGun", "Toggle Gun");
XPLMCommandRef commandToggleWheelPants = XPLMCreateCommand("J3/Ground/ToggleWheelPants", "Toggle Wheel Pants");
XPLMCommandRef commandWaterRudder = XPLMCreateCommand("J3/WaterRudder/Toggle", "Toggle the water rudder");

void miscStart() {
	logMsg("Registering commands...");
	XPLMRegisterCommandHandler(commandCovers, toggleCovers, 1, nullptr);
	XPLMRegisterCommandHandler(commandToggleClutter, toggleClutter, 1, nullptr);
	XPLMRegisterCommandHandler(commandToggleGun, toggleGun, 1, nullptr);
	XPLMRegisterCommandHandler(commandToggleWheelPants, toggleWheelPants, 1, nullptr);
	XPLMRegisterCommandHandler(commandWaterRudder, toggleWaterRudder, 1, nullptr);

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
	XPLMUnregisterCommandHandler(commandToggleGun, toggleGun, 1, nullptr);
	XPLMUnregisterCommandHandler(commandToggleWheelPants, toggleWheelPants, 1, nullptr);
	XPLMUnregisterCommandHandler(commandWaterRudder, toggleWaterRudder, 1, nullptr);
}