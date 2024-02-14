compile:
	arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2

upload:
	arduino-cli upload --port /dev/ttyUSB1 --fqbn esp8266:esp8266:nodemcuv2

read:
	stty -F /dev/ttyUSB1 raw 115200; cat /dev/ttyUSB1
