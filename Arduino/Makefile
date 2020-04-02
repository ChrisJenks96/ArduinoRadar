all:
	#create object file from source code
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o main.o main.c 
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o uart.o uart.c 
	avr-gcc -mmcu=atmega328p main.o uart.o -o main.exe #create executable
	avr-objcopy -O ihex -R .eeprom main.exe main.hex #create binary from executable
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyUSB1 -b 115200 -U flash:w:main.hex
clean:
	rm main.o main.hex main.exe
