/*
 * misc.cpp
 *
 * This registers and handles datarefs & commands,
 * as well as validates some visual options based
 * on the state of the aircraft.
 */
#include <XPLMMenus.h>
#include <acfutils/log.h>
#include <acfutils/dr.h>
#include <acfutils/cmd.h>
#include "misc.h"

// Variables
int show_covers;
int show_gnd_clutter;
int show_gun = true;
int show_wheel_pants;
float water_rudder_pos = 0;
bool water_rudder_lower;
bool on_floats;
int eng_running;
int wow;

// Datarefs
dr_t dr_show_gun;
dr_t dr_show_covers;
dr_t dr_show_clutter;
dr_t dr_show_wheel_pants;
dr_t dr_water_rudder;
dr_t dr_xp_batt;
dr_t dr_eng_running;
dr_t dr_wow;
dr_t dr_acf_desc;

// Commands
XPLMCommandRef cmd_toggle_covers = XPLMCreateCommand("J3/Ground/ToggleCovers", "Toggle Covers");
XPLMCommandRef cmd_toggle_clutter = XPLMCreateCommand("J3/Ground/ToggleClutter", "Toggle Ground Clutter");
XPLMCommandRef cmd_toggle_gun = XPLMCreateCommand("J3/Ground/ToggleGun", "Toggle Gun");
XPLMCommandRef cmd_toggle_wheel_pants = XPLMCreateCommand("J3/Ground/ToggleWheelPants", "Toggle Wheel Pants");
XPLMCommandRef cmd_water_rudder = XPLMCreateCommand("J3/WaterRudder/Toggle", "Toggle the water rudder");

// Handle toggle commands
int cmd_handler(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon) {
    if (inCommand == cmd_toggle_covers) {
        show_covers = !show_covers;
    }
    else if (inCommand == cmd_toggle_clutter) {
        show_gnd_clutter = !show_gnd_clutter;
    }
    else if (inCommand == cmd_toggle_gun) {
        show_gun = !show_gun;
    }
    else if (inCommand == cmd_toggle_wheel_pants) {
        show_wheel_pants = !show_wheel_pants;
    }
    else if (inCommand == cmd_water_rudder) {
        water_rudder_lower = !water_rudder_lower;
    }

    return 1;
}

void misc_start() {
    // Create & fetch datarefs
	dr_create_i(&dr_show_gun, &show_gun, true, "J3/Ground/ShowGun");
	dr_create_i(&dr_show_covers, &show_covers, true, "J3/Ground/ShowCovers");
	dr_create_i(&dr_show_clutter, &show_gnd_clutter, true, "J3/Ground/ShowClutter");
	dr_create_i(&dr_show_wheel_pants, &show_wheel_pants, true, "J3/Options/WheelPants");
	dr_create_f(&dr_water_rudder, &water_rudder_pos, true, "J3/WaterRudder/IsLowered");
	dr_find(&dr_xp_batt, "sim/cockpit/electrical/battery_on");
	dr_find(&dr_eng_running, "sim/flightmodel/engine/ENGN_running");
	dr_find(&dr_wow, "sim/flightmodel2/gear/on_ground");
	dr_find(&dr_acf_desc, "sim/aircraft/view/acf_descrip");

    // Register commands
    cmd_bind("J3/Ground/ToggleCovers", cmd_handler, true, nullptr);
    cmd_bind("J3/Ground/ToggleClutter", cmd_handler, true, nullptr);
    cmd_bind("J3/Ground/ToggleGun", cmd_handler, true, nullptr);
    cmd_bind("J3/Ground/ToggleWheelPants", cmd_handler, true, nullptr);
    cmd_bind("J3/WaterRudder/Toggle", cmd_handler, true, nullptr);

	// Set up aircraft menu
	XPLMMenuID acf_menu = XPLMFindAircraftMenu();
	if (acf_menu) {
		XPLMAppendMenuItemWithCommand(acf_menu, "Toggle Covers", cmd_toggle_covers);
		XPLMAppendMenuItemWithCommand(acf_menu, "Toggle Ground Clutter", cmd_toggle_clutter);
	}

    // Determine if this is the floats variant of the aircraft
	char acf_desc[128];
	dr_gets(&dr_acf_desc, acf_desc, sizeof(acf_desc));
    on_floats = (strcmp(acf_desc, "SimSolutions J3 Cub - Floats") == 0);
}

void misc_ref() {
	// Remove chocks & tent if engine is running
	dr_getvi(&dr_eng_running, &eng_running, 0, 1);
    dr_getvi(&dr_wow, &wow, (4 * on_floats), 1);
	if ((eng_running == 1) && ((show_covers == 0) || (show_gnd_clutter == 0))) {
		logMsg("Engine running with covers or clutter set, removing!");
        show_covers = 1;
        show_gnd_clutter = 1;
	}
	else if ((wow == 0) && (show_gnd_clutter == 0)) {
		logMsg("Wheels off ground with clutter set, removing!");
        show_gnd_clutter = 1;
	}

    // Handle the water rudder
	if (water_rudder_lower) {
        water_rudder_pos = water_rudder_pos + 0.01f;
		if (water_rudder_pos > 1) {
            water_rudder_pos = 1;
		}
	}
	else {
        water_rudder_pos = water_rudder_pos - 0.01f;
		if (water_rudder_pos < 0) {
            water_rudder_pos = 0;
		}
	}

    // Force X-Plane to think the battery is constantly on
	dr_seti(&dr_xp_batt, 1);
}

void misc_stop() {
	// Unregister commands
    cmd_unbind("J3/Ground/ToggleCovers", cmd_handler, true, nullptr);
    cmd_unbind("J3/Ground/ToggleClutter", cmd_handler, true, nullptr);
    cmd_unbind("J3/Ground/ToggleGun", cmd_handler, true, nullptr);
    cmd_unbind("J3/Ground/ToggleWheelPants", cmd_handler, true, nullptr);
    cmd_unbind("J3/WaterRudder/Toggle", cmd_handler, true, nullptr);
}