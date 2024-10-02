/*

Sounder.h

editted by WilliamHVollita
Sep 24, 2024
Inspired by https://blog.csdn.net/u011017694/article/details/104314855


Usage:
#define GPIOX                   --->   GPIOB by default
#define GPIO_PIN_X              --->   GPIO_PIN_0 by default
									   note that the GPIO should be set as GPIO_OUTPUT with HIGH maximum output speed
#define overall_speed_handler   --->   customize playing speed, 16 by default

Usage - optional:
#define X_major                 --->   optionally enable X major, currently supports G, D, A, E and B major
#define delay_handler_tim       --->   use another tim for delay_us() if you want, &htim1 by default
									   note that you'll have to set Prescaler as 8-1
#define playingLED              --->   if you want to light an LED when playing a tone
#define playingLED_GPIO         --->   when playingLED is defined
#define playingLED_GPIO_PIN     --->   when playingLED is defined
#define playingOPERATION        --->   if you want to do something when playing a tone

Functions:
play_tone(tone, length)         --->   play a specific tone for a specific length
play_demo_tone_table()          --->   Travelers from "Outer Wilds"   :)

*/

// define sounder pin
#ifndef GPIOX
	#define GPIOX GPIOB
#endif
#ifndef GPIO_PIN_X
	#define GPIO_PIN_X GPIO_PIN_0
#endif
// define timer handle
#define delay_handler_tim &htim1

// define note length
#define FULL 8 // full note
#define HALF 4 // half note
#define QUAL 2 // qualter note
#define EIGH 1 // eighth note

// a coefficient for overall speed control
#ifndef overall_speed_handler
	#define overall_speed_handler 16
#endif

// define note freq
#define A2 110
#define B2 123

#define C3 131
#define D3 147
#define E3 165
#define F3 175
#define G3 196
#define A3 220
#define B3 247

#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494

#define C5 523
#define D5 587
#define E5 659
#define F5 698
#define G5 784

// call special major note
// e.g. if D_major is required in one piece
// then all specific majors for G_major should be followed
// so i just put 'em inside
#ifdef B_major
	#define E_major
	
	#undef A2
	#define A2 117
	#undef A3
	#define A3 233
	#undef A4
	#define A4 466
#endif

#ifdef E_major
	#define A_major
	
	#undef D3
	#define D3 147
	#undef D4
	#define D4 311
	#undef D5
	#define D5 622
#endif

#ifdef A_major
	#define D_major

	#undef G3
	#define G3 208
	#undef G4
	#define G4 415
	#undef G5
	#define G5 831
#endif

#ifdef D_major
	#define G_major
	
	#undef C3
	#define C3 139
	#undef C4
	#define C4 277
	#undef C5
	#define C5 554
#endif

#ifdef G_major
	#undef F3
	#define F3 185
	#undef F4
	#define F4 370
	#undef F5
	#define F5 740
#endif


// simple demo tone table
uint16_t demo_tone_table[] = { \
	D3, HALF, A3, HALF, C4, FULL, A3, QUAL, D4, QUAL, C4, QUAL, B3, QUAL, A3, QUAL, B3, QUAL, C4, QUAL, A3, HALF, \
	E3, HALF, A3, HALF, C4, FULL, A3, QUAL, D4, QUAL, C4, QUAL, B3, QUAL, A3, QUAL, B3, QUAL, C4, QUAL, E4, QUAL, C4, QUAL, \
	F3, HALF, A3, HALF, C4, FULL, A3, QUAL, D4, QUAL, C4, QUAL, B3, QUAL, A3, QUAL, B3, QUAL, C4, QUAL, A3, HALF, \
	E3, HALF, A3, HALF, C4, FULL, A3, QUAL, D4, QUAL, C4, QUAL, B3, QUAL, A3, QUAL, B3, QUAL, C4, QUAL, E4, QUAL, C4, QUAL, \
};


void delay_us(uint16_t nus){ 
	__HAL_TIM_SET_COUNTER(delay_handler_tim, 0);  // set the counter value a 0
	__HAL_TIM_ENABLE(delay_handler_tim);          // start the counter
	// wait for the counter to reach the us input in the parameter
	while (__HAL_TIM_GET_COUNTER(delay_handler_tim) < nus);
	__HAL_TIM_DISABLE(delay_handler_tim);         // stop the counter
}

void play_tone(uint16_t tone, uint8_t length){
	// if you want to do something when playing a tone
	#ifdef playingOPERATION
		playingOPERATION;
	#endif

	// if you want to light an LED when playing a tone
	#ifdef playingLED
		HAL_GPIO_WritePin(playingLED_GPIO, playingLED_GPIO_PIN, GPIO_PIN_RESET);
	#endif
	
	uint32_t freq_time = 500000 / tone;
	for(uint8_t n = 0; n < length * overall_speed_handler; n ++){
		for(uint16_t i = 0; i < tone / 12; i ++){
			HAL_GPIO_WritePin(GPIOX, GPIO_PIN_X, GPIO_PIN_SET);
			delay_us(freq_time);
			HAL_GPIO_WritePin(GPIOX, GPIO_PIN_X, GPIO_PIN_RESET);
			delay_us(freq_time);
		}
	}
	
	#ifdef playingLED
		HAL_GPIO_WritePin(playingLED_GPIO, playingLED_GPIO_PIN, GPIO_PIN_SET);
		HAL_Delay(50);
	#endif
}

void play_demo_tone_table(){
	
	for(uint8_t i = 0; i < sizeof(demo_tone_table) / sizeof(demo_tone_table[0]); i += 2){
		play_tone(demo_tone_table[i], demo_tone_table[i + 1]);
	}
	
}