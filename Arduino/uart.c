#include "uart.h"

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
void uartTransmitData(unsigned char* data)
{
    while ((*data) != '\0'){
        //wait while the UDRE empty data bit is 0 in UCSRA
        while (!(UCSR0A & (1 << UDRE0)));
        //load data into the register which sets UDRE to 1 for transmission of the 1 byte char data
        UDR0 = *data;
        data++;
    }
}

unsigned char uartReceiveData(void)
{
    //wait while RXC is empty, when the bit is set, the unsigned char is received and transferred to UDR register 
    while (!(UCSR0A) & (1 << RXC0));
    //return the received data from UDR register
    return UDR0;
} 