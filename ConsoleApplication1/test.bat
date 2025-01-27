@echo off
start adb shell
timeout /t 1 /nobreak >nul
echo getevent -l | adb shell