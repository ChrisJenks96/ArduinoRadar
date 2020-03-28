all:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o main.o main.c #create object file from source code
	avr-gcc -mmcu=atmega328p main.o -o main #create executable
	avr-objcopy -O ihex -R .eeprom main main.hex #create binary from executable
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyUSB0 -b 115200 -U flash:w:main.hex
clean:
	rm main.o main.hex main
