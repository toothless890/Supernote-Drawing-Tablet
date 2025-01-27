# Supernote Drawing Tablet
Designed to work with the Supernote A5X on a windows PC, but should be functional with relatively minor changes to any supernote device.

May work on most android devices changing /dev/input/event5 to whatever the digitizer is on that device

This DOES work wirelessly if you can figure out setting up a wireless ADB connection. But it will disconnect if theres not enough data being sent. Ill try to work on it more so that it reconnects automatically

# Installation instructions:
1. Download the executable from releases.

2. Install adb if you dont have it

   download at https://developer.android.com/tools/releases/platform-tools#downloads

    **adb.exe, AdbWinApi.dll, AdbWinUsbApi.dll MUST BE IN THE SAME FOLDER AS THE EXE**
3. unlock the adb shell
   This repo has an easy script for it https://github.com/TA1312/supernote-a5x/tree/master#recovery-mode 
6. Run the exe

#

# Goals:
Expand compatibility,
reduce external requirements,
get touch working to allow for buttons, like undo, zoom, erase mode, etc. 

#

Created in visual studio because I was trying something new.

The important file is main.cpp located in Supernote-Drawing-Tablet/Android2DrawingTablet/

I need to clean up a lot of stuff...

