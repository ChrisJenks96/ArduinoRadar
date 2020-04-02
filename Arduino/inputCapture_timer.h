/*
 * inputCapture_timer.h
 *
 *  Created on: Nov 17, 2017
 *      Author: abel
 */

#ifndef INPUTCAPTURE_TIMER_H_
#define INPUTCAPTURE_TIMER_H_

#define SET 1
#define CLEAR 0

#define FOSC 16000000UL
#define TICKS_VAL (FOSC/256)
#define F_OC2A 50000L
#define OC2A_PRESCALER 8L
#define OCR2A_VALUE (float)( (FOSC/(2*OC2A_PRESCALER*F_OC2A))-1)

volatile typedef enum edge_state_t
{
    INIT_RISING, RISING, FALLING
} edge_state;

volatile typedef struct 
{
	edge_state current_edge;
	edge_state next_edge;
} edge_t;

volatile edge_t edge;

#endif /* INPUTCAPTURE_TIMER_H_ */
