# Supernote Drawing Tablet
Designed to work with the Supernote A5X on a windows PC, but should be functional with relatively minor changes to any supernote device.

May work on most android devices changing /dev/input/event5 to whatever the digitizer is on that device


# Installation instructions:
1. Download the executable from releases.

2. Install adb if you dont have it

   download at https://developer.android.com/tools/releases/platform-tools#downloads

    **adb.exe, AdbWinApi.dll, AdbWinUsbApi.dll MUST BE IN THE SAME FOLDER AS THE EXE**
3. unlock the adb shell by running the script found at https://github.com/TA1312/supernote-a5x/tree/master#recovery-mode 
4. Run the exe

#

# Goals:
Expand compatibility,
reduce external requirements,
get touch working to allow for buttons, like undo, zoom, erase mode, etc. 

#

Created in visual studio because I was trying something new.

The important file is main.cpp located in Supernote-Drawing-Tablet/Android2DrawingTablet/

I need to clean up a lot of stuff...

