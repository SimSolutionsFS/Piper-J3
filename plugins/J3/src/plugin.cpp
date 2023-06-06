#include <XPLMProcessing.h>
#include <XPLMMenus.h>
#include <acfutils/log.h>
#include <acfutils/except.h>
#include <cstring>

#include "misc.h"
#include "radio.h"

#define ACF_VERSION "v1.0.2"
XPLMFlightLoopID acf_flt_loop;

float flt_loop(float elapsedMe, float elapsedSim, int counter, void* refcon) {
    misc_ref();
	return -1;
}

PLUGIN_API int XPluginStart(char* plugin_name, char* plugin_signature, char* plugin_description) {
	strcpy(plugin_name, "SimSolutions J3");
	strcpy(plugin_signature, "org.simsolutions.J3");
	strcpy(plugin_description, "SimSolutions J3");

	log_init(XPLMDebugString, "SimSolutions J3");
	logMsg("J3 Version: %s", ACF_VERSION);

	except_init();
    misc_start();
    radio_start();

	return 1;
}
PLUGIN_API void	XPluginStop(void) {
    misc_stop();
	radio_stop();

	except_fini();
}
PLUGIN_API void XPluginDisable(void) {
	XPLMDestroyFlightLoop(acf_flt_loop);
}
PLUGIN_API int  XPluginEnable(void) {
	XPLMCreateFlightLoop_t fl_params {
        sizeof(XPLMCreateFlightLoop_t),
        xplm_FlightLoop_Phase_AfterFlightModel,
        flt_loop,
        nullptr
	};
    acf_flt_loop = XPLMCreateFlightLoop(&fl_params);
	XPLMScheduleFlightLoop(acf_flt_loop, -1, 1);

	return 1; 
}
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void* inParam) {}