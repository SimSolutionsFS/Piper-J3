#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
#include <XPLMMenus.h>
#include <acfutils/log.h>
#include <acfutils/dr.h>
#include "misc.h"

int showCovers;
int showGroundClutter;
int showGun = 1;
int showWheelPants;
float waterRudderLowered = 0;
bool lowerWaterRudder;
bool isFloats;

dr_t drefShowGun;
dr_t drefShowCovers;
dr_t drefShowClutter;
dr_t drefWheelPants;
dr_t drefWaterRudder;
dr_t drefBatteryPowered;
dr_t drefEngineRunning;
dr_t drefWheelsonGround;
dr_t drefAcfDescription;

int toggleCovers(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon) {
	if (inPhase == 0) {
		showCovers = !showCovers;
	}

	return 1;
}
int toggleClutter(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon) {
	if (inPhase == 0) {
		showGroundClutter = !showGroundClutter;
	}

	return 1;
}
int toggleGun(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon) {
	if (inPhase == 0) {
		showGun = !showGun;
	}

	return 1;
}
int toggleWheelPants(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon) {
	if (inPhase == 0) {
		showWheelPants = !showWheelPants;
	}

	return 1;
}
int toggleWaterRudder(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon) {
	if (inPhase == 0) {
		lowerWaterRudder = !lowerWaterRudder;
	}

	return 1;
}

XPLMCommandRef commandCovers = XPLMCreateCommand("J3/Ground/ToggleCovers", "Toggle Covers");
XPLMCommandRef commandToggleClutter = XPLMCreateCommand("J3/Ground/ToggleClutter", "Toggle Ground Clutter");
XPLMCommandRef commandToggleGun = XPLMCreateCommand("J3/Ground/ToggleGun", "Toggle Gun");
XPLMCommandRef commandToggleWheelPants = XPLMCreateCommand("J3/Ground/ToggleWheelPants", "Toggle Wheel Pants");
XPLMCommandRef commandWaterRudder = XPLMCreateCommand("J3/WaterRudder/Toggle", "Toggle the water rudder");

void miscStart() {
	dr_create_i(&drefShowGun, &showGun, true, "J3/Ground/ShowGun");
	dr_create_i(&drefShowCovers, &showCovers, true, "J3/Ground/ShowCovers");
	dr_create_i(&drefShowClutter, &showGroundClutter, true, "J3/Ground/ShowClutter");
	dr_create_i(&drefWheelPants, &showWheelPants, true, "J3/Options/WheelPants");
	dr_create_f(&drefWaterRudder, &waterRudderLowered, true, "J3/WaterRudder/IsLowered");
	dr_find(&drefBatteryPowered, "sim/cockpit/electrical/battery_on");
	dr_find(&drefEngineRunning, "sim/flightmodel/engine/ENGN_running");
	dr_find(&drefWheelsonGround, "sim/flightmodel2/gear/on_ground");
	dr_find(&drefAcfDescription, "sim/aircraft/view/acf_descrip");

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

	char acfDescription[29];
	dr_gets(&drefAcfDescription, acfDescription, sizeof(acfDescription));

	if (strcmp(acfDescription, "SimSolutions J3 Cub - Floats") == 0) {
		isFloats = true;
	} 
}

int engineIsRunning;
int wheelOnGround;
void miscRefresh() {
	// Clear chocks & tent if engine is running
	dr_getvi(&drefEngineRunning, &engineIsRunning, 0, 1);
	if (isFloats) {
		dr_getvi(&drefWheelsonGround, &wheelOnGround, 4, 1);
	}
	else {
		dr_getvi(&drefWheelsonGround, &wheelOnGround, 0, 1);
	}
	
	if ((engineIsRunning == 1) && ((showCovers == 0) || (showGroundClutter == 0))) {
		logMsg("Engine running with covers or clutter set, removing!");
		showCovers = 1; 
		showGroundClutter = 1;
	}
	else if ((wheelOnGround == 0) && (showGroundClutter == 0)) {
		logMsg("Wheels off ground with clutter set, removing!");
		showGroundClutter = 1;
	}

	if (lowerWaterRudder) {
		waterRudderLowered = waterRudderLowered + 0.01f;
		if (waterRudderLowered > 1) {
			waterRudderLowered = 1;
		}
	}
	else {
		waterRudderLowered = waterRudderLowered - 0.01f;
		if (waterRudderLowered < 0) {
			waterRudderLowered = 0;
		}
	}

	dr_seti(&drefBatteryPowered, 1);
}

void miscStop() {
	logMsg("Unregistering commands...");
	XPLMUnregisterCommandHandler(commandCovers, toggleCovers, 1, nullptr);
	XPLMUnregisterCommandHandler(commandToggleClutter, toggleClutter, 1, nullptr);
	XPLMUnregisterCommandHandler(commandToggleGun, toggleGun, 1, nullptr);
	XPLMUnregisterCommandHandler(commandToggleWheelPants, toggleWheelPants, 1, nullptr);
	XPLMUnregisterCommandHandler(commandWaterRudder, toggleWaterRudder, 1, nullptr);
}