// Hardware
/*
LED-Videoleuchte - YONGNUO YN-300 III
Arduino Uno
WS2812 5050 RGB-LED-Ring 24-Bit LED Ring
IR LED mit 220R Vorwiderstand 
atolla USB Hub mit ext. Stromversorgung
*/



// Windows Batch Datei dslrsend.bat
/*
@echo off
set /a "ComPort=6"
set "TAB=		"

::IF "%~1" == "session_start" (
::	set "send=session_start\n"
::	GOTO send
::)

IF "%~1" == "countdown_start" (
	set "send=countdown_start%~2%"
	set "TAB=	"
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
IF "%~1" == "printing" (
	set "send=printing %~2%"
	GOTO logprint
)


exit

:send
	start /MIN C:\tmp\SerialSend.exe /baudrate 9600 /devnum %ComPort% /hex %send%\n
	echo Datum: %date% %time% Ereignis: %send% >> c:\tmp\dslrbooth_status.txt 
::timeout /t 1

exit

:logprint
	echo Datum: %date% %time% Ereignis: %send% >> c:\tmp\dslrbooth_printed.txt 
::timeout /t 1

exit
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




// Windows Powershell Datei  gedruckteBilder.ps1
/*
Desktop Link: %windir%\System32\WindowsPowerShell\v1.0\powershell.exe -noexit -File "C:\tmp\gedruckteBilder.ps1"
Damit die Ausführung klappt: 
  PowerShell als Admin öffnen
    Set-ExecutionPolicy RemoteSigned

in BilderProRolle die Anzahl der Bilder eintragen, welche gedruckt werden können
in BereitsGedruckteBilder die Anzahl der Bilder eintragen, welche schon gedruckt wurden (zähle die Schnittstreifen ;-), aber nicht im Log stehen


clear
#Pfad zur txt Datei
$path = "c:\tmp\dslrbooth_printed.txt"
#Inputfile laden
$input = Get-Content $path

$counter = 0
$imagecnt = 0
$BilderProRolle = 300
$BereitsGedruckteBilder = 10
$BilderVerfuegbar = 0

	
$EventList = New-Object -TypeName System.Collections.ArrayList
$ImageList = New-Object -TypeName System.Collections.ArrayList

 
foreach ($temp in $input)
{
	$counter++
    $Inhalttrennen = $temp.Split(" ")
    $Datum_text = $Inhalttrennen[0]
    $Datum_date = $Inhalttrennen[1]
    $Datum_time = $Inhalttrennen[2]

    $Event_text = $Inhalttrennen[3]
    $Event_event = $Inhalttrennen[4]
    $Event_printed = $Inhalttrennen[5]
    $Eventtrennen = $temp.Split("\")
	$Eventname = $Eventtrennen[2]

	$EventList.Add($Eventname) > $null

}


$grouped = $EventList | Group-Object

foreach ($group in $grouped) {
$tab = "`t"
if ($group.Name.Length -lt 5) { $tab = $tab + "`t" }
    Write-Output ("Event: {0} {1} Bilder: {2}" -f $group.Name, $tab, $group.Count)
}

	$BilderVerfuegbar = $BilderProRolle - $BereitsGedruckteBilder - $counter

    $Ausgabe += "`n------------------------------------------------`n"
    $Ausgabe += "Anzahl Bilder pro Rolle: $BilderProRolle`n" 
    $Ausgabe += "Manuelle Angabe bereits gedruckter Bilder: `t$BereitsGedruckteBilder`n" 
    $Ausgabe += "Anzahl gedruckter Bilder lt. Log: `t`t$counter`n" 
    $Ausgabe += "Anzahl Bilder auf Rolle verfuegbar: `t`t$BilderVerfuegbar" 
    $Ausgabe
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
