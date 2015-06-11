// Copyright (c) 2013-2015 United States Government as represented by the Administrator of the
// National Aeronautics and Space Administration. All Rights Reserved.
//
// DISCLAIMERS
//     No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY KIND,
//     EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT
//     THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED WARRANTIES OF
//     MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY
//     THAT THE SUBJECT SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED,
//     WILL CONFORM TO THE SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
//     ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS, RESULTING DESIGNS,
//     HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS RESULTING FROM USE OF THE SUBJECT
//     SOFTWARE.  FURTHER, GOVERNMENT AGENCY DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING
//     THIRD-PARTY SOFTWARE, IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
//     Waiver and Indemnity: RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE UNITED STATES
//     GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT.  IF
//     RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY LIABILITIES, DEMANDS, DAMAGES, EXPENSES
//     OR LOSSES ARISING FROM SUCH USE, INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING
//     FROM, RECIPIENT'S USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
//     UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT,
//     TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR ANY SUCH MATTER SHALL BE THE
//     IMMEDIATE, UNILATERAL TERMINATION OF THIS AGREEMENT.

#include "../src/xplaneConnect.h"
#include "stdio.h"

#ifdef WIN32
#include "Windows.h"

HANDLE console;

void setCursor(short x, short y)
{
	if (!console)
	{
		console = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 0;
		info.bVisible = FALSE;
		SetConsoleCursorInfo(console, &info);
	}
	COORD dwCursorPosition;
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;

	BOOL result = SetConsoleCursorPosition(console, dwCursorPosition);
	if (!result)
	{
		printf("Unable to set console cursor position. Failed with error code %i.\n", GetLastError());
		exit(EXIT_FAILURE);
	}
}
#else
#include "time.h"

void setCursor(short x, short y)
{
	// ???
}

void Sleep(int ms)
{
	struct timespec tv;
	tv.tv_nsec = ms * 1000 * 1000;
	nanosleep(tv, NULL);
}
#endif

void printChrome()
{
	setCursor(0, 0);

	// Set up initial screen, assuming the console is at least 72 characters wide and 16 lines tall.
	printf("+======================================================================+\n");
	printf("| X-Plane Connect Status Monitor Example Application                   |\n");
	printf("+===================+==================================================+\n");
	printf("| Aircraft Position | Aircraft Orientation | Aircraft Control Surfaces |\n");
	printf("+-------------------+----------------------+---------------------------+\n");
	printf("| Lat:              |  Roll:               |  Aileron:                 |\n");
	printf("| Lon:              | Pitch:               | Elevator:                 |\n");
	printf("| Alt:              |   Yaw:               |   Rudder:                 |\n");
	printf("|                   |                      |    Flaps:                 |\n");
	printf("+-------------------+----------------------+---------------------------+\n");
	printf("| Throttle:         | Gear:                |                           |\n");
	printf("+-------------------+----------------------+---------------------------+\n");
	printf("| Press Ctrl+C to quit.                                                |\n");
	printf("+======================================================================+\n");
}

void printLocation(float lat, float lon, float alt)
{
	setCursor(7, 5);
	printf("%4f", lat);
	setCursor(7, 6);
	printf("%4f", lon);
	setCursor(7, 7);
	printf("%2fm", alt);
}

void printOrientation(float roll, float pitch, float yaw)
{
	setCursor(29, 5);
	printf("%4f", roll);
	setCursor(29, 6);
	printf("%4f", pitch);
	setCursor(29, 7);
	printf("%4f", yaw);
}

void printControls(float aileron, float elevator, float rudder, float flaps)
{
	setCursor(55, 5);
	printf("%4f", aileron);
	setCursor(55, 6);
	printf("%4f", elevator);
	setCursor(55, 7);
	printf("%4f", rudder);
	setCursor(55, 8);
	printf("%4f", flaps);
}

void printThrottle(float throttle)
{
	setCursor(12, 10);
	printf("%2f", throttle);
}

void printGear(float gear)
{
	setCursor(28, 10);
	printf("%2f", gear);
}

int main(void)
{
	printChrome();
	XPCSocket client = openUDP("127.0.0.1");

	const int aircraftNum = 0;
	while (1)
	{
		float posi[7];
		getPOSI(client, posi, aircraftNum);
		printLocation(posi[0], posi[1], posi[2]);
		printOrientation(posi[4], posi[3], posi[5]);

		float ctrl[7];
		getCTRL(client, ctrl, aircraftNum);
		printControls(ctrl[1], ctrl[0], ctrl[2], ctrl[5]);
		printThrottle(ctrl[3]);
		printGear(ctrl[4]);
		Sleep(100);
	}
	return 0;
}