/**

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
//#include "kb.h"

#define BREAK     0x01
#define MODIFIER  0x02
#define MODIFIER2  0x04

const uint8_t ps2_xlat[128+16] = {
	/* 0 ,    1,    2,    3,    4,    5,    6,    7,    8,    9,    A,    B,    C,    D,    E,    F       */
	    0, 0x68,    0, 0x64, 0x59, 0x57, 0x58, 0x73,    0, 0x71, 0x67, 0x65, 0x5A, 0xBE, 0xBF,    0, /* 0 */
	    0, 0xB1, 0xAE,    0, 0xAF, 0xC1, 0xC0,    0,    0,    0, 0xC3, 0xC7, 0xC2, 0xC6, 0xC5,    0, /* 1 */
	    0, 0xCE, 0xC8, 0xCD, 0xCC, 0xD0, 0xCB,    0,    0, 0xD4, 0xD3, 0xD2, 0xD7, 0xD1, 0xD6,    0, /* 2 */
	    0, 0xDE, 0xD9, 0xDD, 0xD8, 0xDC, 0xDB,    0,    0,    0, 0xE3, 0xE2, 0xE1, 0xE0, 0xE5,    0, /* 3 */
	    0, 0xE8, 0xE7, 0xE6, 0xEB, 0xEF, 0xEA,    0,    0, 0xED, 0xF3, 0xEC, 0xF2, 0xF0, 0xF9,    0, /* 4 */
	    0,    0, 0xFB,    0, 0xFA, 0xF5,    0,    0, 0xB0, 0xAE, 0xBD, 0xF6,    0, 0xF7,    0,    0, /* 5 */
	    0,    0,    0,    0,    0,    0, 0xBC,    0,    0, 0x96,    0, 0x99, 0x9d,    0,    0,    0, /* 6 */
         0x92, 0x94, 0x97, 0x9A, 0x9B, 0x9e, 0x56,    0, 0x72, 0x9c, 0x98, 0xa0, 0xa3, 0x9f, 0x7c,    0, /* 7 */
            0,    0,    0, 0x66,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, /* 8 */
            };		   

volatile bool vt220_keystate[256];

bool vt220_scan( uint8_t c )
{
	return vt220_keystate[c];
}

void ps2_dataproc();

// The ISR for the external interrupt
void ps2_isr( void )
{
	static uint8_t bitcount=0;
	static uint16_t incoming=0;
	static uint32_t prev_ms=0;
	static uint8_t pc = 0;
	uint32_t now_ms;
	uint8_t n, val;

	val = digitalRead(7);
	now_ms = millis();
	if (now_ms - prev_ms > 250) {
		bitcount = 0;
		incoming = 0;
	}
	prev_ms = now_ms;
	n = bitcount;
	incoming |= (val << n);
	if ( (n >= 1 ) && ( n <= 9 ) && val )
		pc++; 
	// Sa 0 1 2 3 4 5 6 7 P St
	bitcount++;
	if (bitcount == 11) {
		if ( (1&~incoming) /*&& (0x200&incoming)*/) {
			if ( 1 & pc )
			ps2_dataproc( ( incoming >> 1 ) & 0xFF );
		}
		pc = 0;
		bitcount = 0;
		incoming = 0;
	}
}

void ps2_dataproc( int s )
{
	static uint8_t mods = 0;
	uint8_t c;
	int e = 0;
	if (s == 0xF0) {
		mods |= BREAK;
		return;
	} else if (s == 0xE0) {
		mods |= MODIFIER;
		return;
	} else if (s == 0xE1) {
		mods |= MODIFIER2;
		return;
	}
	
	if (mods & BREAK) {
		e = 1;
	}
	
	c = 0;
	if (mods & MODIFIER) {
		switch (s) {
			case 0x70: c = 0x8a;	break;
			case 0x6C: c = 0x8b;	break;
			case 0x7D: c = 0x8c;	break;
			case 0x71: c = 0x8d;	break;
			case 0x69: c = 0x8e;	break;
			case 0x7A: c = 0x8f;	break;
			case 0x75: c = 0xAA;	break;
			case 0x1f: c = 0xc9;	break;
			case 0x7C: c = 0x74;	break;
			case 0x12: c = 0x74;	break;
			case 0x6B: c = 0xA7;	break;
			case 0x72: c = 0xa9;	break;
			case 0x74: c = 0xa8;	break;
			case 0x4A: c = 0xA2;	break;
			case 0x5A: c = 0x95;	break;
			default:		break;
		}
	} else if (mods & MODIFIER2) {
		switch (s) {
			case 0x14: c = 0x7d;	break;
			default:		break;
		}
	} else if (s < 256) {
			c = ps2_xlat[s];
	}
	
	mods = 0;
	
	if ( !c )
		return;
	
	vt220_keystate[c] = !e;
}

void ps2_init() {
  int i;
  for (  i = 0; i < 256; i++ )
  	vt220_keystate[i] = false;

  // initialize the pins
#ifdef INPUT_PULLUP
  pinMode(3, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
#else
  pinMode(3, INPUT);
  digitalWrite(3, HIGH);
  pinMode(7, INPUT);
  digitalWrite(7, HIGH);
#endif
  attachInterrupt(1, ps2_isr, FALLING);
}


