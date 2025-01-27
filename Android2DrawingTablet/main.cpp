#include <iostream>
#include <Windows.h>
#include <sstream>
    

POINTER_TYPE_INFO* parse_getevent_output(const std::string& line, const HSYNTHETICPOINTERDEVICE device, POINTER_TYPE_INFO* inputInfo, int xscale, int yscale) {

    std::istringstream stream(line);
    std::string  event_type, event_code, event_value;
    if (stream >> event_type >> event_code >> event_value) {
        // convert the data into hex
        int event;
        sscanf_s(event_type.data(), "%x", &event);
        int code;
        sscanf_s(event_code.data(), "%x", &code);
        int value;
        sscanf_s(event_value.data(), "%x", &value);

        // handle each event type seperately 
        // TODO: is there a way to make this better using map? I hate having so many conditionals
        // Speed is currently sufficient but it irks me

        // send a input update whenever SYN is recieved
        if (event == 0) {
            InjectSyntheticPointerInput(device, inputInfo, 1);
        }
        // handle near or touching flags
        else if (event == 1) {
            if (code == 0x0140) {
                inputInfo[0].penInfo.pointerInfo.pointerFlags ^= POINTER_FLAG_INRANGE;
            }
            else if (code == 0x014a) {
                inputInfo[0].penInfo.pointerInfo.pointerFlags ^= POINTER_FLAG_INCONTACT;
            }
        }
        // handle events with values
        else if (event == 3) {
            if (code == 0x0000) {
                inputInfo[0].penInfo.pointerInfo.ptPixelLocation.x = value / xscale;
            }
            else if (code == 0x0001) {
                inputInfo[0].penInfo.pointerInfo.ptPixelLocation.y = value / yscale;
            }
            else if (code == 0x0018) {
                inputInfo[0].penInfo.pressure = value / 2;
            }
            else if (code == 0x001a) {
                // values are 16 bit signed ints, but are read as unsigned. Convert sign properly (Thanks ChatGPT)
                inputInfo[0].penInfo.tiltX = static_cast<int16_t>(value & 0xFFFF) / 100;
            }
            else if (code == 0x001b) {
                inputInfo[0].penInfo.tiltY = static_cast<int16_t>(value & 0xFFFF) / 100;
            }
        }
        else {
            // I have never seen this happen, but id like to know if it ever does.
            printf("%s", "ERROR, INVALID EVENT");
        }
    }
    return inputInfo;
}

int main() {
    ::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
    
    printf("%s", " Make sure adb.exe, AdbWinUsbApi.dll, and AdbWinApi.dll in the same folder as this executable!\n\n\n");

    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    // 20967 and 15725 are the limits of the display on the A5X. Other devices may differ. 
    int xscale = 20967 / width;
    int yscale = 15725 / height;

    // Open a process to execute the adb shell getevent command

    FILE* fp = _popen("adb shell -x getevent /dev/input/event5", "r");
    if (fp == nullptr) {
        std::cerr << "Failed to run adb command. " << std::endl;
        return 1;
    }
    std::cout << "\x1B[2J\x1B[H";
    char buffer[256];

    // Create and initialize the pen
    const HSYNTHETICPOINTERDEVICE device = CreateSyntheticPointerDevice(PT_PEN, 1, POINTER_FEEDBACK_DEFAULT);
    POINTER_TYPE_INFO inputInfo[1]{};
    inputInfo[0].type = PT_PEN;
    inputInfo[0].penInfo.penMask = PEN_MASK_PRESSURE | PEN_MASK_TILT_X | PEN_MASK_TILT_Y;

    // Read each line from the output of the getevent command
    while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        // Convert the buffer to a string
        std::string line(buffer);
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

        parse_getevent_output(line, device, inputInfo, xscale, yscale);
    }

    // Close the file if something bad happens
    fclose(fp);

    return 0;
}


//// ADB shell getevent translation
//std::map<int, std::string> eventmap;
//eventmap[0x0000] = "SYN";
//eventmap[0x0001] = "KEY";
//eventmap[0x0003] = "ABS";

//std::map<int, std::string> codemap;
//codemap[0x0140] = "BTN_DIGI"; // 1 = down, 0 = up
//codemap[0x0141] = "BTN_TOOL_RUBBER";
//codemap[0x014a] = "BTN_TOUCH"; // 1 = down, 0 = up
//codemap[0x014b] = "BTN_STYLUS";
//codemap[0x014c] = "BTN_STYLUS2";

//codemap[0x0000] = "ABS_X"; // 0-20967
//codemap[0x0001] = "ABS_Y"; // 0-17527 (On A5X!)
//codemap[0x0018] = "ABS_PRESSURE";
//codemap[0x0019] = "ABS_DISTANCE";
//codemap[0x001a] = "ABS_TILT_X";
//codemap[0x001b] = "ABS_TILT_Y";