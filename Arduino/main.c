#include <stdlib.h>
#include <avr/io.h>
#include "uart.h"

#include "inputCapture_timer.h"

//http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
//https://create.arduino.cc/projecthub/milanistef/introduction-to-bare-metal-programming-in-arduino-uno-f3e2b4
//https://www.arduino.cc/en/tutorial/blink
//http://www.rjhcoding.com/avr-asm-io.php
//https://en.wikiversity.org/wiki/AVR_programming_introduction/Simplistic_program

//https://github.com/abtom87/Distance-measurement-

#define MAX_DIST 23200
#define SPEED_OF_SOUND_IN_CM_S (331/10)

volatile float temp = 0;
volatile float distance = 0;
volatile uint16_t dist_whole = 0;
volatile uint16_t dist_dec = 0;
volatile uint16_t ticks_t1 = 0;
volatile uint16_t ticks_t2 = 0;
volatile uint16_t elapsed_time = 0;
volatile static uint16_t tick_count = 0;
char value_buf[7] = { 0 };
char dec_val_buf[7] = { 0 };

void inputCaptureInit()
{
    //Enable ICP interrupt
    TIMSK1 = ((1 << ICIE1));
    //Enable rising edge detection, noise cancellation, clock Pre-scaler 256
    TCCR1B = ((1 << ICES1) | (1 << ICNC1) | (1 << CS12));
    edge.current_edge = INIT_RISING;
    edge.next_edge = INIT_RISING;
}

void timer2Init()
{
    //clear output compare match flag
    TIFR2 = 1 << OCF2A;
    //timer 2 compare match flag enabled
    TIMSK2 = 1 << OCIE2A;
    //CTC mode
    TCCR2A = 1 << WGM21;
    //clock prescalar 8
    TCCR2B = 1 << CS21;

}

unsigned long HCSR04Calculate()
{
    //LOW pulse to ensure clean high pulse (5 microseconds)
    /*PORTB &= ~(1 << PORTB3);
    _delay_us(5);
    //Sensor is triggered by a HIGH pulse (10 microseconds)
    PORTB |= (1 << PORTB3);
    _delay_us(10);
    PORTB &= ~(1 << PORTB3);*/

    //DO PULSE HERE....
    //https://github.com/abtom87/Distance-measurement-


    //(pin 12 - echoPin) - Input
    //DDRB |= (1 << DDB4);
    
    return 0;
}

//TRIGGER PIN 11 PORTB3
//ECHO PIN 12 PORTB4

int main(void)
{
    uartInit();
    //Enable internal pullups on PORTC Pins SDA(PC4), SCL(PC5)
    PORTC = 0x00;
    //DDRB - (digital pins 8-13) port D data directional register (read/write)
    //(pin 11 - trigPin) - Output
    DDRB &= ~(1 << DDB3);
    //(pin 12 - echoPin) - Input
    DDRB |= (1 << DDB4);
    //timer that generates an interrupt every 20uS
    OCR2A = (uint8_t)OCR2A_VALUE;
    //init timer and ICP
    inputCaptureInit();
    timer2Init();
    sei();

    while (1)
    {
        if (edge.current_edge == FALLING) 
        {
			cli();
			if (ticks_t2 > ticks_t1)
				temp = (float) (ticks_t2 - ticks_t1) / (float) TICKS_VAL;
			else
				temp = (float) ((65535 - ticks_t1) + ticks_t2) / (float) TICKS_VAL;
            //Calculate the pulse width in uS
			temp *= 1000000;
			elapsed_time = (uint16_t) temp;
			distance = ((float) SPEED_OF_SOUND_IN_CM_S * (float) elapsed_time) / (float) 2000;
            //print out
            //char buffer[32];
            //fprintf(&buffer[0], "%f\n", distance);
            //uartTransmitData(buffer);
            uartTransmitData("test");

			//dist_whole = (uint16_t) distance; /*Characteristic part of Number*/
			//dist_dec = (uint16_t) (((float) distance - (float) dist_whole) * 1000); /*Mantissa of the number*/
			sei();
        }
    }

        /*HCSR04Count = HCSR04Calculate();
        if (HCSR04Count >= MAX_DIST)
            uartTransmitData("Out of Range!");
        else
        {
            unsigned char HCSR04Str[10];
            ltoa(HCSR04Count, HCSR04Str, 10);
            uartTransmitData(HCSR04Str);
        }

        //wait for next measurement
        _delay_ms(250);
        }*/

    return 0;   
}

ISR(TIMER1_CAPT_vect) 
{
	cli();
	switch (edge.next_edge) 
    {
        case INIT_RISING: 
        {
            ticks_t1 = ICR1L;
            ticks_t1 |= (ICR1H << 8);
            /*Next Interrupt on Falling edge*/
            TCCR1B &= ~(1 << ICES1); 
            edge.next_edge = FALLING;

        }
        break;

        case RISING: 
        {
            ticks_t1 = (uint16_t) ICR1L;
            ticks_t1 |= (uint16_t) (ICR1H << 8);
            /*Next Interrupt on Falling edge*/
            TCCR1B &= ~(1 << ICES1); 
            edge.current_edge = RISING;
            edge.next_edge = FALLING;

        }
        break;

        case FALLING: 
        {
            ticks_t2 = (uint16_t) ICR1L;
            ticks_t2 |= (uint16_t) (ICR1H << 8);
            /*Next Interrupt on Rising edge*/
            TCCR1B |= (1 << ICES1); 
            edge.current_edge = FALLING;
            edge.next_edge = RISING;
        }
        break;

        default:
            break;
	}

	sei();
}

ISR(TIMER2_COMPA_vect) 
{
	cli();
	tick_count++;

	if (tick_count == 1){
		PORTB |= 1 << PORTB3;
	}

	if (tick_count == 2){
		PORTB &= ~(1 << PORTB3);
	}

	if (tick_count >= 700){
		tick_count = 0;
	}

	sei();
}