set onMode to false
set use_port to "/dev/cu.usbmodemfa141"
serialport close use_port
set myText to choose file
set myLength to get eof of myText

display dialog "File Length is " & myLength

open for access myText without write permission

repeat until (get serialport list) contains use_port
	delay 3
end repeat
if (get serialport list) contains use_port then set onMode to true
set myPort to serialport open use_port bps rate 115200 data bits 8 parity 0 stop bits 1 handshake 0
delay 1
if myPort is equal to -1 then display dialog "could not open port"
delay 1

serialport write (ASCII character (13)) to myPort -- Carraige Return

display dialog "All ready?"
delay 0.5

set i to 1
set myLength to myLength + 1
set n to 0

repeat until i = myLength
	
	if n is greater than 30 then
		set waitFlag to serialport read myPort
		if waitFlag = "@" then
			set n to 0
		end if
		delay 0.5
	end if
	
	set thisCharacter to read myText from i for 1
	serialport write thisCharacter to myPort
	set i to i + 1
	set n to n + 1
	delay 0.01
	
end repeat

serialport write (ASCII character (13)) to myPort -- Carraige Return

close access myText
delay 1
display dialog "All Finished!"
serialport close use_port