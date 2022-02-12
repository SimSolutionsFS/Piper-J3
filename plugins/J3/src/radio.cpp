#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
#include <XPLMMenus.h>
#include <XPLMDisplay.h>
#include <XPLMGraphics.h>
#include <XPLMPlanes.h>
#include <acfutils/log.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "radio.h"

#if IBM
#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif
#if LIN
	#include <GL/gl.h>
#elif __GNUC__
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif
	#ifndef XPLM301
	#error This is made to be compiled against the XPLM300 SDK
#endif

XPLMDataRef drefCom1Radio = XPLMFindDataRef("sim/cockpit2/radios/actuators/com1_frequency_hz_833");
XPLMDataRef drefCom1Power = XPLMFindDataRef("sim/cockpit2/radios/actuators/com1_power");

XPLMCommandRef radio1 = XPLMCreateCommand("J3/Radio/Num1", "Handheld Radio Press 1");
XPLMCommandRef radio2 = XPLMCreateCommand("J3/Radio/Num2", "Handheld Radio Press 2");
XPLMCommandRef radio3 = XPLMCreateCommand("J3/Radio/Num3", "Handheld Radio Press 3");
XPLMCommandRef radio4 = XPLMCreateCommand("J3/Radio/Num4", "Handheld Radio Press 4");
XPLMCommandRef radio5 = XPLMCreateCommand("J3/Radio/Num5", "Handheld Radio Press 5");
XPLMCommandRef radio6 = XPLMCreateCommand("J3/Radio/Num6", "Handheld Radio Press 6");
XPLMCommandRef radio7 = XPLMCreateCommand("J3/Radio/Num7", "Handheld Radio Press 7");
XPLMCommandRef radio8 = XPLMCreateCommand("J3/Radio/Num8", "Handheld Radio Press 8");
XPLMCommandRef radio9 = XPLMCreateCommand("J3/Radio/Num9", "Handheld Radio Press 9");
XPLMCommandRef radio0 = XPLMCreateCommand("J3/Radio/Num0", "Handheld Radio Press 0");
XPLMCommandRef radioCLR = XPLMCreateCommand("J3/Radio/CLR", "Handheld Radio Press CLR");
XPLMCommandRef radiodot = XPLMCreateCommand("J3/Radio/dot", "Handheld Radio Press dot");

char freqBuffer[7];
int pos = 0;
int fontTextureID;

