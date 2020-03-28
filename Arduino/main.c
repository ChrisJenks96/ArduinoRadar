#include <avr/io.h>
#include <util/delay.h>

//http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
//https://create.arduino.cc/projecthub/milanistef/introduction-to-bare-metal-programming-in-arduino-uno-f3e2b4
//https://www.arduino.cc/en/tutorial/blink
//http://www.rjhcoding.com/avr-asm-io.php
//https://en.wikiversity.org/wiki/AVR_programming_introduction/Simplistic_program

int main(void)
{
    //set to 1 the 5th bit of DDRB (digital pin 13) to output mode
    //DDRB - (digital pins 8-13) port D data directional register (read/write)
    DDRB |= (1 << DDB5);
    while (1)
    {
        //set to 1 the 5th bit of PORTB (Set HIGH to pin 13)
        PORTB |= (1 << PORTB5);
        _delay_ms(1000);
        //set to 0 the 5th bit of PORTB (Set LOW to pin 13)
        PORTB &= ~(1 << PORTB5);
        _delay_ms(1000);
    }

    return 0;   
}