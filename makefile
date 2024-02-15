USB_PORT = /dev/ttyUSB0
MCU = esp8266:esp8266:nodemcuv2

compile:
	arduino-cli compile --fqbn $(MCU)

upload:
	arduino-cli upload --port $(USB_PORT) --fqbn $(MCU)

read:
	stty -F $(USB_PORT) raw 115200; cat $(USB_PORT)