int loadImage(const std::string &fileName)
{
	int imgWidth, imgHeight, nComps;

	uint8_t *data = stbi_load(fileName.c_str(), &imgWidth, &imgHeight, &nComps, sizeof(uint32_t));

	if (!data)
	{
		logMsg("Couldn't load image: ");
		logMsg(stbi_failure_reason());
	}

	int id;
	XPLMGenerateTextureNumbers(&id, 1);
	XPLMBindTexture2d(id, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0,
				 GL_RGBA, imgWidth, imgHeight, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	return id;
}

int radioAppend(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void *inRefcon)
{
	int buttonvalue = '-';
	if (inPhase == 2)
	{
		if (inCommand == radio1)
			buttonvalue = 1;
		else if (inCommand == radio2)
			buttonvalue = 2;
		else if (inCommand == radio3)
			buttonvalue = 3;
		else if (inCommand == radio4)
			buttonvalue = 4;
		else if (inCommand == radio5)
			buttonvalue = 5;
		else if (inCommand == radio6)
			buttonvalue = 6;
		else if (inCommand == radio7)
			buttonvalue = 7;
		else if (inCommand == radio8)
			buttonvalue = 8;
		else if (inCommand == radio9)
			buttonvalue = 9;
		else if (inCommand == radio0)
			buttonvalue = 0;

		if (buttonvalue != '-')
		{
			freqBuffer[pos] = buttonvalue + '0';
			pos++;
		}
		else
		{
			// If CLR is pressed
			strcpy(freqBuffer, "-----");
			pos = 0;
		}
		if (pos > 5)
		{
			pos = 0;
			XPLMSetDatai(drefCom1Radio, atoi(freqBuffer));
		}
	}
	return 1;
}

int radioDraw(XPLMDrawingPhase inPhase, int inIsBefore, void *inRefcon)
{

	XPLMSetGraphicsState(0, 1, 0, 0, 1, 0, 0);
	float col_white[] = {1.0, 1.0, 1.0};
	int left = 0;
	int right = 100;
	int top = 100;
	int bottom = 0;
	XPLMBindTexture2d(fontTextureID, 0);

	int x = 200;
	int y = 1620;
	int size = 264;
	int height = 128;
	int width = 512;
	for (size_t i = 0; i < strlen(freqBuffer); i++)
	{
		int number = freqBuffer[i] - '0';
		if (number >= 0 && number <= 9)
		{
			int trow = number / 5;
			int tcol = number % 5;

			float row = trow;
			float col = tcol;

			// identify topleft

			glBegin(GL_QUADS);
			// map top left texture to bottom left vertex
			glTexCoord2f(col / 8, (row + 1) / 2);
			// glTexCoord2i(0, 1);
			glVertex2i(x + i * size, y);

			// map bottom left texture to top left vertex
			glTexCoord2f(col / 8, row / 2);
			// glTexCoord2i(0, 0);
			glVertex2i(x + i * size, y + size);

			// map bottom right texture to top right vertex
			glTexCoord2f((col + 1) / 8, row / 2);
			// glTexCoord2i(1, 0);
			glVertex2i(x + (i + 1) * size, y + size);

			// map top right texture to bottom right vertex
			glTexCoord2f((col + 1) / 8, (row + 1) / 2);
			// glTexCoord2i(1, 1);
			glVertex2i(x + (i + 1) * size, y);
			glEnd();
		}
	}
	float coldot = 5;
	float rowdot = 0;
	glBegin(GL_QUADS);
	// map top left texture to bottom left vertex
	glTexCoord2f(coldot / 8, (rowdot + 1) / 2);
	// glTexCoord2i(0, 1);
	glVertex2i(x + 2.5 * size, y);

	// map bottom left texture to top left vertex
	glTexCoord2f(coldot / 8, rowdot / 2);
	// glTexCoord2i(0, 0);
	glVertex2i(x + 2.5 * size, y + size);

	// map bottom right texture to top right vertex
	glTexCoord2f((coldot + 1) / 8, rowdot / 2);
	// glTexCoord2i(1, 0);
	glVertex2i(x + (2.5 + 1) * size, y + size);

	// map top right texture to bottom right vertex
	glTexCoord2f((coldot + 1) / 8, (rowdot + 1) / 2);
	// glTexCoord2i(1, 1);
	glVertex2i(x + (2.5 + 1) * size, y);
	glEnd();
	return 1;
}

void radioStart()
{
	logMsg("Registering radio commands...");
	XPLMRegisterCommandHandler(radio1, radioAppend, 1, nullptr);
	XPLMRegisterCommandHandler(radio2, radioAppend, 1, nullptr);
	XPLMRegisterCommandHandler(radio3, radioAppend, 1, nullptr);
	XPLMRegisterCommandHandler(radio4, radioAppend, 1, nullptr);
	XPLMRegisterCommandHandler(radio5, radioAppend, 1, nullptr);
	XPLMRegisterCommandHandler(radio6, radioAppend, 1, nullptr);
	XPLMRegisterCommandHandler(radio7, radioAppend, 1, nullptr);
	XPLMRegisterCommandHandler(radio8, radioAppend, 1, nullptr);
	XPLMRegisterCommandHandler(radio9, radioAppend, 1, nullptr);
	XPLMRegisterCommandHandler(radio0, radioAppend, 1, nullptr);
	XPLMRegisterCommandHandler(radioCLR, radioAppend, 1, nullptr);
	// XPLMRegisterCommandHandler(radiodot, radioAppend, 1, nullptr);

	XPLMRegisterDrawCallback(radioDraw, xplm_Phase_Gauges, 1, nullptr);

	/* FONTS SETUP */
	char rootPath[512];
	char temp[256];
	char relPath[] = "plugins/J3/radioscreen.png";
	char *name_with_extension;
	// logMsg("Variables initialized");
	XPLMGetNthAircraftModel(0, temp, rootPath);
	// logMsg("Aircraft model gotten");
	rootPath[strlen(rootPath) - 7] = '\0'; // Cutting the J-3.acf from the end of the path. Really hope we don't change the filename
	name_with_extension = (char *)malloc(strlen(rootPath) + 1 + strlen(relPath));
	// logMsg("Malloc");
	strcpy(name_with_extension, rootPath);
	// logMsg("ccpy");
	strcat(name_with_extension, relPath);
	// logMsg(name_with_extension);
	fontTextureID = loadImage(name_with_extension);
	XPLMSetDatai(drefCom1Power, 1);
	itoa(XPLMGetDatai(drefCom1Radio), freqBuffer, 10);
	// logMsg("Texture loaded");
}

void radioStop()
{
	logMsg("Unregistering commands...");
	XPLMUnregisterCommandHandler(radio1, radioAppend, 1, nullptr);
	XPLMUnregisterDrawCallback(radioDraw, xplm_Phase_Gauges, 1, nullptr);
}