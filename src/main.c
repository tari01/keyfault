/*
    Copyright 2016-2020 Robert Tari <robert.tari@gmail.com>

    This file is part of KeyFault.

    KeyFault is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    KeyFault is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with KeyFault.  If not, see <http://www.gnu.org/licenses/gpl-3.0.txt>.
*/

#include <unistd.h>
#include <stdio.h>
#include <X11/extensions/scrnsaver.h>
#include <X11/XKBlib.h>

#define APPVERSION "23.1.31"

int main(int argc, char* argv[])
{
    unsigned int nTimeout = 3600000;

    while (1)
    {
        Display* pDisplay = XOpenDisplay(NULL);

        if (!pDisplay)
        {
            printf("PANIC: XOpenDisplay\n");

            return 1;
        }

        XScreenSaverInfo pXScreenSaverInfo;
        XScreenSaverQueryInfo(pDisplay, DefaultRootWindow(pDisplay), &pXScreenSaverInfo);

        if (pXScreenSaverInfo.state == ScreenSaverOn || pXScreenSaverInfo.idle > nTimeout)
        {
            XkbIgnoreExtension(False);

            char *sDisplayName = "";
            int nEventCode;
            int nErrorReturn;
            int nMajor = XkbMajorVersion;
            int nMinor = XkbMinorVersion;
            int nReasonReturn;
            Display* pDisplay2 = XkbOpenDisplay(sDisplayName, &nEventCode, &nErrorReturn, &nMajor, &nMinor, &nReasonReturn);

            if (nReasonReturn != XkbOD_Success)
            {
                printf("PANIC: XkbOpenDisplay\n");
                return 1;
            }

            XkbStateRec pXkbStateRec;
            XkbGetState(pDisplay2, XkbUseCoreKbd, &pXkbStateRec);

            if (pXkbStateRec.group != 0)
            {
                if (XkbLockGroup(pDisplay2, XkbUseCoreKbd, 0) == False)
                {
                    printf("PANIC: XkbLockGroup\n");
                    return 1;
                }

                XkbGetState(pDisplay2, XkbUseCoreKbd, &pXkbStateRec);
            }

            XCloseDisplay(pDisplay2);
            pDisplay2 = NULL;
        }

        XCloseDisplay(pDisplay);
        pDisplay = NULL;

        sleep(10);
    }
}
