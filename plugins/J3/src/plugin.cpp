// Includes
#include <XPLMPlugin.h>
#include <XPLMUtilities.h>
#include <XPLMProcessing.h>
#include <XPLMMenus.h>
#include <XPLMDataAccess.h>
#include <acfutils/log.h>
#include <cstring>
#include "misc.h"

// Variables
const char acfVersion[] = "v1.0.0";
XPLMFlightLoopID mainFlightLoop;

// Callbacks
void logString(const char* string) {
	XPLMDebugString(string);
}

PLUGIN_API int XPluginStart(char* plugin_name, char* plugin_signature, char* plugin_description) {
	strcpy(plugin_name, "SimSolutions J3");
	strcpy(plugin_signature, "simsolutions.J3");
	strcpy(plugin_description, "SimSolutions J3");

	log_init(logString, "SimSolutions J3");
	logMsg("J3 Version: %s", acfVersion);

	miscStart();

	return 1;
}
PLUGIN_API void	XPluginStop(void) { 	
	miscStop();
}
PLUGIN_API void XPluginDisable(void) {}
PLUGIN_API int  XPluginEnable(void) { 
	return 1; 
}
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void* inParam) {}