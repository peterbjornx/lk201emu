//#include "kb.h"

funcdiv_t divs[16];
autobuf_t bufs[4];
bool	  keys[256];
void setup()
{
	int i;
	//Serial.begin(9600);
	for ( i = 0; i < 256; i++ )
		keys[i] = false;
	periph_init();
	ps2_init();
	comm_init();
	funcdiv_defaults();
}

void loop()
{
	static unsigned long lc = 0;
	unsigned long dc,cc;
	comm_loop();
	cc = millis();
	dc = lc - cc;
	if ( dc > 8 ) {
		lc = cc;
		scan();
	}
}

int arcode=-1;
uint8_t dur[10];
int durp=0;
void scan()
{
	static int artimer = 0;
	static int lastar = -1;
	unsigned long nows = millis();
	static unsigned long arstart = nows + 99999999;
	unsigned long arn;
	static int nextar;
	int i = 0;
	bool s;
	int fd, arr= 0, ks=0,dua=0;
	durp=0;
	/* Scan keys */
	for ( i = 0; i < 256; i++ ) {
		fd = map_funcdiv( i );
		s = vt220_scan( i );
		if ( s && !keys[i] ) {
			periph_click();
			//Down
			///Serial.print(i);
			//Serial.print("d");
		//	Serial.println(s);
			if ( divs[fd].mode == FDM_AUTO ) {
				arcode = i;
				arstart = nows + bufs[divs[fd].buffer].timeout;
			} else {
				ks = 1;
			}
			if ( divs[fd].mode == FDM_DOWNUP )
				dua = 1;
			comm_write( i );
			keys[i] = true;
		} else if ( s && keys[i] ) {
			if ( divs[fd].mode == FDM_DOWNUP )
				dua = 1;
		} else if ( keys[i] && !s ) {
		//	Serial.print(i);
		//	Serial.print("u");
		//	Serial.println(s);
			if ( arcode == i ) {
				arr = 1;
				arcode = -1;
			}
			if ( divs[fd].mode == FDM_DOWNUP ) {
				/* Check for all ups */
				dur[durp++] = i;
			}
			keys[i] = false;
		}
	}
	
	/* If downup released and all ups */
	if ( durp && !dua) {
		comm_write( SCAN_ALL_UPS );
		ks = 1;
	} else if ( durp ) {
		for ( i = 0; i < durp; i++)
			comm_write(dur[i]);
		ks = 1;
	}
		
	if ( arr ) {
		for ( i = 0; i < 256; i++ ) {
			fd = map_funcdiv( i );
			if (  (divs[fd].mode == FDM_AUTO) && keys[i] ) {
				arcode = i;
				arstart = nows + bufs[divs[fd].buffer].timeout;
			}
		}
	}
	
	if ( nows > arstart ) {
		if ( arcode == -1 )
			return;
			
		fd = map_funcdiv(arcode);
		arn = nows - arstart;
		arr = (arn / (unsigned long) bufs[divs[fd].buffer].interval);
		if ( nextar != arr ) {
		//	Serial.print("Repeat for ");
		//	Serial.println(arcode);
			nextar = arr;
			if ( ks || ! arr ) 
				comm_write( arcode );
			else
				comm_write( SCAN_METRONOME );
		} 
	}
	
}

void funcdiv_defaults( void )
{
	divs[FD_MAIN].mode	= FDM_AUTO;
	divs[FD_MAIN].buffer	= 0;
	divs[FD_NUMPAD].mode	= FDM_AUTO;
	divs[FD_NUMPAD].buffer	= 0;
	divs[FD_DELETE].mode	= FDM_AUTO;
	divs[FD_DELETE].buffer	= 1;
	divs[FD_HCURSOR].mode	= FDM_AUTO;
	divs[FD_HCURSOR].buffer	= 1;
	divs[FD_VCURSOR].mode	= FDM_AUTO;
	divs[FD_VCURSOR].buffer	= 1;
	divs[FD_RETURN].mode	= FDM_DOWN;
	divs[FD_LOCK].mode	= FDM_DOWN;
	divs[FD_SHIFT].mode	= FDM_DOWNUP;
	divs[FD_EDITING].mode	= FDM_DOWNUP;
	divs[FD_FA].mode	= FDM_DOWN;//Guessed
	divs[FD_FB].mode	= FDM_DOWN;//Guessed
	divs[FD_FC].mode	= FDM_DOWN;//Guessed
	divs[FD_FD].mode	= FDM_DOWN;//Guessed
	divs[FD_FE].mode	= FDM_DOWN;//Guessed
	bufs[0].timeout = 500;
	bufs[1].timeout = 300;
	bufs[2].timeout = 500;
	bufs[3].timeout = 300;
	bufs[0].interval = 1000/30;
	bufs[1].interval = 1000/30;
	bufs[2].interval = 1000/40;
	bufs[3].interval = 1000/40;
}

void funcdiv_downonly( void )
{
	int i;
	for ( i = 0; i < 16; i++ ) {
		if ( divs[i].mode == FDM_AUTO )
			divs[i].mode = FDM_DOWN;
	}
}

void autobuf_set( int buf, int to, int iv )
{
	bufs[buf].timeout = (int) (8.3 * to);
	bufs[buf].interval = (int) (1000 / iv);
}

void funcdiv_set( int fd, int m, int b )
{
	divs[fd].mode = m; 
	if ( ( b != 4 ) && ( m == FDM_AUTO ) )
		divs[fd].buffer = b;
}
