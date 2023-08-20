//Zestaw naglowkow funkcji obslugi wyswietlacza LCD pracuj¹cego w trybie tekstowym
#ifndef LCD_lib
#define LCD_lib

#include <inttypes.h>

void LCDsendChar(uint8_t);		                //wyslanie pojedynczego znaku (danych) do 74HC164
void LCDsendCommand(uint8_t);	                        //Wyslanie rozkazu sterujacego do 74HC164
void LCDinit(void);			                //Kilkuetapowa inicjalizacja LCD
void LCDclr(void);				        //Czysc LCD
void LCDhome(void);			                //LCD Kusor do pozycji domowej
void LCDstring(uint8_t*, uint8_t);	                    //wyslij caly string do LCD
void LCDGotoXY(uint8_t, uint8_t);	                    //przesun kursor do pozycji: X Y 
void CopyStringtoLCD(const uint8_t*, uint8_t, uint8_t); //kopiuj lanuch z pamieci flash do LCD w punkcie x,y
void LCDdefinechar(const uint8_t *,uint8_t);            //wpisz znak do LCD CGRAM (czy tworzenie wlasnego znaku) 
void LCDshiftRight(uint8_t);	                        //przesun wysw. LCD o n znakow w prawo
void LCDshiftLeft(uint8_t);	                            //przesun wysw. LCD o n znakow w lewo
void LCDcursorOn(void);		                            //wlacz kursona ON
void LCDcursorOnBlink(void);	                        //wlacz miganie kursora ON
void LCDcursorOFF(void);		                        //kursor wylaczony OFF
void LCDblank(void);		 	                        //ukryj wysw. ale nie czysc
void LCDvisible(void);			                        //pokaz wysw.
void LCDcursorLeft(uint8_t);	                        //przesun kursor w lewo o n
void LCDcursorRight(uint8_t);	                        //przesun kursor w prawo o n

#endif

