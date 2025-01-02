/*
 * plugin.c
 * Copyright 2025 Brady Margeson & Ian Ward
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include <string.h>
#include <XPLMProcessing.h>
#include <acfutils/except.h>
#include <acfutils/log.h>

#include "misc.h"
#include "radio.h"

#define ACF_VERSION "v1.0.2"
XPLMFlightLoopID acf_flt_loop;

float flt_loop(float elapsedMe, float elapsedSim, int counter, void *refcon) {
	misc_ref();
	return -1;
}

PLUGIN_API int XPluginStart(char *plugin_name, char *plugin_signature, char *plugin_description) {
	strcpy(plugin_name, "SimSolutions J3");
	strcpy(plugin_signature, "org.SimSolutions.J3");
	strcpy(plugin_description, "SimSolutions J3");

	log_init(XPLMDebugString, "SimSolutions J3");
	logMsg("J3 Version: %s", ACF_VERSION);

	except_init();
	misc_start();
	radio_start();

	return 1;
}

PLUGIN_API void XPluginStop(void) {
	misc_stop();
	radio_stop();
	except_fini();
}

PLUGIN_API void XPluginDisable(void) {
	XPLMDestroyFlightLoop(acf_flt_loop);
}

PLUGIN_API int XPluginEnable(void) {
	XPLMCreateFlightLoop_t fl_params = {
		sizeof(XPLMCreateFlightLoop_t),
		xplm_FlightLoop_Phase_AfterFlightModel,
		flt_loop,
		NULL
	};
	acf_flt_loop = XPLMCreateFlightLoop(&fl_params);
	XPLMScheduleFlightLoop(acf_flt_loop, -1, 1);

	return 1;
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void *inParam) {}
