// Hardware
/*
LED-Videoleuchte - YONGNUO YN-300 III
Arduino Uno
WS2812 5050 RGB-LED-Ring 24-Bit LED Ring
IR LED mit 220R Vorwiderstand 
atolla USB Hub mit ext. Stromversorgung
*/


// Windows Batch Datei
/*
@echo off
set /a "ComPort=6"
set "TAB=    "

::IF "%~1" == "session_start" (
::  set "send=session_start\n"
::  GOTO send
::)

IF "%~1" == "countdown_start" (
  set "send=countdown_start%~2%"
  set "TAB= "
  GOTO send
)
IF "%~1" == "session_end" (
  set "send=session_end"
  GOTO send
)
IF "%~1" == "sharing_screen" (
  set "send=sharing_screen"
  GOTO send
)

exit

:send
  start /MIN C:\tmp\SerialSend.exe /baudrate 9600 /devnum %ComPort% /hex %send%\n
  echo Datum: %date% %time% Ereignis: %send% >> c:\tmp\dslrbooth_status.txt 
*/


// dslrbooth_status.txt
/*
Datum: 05.05.2024 20:20:08,80 Ereignis: countdown_start10  
Datum: 05.05.2024 20:20:19,04 Ereignis: sharing_screen  
Datum: 05.05.2024 20:20:24,45 Ereignis: session_end  
Datum: 05.05.2024 20:20:59,77 Ereignis: session_end  
Datum: 05.05.2024 20:21:01,82 Ereignis: countdown_start10  
Datum: 05.05.2024 20:21:11,36 Ereignis: countdown_start5  
Datum: 05.05.2024 20:21:16,43 Ereignis: sharing_screen  
Datum: 05.05.2024 20:21:21,84 Ereignis: session_end  
Datum: 05.05.2024 20:23:42,17 Ereignis: session_end  
*/

// Circuit
/*
IR LED [Anode] <> [220]R <>  Arduino UNO [3]
IR LED [Cathode] <> Arduino UNO [GND]

WS2412 24LED Ring [DIN] <> [1000]R <>  Arduino UNO [12]
WS2412 24LED Ring [5V] <> atolla USB Hub [5V +]
WS2412 24LED Ring [GND] <> atolla USB Hub [5V GND]
atolla USB Hub [5V +] <> Elko [2200]uF 25V <> atolla USB Hub [5V GND]

Arduino UNO USB B <> atolla USB Hub
atolla USB Hub <> Windows PC
*/
