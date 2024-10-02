/* 

neoPixel.h

editted by WilliamHVollita
Sep 23rd, 2024
Inspired by https://blog.csdn.net/weixin_67907028/article/details/134653661


Usage: 
#define Pixel_num                          --->   1 by default
#define neoPixel_tim                       --->   &htim3 by default
#define neoPixel_tim_channel               --->   TIM_CHANNEL_$ by default

Usage - optional:
#define CODE_1                             --->   59 by default
#define CODE_0                             --->   29 by default

Functions:
neoPixel_setOneColor(neoPixel_id, color)   --->   set neoPixel[id] as modified color
neoPixel_setRandColor(neoPixel_id)         --->   set neoPixel[id] as one random color in demo_colors[] table
neoPixel_sendData()                        --->   send current color table to physical neoPixel LED


*** NOTE that there're some typical color demos, you may check demo_colors[] below

*/


#include "stdlib.h"
#include "stm32f1xx.h"

// assume that there's only one neoPixel, however this value can be manually defined
#ifndef neoPixel_num
    #define neoPixel_num 1
#endif

// define 0 code and 1 code, which is actually the value of CCR register
// you may use PWM output mode 1, which means active level (configured as high level in CubeMX) when counter < CCR
// note that since APB1 freq has been set 72MHz in CubeMX by default,
// you'll have to set Prescaler value as 0, then Counter Period as 90,
// which will make it to 72000KHz / 90 = 800KHz (required freq for neoPixel)
#ifndef CODE_1
    #define CODE_1 (59) // counting time for 1 code counter, set Duty Cycle as 59 / 90 = 66%
#endif
#ifndef CODE_0
    #define CODE_0 (29) // counting time for 0 code counter, set Duty Cycle as 29 / 90 = 33%
#endif

// usually you should define your favored tim & channel
// however for demonstration, i would like to use tim3 & channel4
#ifndef neoPixel_tim
    #define neoPixel_tim &htim3
#endif
#ifndef neoPixel_tim_channel
    #define neoPixel_tim_channel TIM_CHANNEL_4
#endif


// store PWM output data, with each 24 bytes of data stands for one neoPixel LED
// note that the last 10 times 24 bytes would make sure there's 25us NULL output, which means data flow ended
uint32_t neoPixel_buf[neoPixel_num + 10][24];

typedef struct{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} RGB_TypeDef;

// define some typical color demo
const RGB_TypeDef demo_colors[] = {\
    {255, 64, 0}, \
    {128, 0, 255}, \
    {0, 0, 0}, \
    {255, 0, 0}, \
    {0, 255, 0}, \
    {0, 0, 255}, \
    {0, 255, 255}, \
    {255, 0, 220}, \
    {128, 216, 0}, \
    {127, 106, 0}, \
    {255, 255, 255}\
};
const RGB_TypeDef *ORANGE = &demo_colors[0];
const RGB_TypeDef *PURPLE = &demo_colors[1];
const RGB_TypeDef *BLACK = &demo_colors[2];
const RGB_TypeDef *RED = &demo_colors[3];
const RGB_TypeDef *GREEN = &demo_colors[4];
const RGB_TypeDef *BLUE = &demo_colors[5];
const RGB_TypeDef *SKY = &demo_colors[6];
const RGB_TypeDef *MAGENTA = &demo_colors[7];
const RGB_TypeDef *YELLOW = &demo_colors[8];
const RGB_TypeDef *OEANGE = &demo_colors[9];
const RGB_TypeDef *WHITE = &demo_colors[10];

// input from array, output to physical neoPixel LED
void neoPixel_sendData(){
    // make sure last 10 times 24 bytes are NULL
    uint8_t i;
    uint8_t ending_zone;
    for (ending_zone = 0; ending_zone < 10; ending_zone ++)
    {
        for (i = 0; i < 24; i ++)
        {
            neoPixel_buf[neoPixel_num + ending_zone][i] = 0;
        }
    }

    // send output data to physical neoPixel LED
    HAL_TIM_PWM_Start_DMA(neoPixel_tim, neoPixel_tim_channel, (uint32_t *)neoPixel_buf, (neoPixel_num + 10) * 24);
}

// main function
void neoPixel_setOneColor(uint8_t neoPixel_id, RGB_TypeDef color){
    uint8_t i;
    // set duration time of High Level output within one PWM period
    // in another word, set the value of PWM Duty Cycle register CCR1
	
	// inspired by the original auther, divide color value by 3 would  the light 
    for (i = 0; i < 8; i ++)
        neoPixel_buf[neoPixel_id][i] = (((color.G / 3) & (1 << (7 - i))) ? (CODE_1) : CODE_0); // store G value in [0] ~ [7]
    for (i = 8; i < 16; i ++)
        neoPixel_buf[neoPixel_id][i] = (((color.R / 3) & (1 << (15 - i))) ? (CODE_1) : CODE_0); // store R value in [8] ~ [15]
    for (i = 16; i < 24; i ++)
        neoPixel_buf[neoPixel_id][i] = (((color.B / 3) & (1 << (23 - i))) ? (CODE_1) : CODE_0); // store B value in [16] ~ [23]
    
    neoPixel_sendData();
}

void neoPixel_setRandColor(uint8_t neoPixel_id){
    uint8_t demo_color_cnt = sizeof(demo_colors) / sizeof(demo_colors[0]);
    //srand(time(NULL));
    uint8_t rand_color_id = rand() % demo_color_cnt + 1;
    neoPixel_setOneColor(neoPixel_id, demo_colors[rand_color_id - 1]);
}