//#include "kb.h"

int keyclick = 0x5;
int beepvol = 0x5;
uint8_t leds = 0;

void periph_init( void )
{
	pinMode(6,OUTPUT);				/* Buzzer */
	pinMode(9,OUTPUT);				/* LED0   */
	pinMode(10,OUTPUT);				/* LED1   */
	pinMode(11,OUTPUT);				/* LED2   */
	pinMode(12,OUTPUT);				/* LED3   */
}

void periph_click( void )
{
	if ( keyclick == 0x7 )
		return;
	tone(6,12000,8);
}

void periph_beep( void )
{
	if ( beepvol == 0x7 )
		return;
	tone(6,440,400);
}

void periph_pushleds( void )
{
	int i;
	for ( i = 0; i < 4; i++ )
		digitalWrite( 9+i, leds & ( 1 << i ) );
}

void periph_ledon( uint8_t l )
{
        leds |=   l & 0xF;
        periph_pushleds();
}

void periph_ledoff( uint8_t l )
{
        leds &= ~(l & 0xF);
        periph_pushleds();
}

void periph_setclickvol( int v )
{
	keyclick = v;
}

void periph_setbeepvol( int v )
{
	beepvol = v;
}


