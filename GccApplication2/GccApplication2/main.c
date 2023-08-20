#define F_CPU 16000000UL
#define LCD_Port PORTD			//PORTD LCD
#define LCD_DPin  DDRD			//4-Bit Pins 
#define RSPIN PD0			//RS Pin
#define ENPIN PD1 			//E Pin
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "LCD_lib.h" 
// ZMIENNE
volatile uint32_t liczBledy = 0;			//MISTAKES
volatile uint32_t czasMin = 0;		//MIN TIME REACTION FROM RESULTSTABLE 
volatile int licznik = 0; 
volatile unsigned long counter = 0;
volatile unsigned long loop = 0;
int t_wyniki[10] = {0, 0, 0, 0, 0, 0, 0, 0 , 0, 0 }; // RESULTS TABLE
volatile uint32_t clockp = 0;			
volatile uint32_t t_red = 0;  // REACTION TIME RED LED 
volatile uint32_t t_green = 0; // REACTION TIME GREEN LED 
int runtime; //Timer LCD
volatile bool G_mistake = false; //FLAGS 
volatile bool R_mistake = false;
volatile bool Y_mistake = false;
volatile uint32_t seedToTest = 0;
// FUN DECLARATIONS
void test1(void);
void test2(void);
void test3(void);
void _delay3000ms(int);
void _delay20ms(void);
int minTime(int arr[], int n);
void timer_setup(void);
void timer_start(void);
void timer_stop(void);
void LCD_Init(void); 
void LCD_Action(unsigned char cmnd);
void LCD_Clear(void);
void LCD_Print (char *str);
void LCD_Printpos(char row, char pos, char *str);
// INTERRUPS
ISR(TIMER0_COMPA_vect)
{
	counter++;
	if (counter > 1500) {
		counter = 0;
		loop ++ ;
		clockp++;
	}
	if (clockp > 10000) {
		clockp = 0;
	}
}
int main(void)
{
	/* IN: 13, 12, 11 pin Arduino: PB5, PB4, PB3 -> 0 
	//OUT: -> 1
	//LCD : 10 (RS), 8 (E), PD0, PD1, PD4, PD5,	PD6, PD7    
	//LEDS: 5 (GRE), 6 (RED), 9 (YEL) Arduino PINS:
			 PB0,	  PB2,	   PB1 */
	DDRB = 0b00000111;
	//START MESSAGE ON LCD:
	LCD_Init(); 
	LCD_Print("PRESS ANY SWITCH");	
	LCD_Action(0xC0);
	LCD_Print("TO START TEST");
	//WAIT FOR USER
	while(seedToTest == 0) {
		if (PINB & (1<< PINB3)) {
			seedToTest++;
		}
		if (PINB & (1<< PINB4)) {
			seedToTest += 80;
			seedToTest++;
		}
		if (PINB & (1<< PINB5)) {
			seedToTest += 160;
			seedToTest++;
		}	
	}
	//STARTING
	LCD_Clear();
	LCD_Print("STARTING...");	
	_delay_ms(3000);
	LCD_Clear();
	// SET TIMER
	timer_setup();
	//CHOOSE TEST
	srand(seedToTest);
	int wybor = rand()%300;
	wybor = (int)wybor/100;
	switch(wybor)
	{
		case 0: test2(); break;
		case 1: test1(); break;
		case 2: test3(); break;
	}
	// RESULTS 
	czasMin = minTime(t_wyniki, 10);
	char showTime [16];
	itoa(czasMin,showTime,10);
	char showLbBledow [16];
	itoa(liczBledy,showLbBledow,10);
	// DISPLAY RESULTS
	LCD_Print("Min time: ");
	LCD_Print(showTime);
	LCD_Print("ms");
	LCD_Action(0xC0);
	LCD_Print("Mistakes: ");
	LCD_Print(showLbBledow);
	_delay_ms(5000);
    while (1)   {	 }
}
int minTime(int arr[], int n) //FIND MIN
{
	int min = arr[0];
	for (int i = 1; i < n; i++) {
		if (arr[i] > 0) { //RULE OUT YELLOW LED
			if (arr[i] < min) {
				min = arr[i];
			}
		}
	}
	return min;
}
void timer_setup() //SET TIMER
{
	TCNT0 = 0;
	TCCR0A |= (1<<WGM12); // CTC
	TCCR0A |= (1<<CS02); //1024 PRESCALER
	TIMSK0|=(1<<OCIE0A); //UNBLOCK INTERRUPS TIMER1
	OCR0A = 31250;
	sei();
}
void timer_start()
{
	TCCR0B |= (1<<CS00) ;
}
void timer_stop()
{
	TCCR0A &= ~ ( (1<<CS00));
	TCNT0 = 0;
	loop=0;
}
void _delay3000ms(int dioda)
{
	loop = 0;
	int d = dioda; // 2,1,0
	timer_start();
	while(loop!=100){
		if (PINB & (1<< PINB3)) {//LOW -> HIGH PB3 - PRESSING BUTTON FOR GREEN LED *LEFT
			t_green = clockp ;
			_delay20ms();
			if (d != 0) { //GREEN LED NOT ON AND  PRESSED BUTTON -> MISTAKE
				G_mistake = true;
			}
		}
		if (PINB & (1<< PINB4)) { //LOW -> HIGH PB4 - PRESSING BUTTON FOR YELLOW LED *MID
			_delay20ms(); 
			Y_mistake = true; //DO NOT PRESS THIS AT ANY TIME -> MISTAKE
		}
		if (PINB & (1<< PINB5)) { //LOW -> HIGH PB5 - PRESSING BUTTON FOR RED LED *RIGHT
			t_red = clockp;
			_delay20ms();
			if (d != 2) { //RED LED ON AND  PRESSED BUTTON -> MISTAKE
				R_mistake = true;
			}
		}	
	}
	timer_stop();	
	return;
}
void test1() // R G Y R Y G R G R Y 
{	
	int t_diody = 0;
	int t_przycisk = 0;
	int t_miedzy = 0;
	int kolejnosc1[10] = {2, 0, 1, 2, 1, 0, 2, 0, 2, 1}; // RED - PB2, YELLOW - PB1, GREEN - PB0
	for(int i=0; i<10; i++) { //LOOP FOR EACH LED
		if (kolejnosc1[i] == 1) { //YELLOW
			PORTB |= 1<<kolejnosc1[i];
			_delay3000ms(kolejnosc1[i]);
			PORTB &= ~(1<<kolejnosc1[i]);
			t_wyniki[i] = -1; 
			if (Y_mistake) { //IF PRESSED MID BUTTON - MISTAKE++, FLAG OFF
				liczBledy++;
				Y_mistake = false;	}	} 
		else if (kolejnosc1[i] == 0) { //GREEN
			t_diody = clockp;
			PORTB |= 1<<kolejnosc1[i];
			_delay3000ms(kolejnosc1[i]);
			PORTB &= ~(1<<kolejnosc1[i]);
			if (R_mistake) { //MISTAKE
				liczBledy++;
				R_mistake = false;
				t_wyniki[i] = 3000;	}
			else if (t_green != 0) { //PRESSED
				t_przycisk = t_green;
				t_miedzy = t_przycisk -  t_diody; //DIFFERENCE
				t_wyniki[i] = t_miedzy * 30 ; }	//x30 TO GET MS
			else { // NOT PRESSED - MAX TIME IN TABLE AND MISTAKE++
				liczBledy++; 
				t_wyniki[i] = 3000;	}	}
		else { //RED
			t_diody = clockp;
			PORTB |= 1<<kolejnosc1[i];
			_delay3000ms(kolejnosc1[i]);
			PORTB &= ~(1<<kolejnosc1[i]);
			if (G_mistake) { //MISTAKE 
				liczBledy++;
				G_mistake = false;
				t_wyniki[i] = 3000;
	}
			else if (t_red != 0) { //PRESSED
				t_przycisk = t_red;
				t_miedzy = t_przycisk -  t_diody;
				t_wyniki[i] = t_miedzy * 30 ; 
			}
			else { // NOT PRESSED - MAX TIME IN TABLE AND MISTAKE++
				liczBledy++;
				t_wyniki[i] = 3000;
			}
		}
	}
	return;
}
void test2() // Y R Y G R R G Y G R
{
	int t_diody = 0;
	int t_przycisk = 0;
	int t_miedzy = 0;
	int kolejnosc1[10] = {1, 2, 1, 0, 2, 0, 2, 1, 0, 2};
	for(int i=0; i<10; i++) { //LOOP 
		if (kolejnosc1[i] == 1) {
			PORTB |= 1<<kolejnosc1[i];
			_delay3000ms(kolejnosc1[i]);
			PORTB &= ~(1<<kolejnosc1[i]);
			t_wyniki[i] = -1;
			if (Y_mistake) {
				liczBledy++;
				Y_mistake = false;		}	}
		else if (kolejnosc1[i] == 0) {
			t_diody = clockp;
			PORTB |= 1<<kolejnosc1[i];
			_delay3000ms(kolejnosc1[i]);
			PORTB &= ~(1<<kolejnosc1[i]);
			if (R_mistake)	{
				liczBledy++;
				R_mistake = false;
				t_wyniki[i] = 3000;	}
			else if (t_green != 0) {
				t_przycisk = t_green;
				t_miedzy = t_przycisk -  t_diody;  
				t_wyniki[i] = t_miedzy * 30 ; 		}
			else	{
				liczBledy++; 
				t_wyniki[i] = 3000;	}	}
		else 	{
			t_diody = clockp;
			PORTB |= 1<<kolejnosc1[i];
			_delay3000ms(kolejnosc1[i]);
			PORTB &= ~(1<<kolejnosc1[i]);
			if (G_mistake)	{
				liczBledy++;
				G_mistake = false;
				t_wyniki[i] = 3000;	}
			else if (t_red != 0)	{
				t_przycisk = t_red;
				t_miedzy = t_przycisk -  t_diody;
				t_wyniki[i] = t_miedzy * 30 ; 		}
			else {	
				liczBledy++;
				t_wyniki[i] = 3000;
			}
		}
	}		
	return;
}
void test3() // G Y G R G R Y R Y G
{
	int t_diody = 0;
	int t_przycisk = 0;
	int t_miedzy = 0;
	int kolejnosc1[10] = {0, 1, 0, 2, 0, 2, 1, 2, 1, 0};
	for(int i=0; i<10; i++){ //LOOP
		if (kolejnosc1[i] == 1) 	{
			PORTB |= 1<<kolejnosc1[i];
			_delay3000ms(kolejnosc1[i]);
			PORTB &= ~(1<<kolejnosc1[i]);
			t_wyniki[i] = -1;
			if (Y_mistake) {
				liczBledy++;
				Y_mistake = false;		}		}
		else if (kolejnosc1[i] == 0) {
			t_diody = clockp;
			PORTB |= 1<<kolejnosc1[i];
			_delay3000ms(kolejnosc1[i]);
			PORTB &= ~(1<<kolejnosc1[i]);
			if (R_mistake)	{
				liczBledy++;
				R_mistake = false;
				t_wyniki[i] = 3000;		}
			else if (t_green != 0) 	{
				t_przycisk = t_green;
				t_miedzy = t_przycisk -  t_diody; 
				t_wyniki[i] = t_miedzy * 30 ; 		}
			else	{
				liczBledy++; 
				t_wyniki[i] = 3000;		}	}
		else {
			t_diody = clockp;
			PORTB |= 1<<kolejnosc1[i];
			_delay3000ms(kolejnosc1[i]);
			PORTB &= ~(1<<kolejnosc1[i]);
			if (G_mistake)	{
				liczBledy++;
				G_mistake = false;
				t_wyniki[i] = 3000;		}
			else if (t_red != 0)	{
				t_przycisk = t_red;
				t_miedzy = t_przycisk -  t_diody;
				t_wyniki[i] = t_miedzy * 30 ; 		}
			else	{
				liczBledy++;
				t_wyniki[i] = 3000;		}		}	}
	return;
}
void _delay20ms()
{
	_delay_ms(20);
	return;
}
void LCD_Init (void)
{
	LCD_DPin = 0xFF;		// LCD Pin (D4-D7)
	_delay_ms(15);		
	LCD_Action(0x02);		//4-Bit Control
	LCD_Action(0x28);       //Control Matrix 4-Bit
	LCD_Action(0x0c);       //Disable Cursor
	LCD_Action(0x06);       //Move Cursor
	LCD_Action(0x01);       //Clean LCD
	_delay_ms(2);
}
void LCD_Action( unsigned char cmnd )
{
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0);
	LCD_Port &= ~ (1<<RSPIN);
	LCD_Port |= (1<<ENPIN);
	_delay_us(1);
	LCD_Port &= ~ (1<<ENPIN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);
	LCD_Port |= (1<<ENPIN);
	_delay_us(1);
	LCD_Port &= ~ (1<<ENPIN);
	_delay_ms(2);
}
void LCD_Clear()
{
	LCD_Action (0x01);		
	_delay_ms(2);			
	LCD_Action (0x80);		
}
void LCD_Print (char *str)
{
	int i;
	for(i=0; str[i]!=0; i++)	{
		LCD_Port = (LCD_Port & 0x0F) | (str[i] & 0xF0);
		LCD_Port |= (1<<RSPIN);
		LCD_Port|= (1<<ENPIN);
		_delay_us(1);
		LCD_Port &= ~ (1<<ENPIN);
		_delay_us(200);
		LCD_Port = (LCD_Port & 0x0F) | (str[i] << 4);
		LCD_Port |= (1<<ENPIN);
		_delay_us(1);
		LCD_Port &= ~ (1<<ENPIN);
		_delay_ms(2);
	}
}
void LCD_Printpos (char row, char pos, char *str)
{
	if (row == 0 && pos<16)
	LCD_Action((pos & 0x0F)|0x80);
	else if (row == 1 && pos<16)
	LCD_Action((pos & 0x0F)|0xC0);
	LCD_Print(str);
}