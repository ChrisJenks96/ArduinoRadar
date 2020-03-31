#include <avr/io.h>
#include <util/delay.h>

//http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
//https://create.arduino.cc/projecthub/milanistef/introduction-to-bare-metal-programming-in-arduino-uno-f3e2b4
//https://www.arduino.cc/en/tutorial/blink
//http://www.rjhcoding.com/avr-asm-io.php
//https://en.wikiversity.org/wiki/AVR_programming_introduction/Simplistic_program

//UART code - async transmit/receive serial communication
//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/
//http://maxembedded.com/2013/09/the-usart-of-the-avr/
//UART SERIAL PINS RX(0) TX(1)

//Uno 16mhz
#define F_CPU 16000000UL

#define BAUD 9600
//Transmission and Receive rate for UBBR https://erg.abdn.ac.uk/users/gorry/eg3576/UART.html
#define BAUD_RATE ((F_CPU) / (BAUD * 16UL)-1)

#define TRIGGER_PIN 11
#define ECHO_PIN 12

//US = Ultrasonic Sensor
#define US PB1
#define US_PORT PORTB
#define US_DDR DDRB

void uartInit(void)
{
    //shift right 8 bits (div 8)
    UBRR0H = (BAUD_RATE >> 8);
    //set the baud rate
    UBRR0L = BAUD_RATE;
    //enable the UART transmission and receiving
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
    //8 bit data format in the frame when transmission and receiving
    UCSR0C = 0x06;//|= (1 << URSEL) | (1 << UCSZO) | (1 << UCSZ1);
}

//https://web.ics.purdue.edu/~jricha14/Serial_Stuff/UCSRA.htm
void uartTransmitData(unsigned char data)
{
    //wait while the UDRE empty data bit is 0 in UCSRA
    while (!(UCSR0A & (1 << UDRE0)));
    //load data into the register which sets UDRE to 1 for transmission of the 1 byte char data
    UDR0 = data;
}

unsigned char uartReceiveData(void)
{
    //wait while RXC is empty, when the bit is set, the unsigned char is received and transferred to UDR register 
    while (!(UCSR0A) & (1 << RXC0));
    //return the received data from UDR register
    return UDR0;
}

int main(void)
{
    long duration, cm, inches;

    uartInit();
    //DDRB - (digital pins 8-13) port D data directional register (read/write)
    //set 1 to 3rd bit of DDRB (Port) (pin 11 - trigPin) - Output
    US_DDR |= (1 << PB3); 
    //set 0 to 4rd bit of DDRB (Port) (pin 12 - echoPin) - Input
    US_DDR &= ~(1 << PB4);
    while (1)
    {
        //LOW pulse to ensure clean high pulse (5 microseconds)
        US_PORT &= ~(1 << PB3);
        _delay_ms(0.005);
        //Sensor is triggered by a HIGH pulse (10 microseconds)
        US_PORT |= (1 << PB3);
        _delay_ms(0.01);
        //toggle bit on port which would be LOW pulse
        US_PORT ^= (1 << PB3);
        //read the signal from the sensor: a HIGH pulse in microseconds
        //from sending the ping to receiving the echo
        //Set Port to input again
        US_DDR &= ~(1 << PB4);
        duration = pulseIn(ECHO_PIN, HIGH);
        uartTransmitData('A');
    }

    return 0;   
}