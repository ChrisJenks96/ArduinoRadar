#ifndef UART_H
#define UART_H

#include <avr/interrupt.h>
#include <util/delay.h>

//UART code - async transmit/receive serial communication
//https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/
//http://maxembedded.com/2013/09/the-usart-of-the-avr/
//UART SERIAL PINS RX(0) TX(1)

//Uno 16mhz
#define F_CPU 16000000UL

#define BAUD 9600
//Transmission and Receive rate for UBBR https://erg.abdn.ac.uk/users/gorry/eg3576/UART.html
#define BAUD_RATE ((F_CPU) / (BAUD * 16UL)-1)

void uartInit(void);
void uartTransmitData(unsigned char* data);
unsigned char uartReceiveData(void);

#endif