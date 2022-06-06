// Includes
#include <XPLMProcessing.h>
#include <XPLMMenus.h>
#include <acfutils/log.h>
#include <acfutils/except.h>
#include <cstring>

#include "misc.h"
#include "radio.h"

const char acfVersion[] = "v1.0.2";
XPLMFlightLoopID mainFlightLoop;

// Callbacks
float fl_main_update(float elapsedMe, float elapsedSim, int counter, void* refcon) {
	miscRefresh();
	return -1;
}

void logString(const char* string) {
	XPLMDebugString(string);
}

PLUGIN_API int XPluginStart(char* plugin_name, char* plugin_signature, char* plugin_description) {
	strcpy(plugin_name, "SimSolutions J3");
	strcpy(plugin_signature, "simsolutions.J3");
	strcpy(plugin_description, "SimSolutions J3");

	log_init(logString, "SimSolutions J3");
	logMsg("J3 Version: %s", acfVersion);

	except_init();

	miscStart();
	radioStart();

	return 1;
}

PLUGIN_API void	XPluginStop(void) { 	
	miscStop();
	radioStop();

	except_fini();
}

PLUGIN_API void XPluginDisable(void) {
	logMsg("Destroying primary flight loop...");
	XPLMDestroyFlightLoop(mainFlightLoop);
}

PLUGIN_API int  XPluginEnable(void) { 
	logMsg("Creating primary flight loop...");
	XPLMCreateFlightLoop_t fl_params {
		sizeof(XPLMCreateFlightLoop_t),
		xplm_FlightLoop_Phase_AfterFlightModel,
		fl_main_update,
		nullptr
	};
	mainFlightLoop = XPLMCreateFlightLoop(&fl_params);
	XPLMScheduleFlightLoop(mainFlightLoop, -1, 1);

	return 1; 
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void* inParam) {}