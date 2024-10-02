# Project.LightsUP
## demo project for STM32F1XX-based Sounder &amp; WS2812B RGB LED


for this project, i use `STM32F103C8T6` as the controller, 

then one `Sounder`, connected to `GPIO_B_0` under `output mode`

and one `WS2812B`, connected to `GPIO_B_1` under `PWM output mode`


i put functions for controlling the two circuits into two header files respectively,

which are `Sounder.h` and `neoPixel.h`


### Quick Start

to simply test this demo code, you may use STM32CubeMX to setup a Keil project based on `Project.LightsUP.ioc`, then replace `main.c` with provided one

also remember to include my custom headers in Keil Configurations   ::)

### Basic Usage

here's a brief usage

```
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
```

```
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


*** NOTE that there're some typical color demos, you may check demo_colors[] in the neoPixel.h
```

for more detailed usage, please check out the source code
