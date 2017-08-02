#include <SoftwareSerial.h>


uint8_t param[4];
int r_cmd = 0;
int r_params;
int r_state = 0;

int inhibit;
volatile int ibp=0;
volatile int irp=0;
volatile uint8_t inhbuf[80];
volatile int kerr=0;

//SoftwareSerial termse(4,2); // RX, TX

void comm_iwrite( uint8_t c )
{
//	termse.write( c );
	Serial.write( c );
}

void comm_write( uint8_t c )
{
	if ( irp == ibp ) {
		ibp = 0; //Order matters here!
		irp = 0;
	}
	
	if ( ibp > sizeof inhbuf ) {
		kerr |= 1;
	} else {
		inhbuf[ ibp++ ] = c;
	}
}

void comm_kbdid( void )
{
    comm_iwrite((uint8_t)SCAN_KBD_FWID);
    comm_iwrite((uint8_t)SCAN_KBD_HWID);
}

void comm_post( void )
{
    comm_kbdid();
    comm_iwrite((uint8_t)0x00);//No ERROR ( report if key down )
    comm_iwrite((uint8_t)0x00);//No KEY DOWN
}


void comm_init( void )  
{
  pinMode(2, OUTPUT);
  Serial.begin(4800);
  comm_post();
}

void comm_loop( void )
{
  if (Serial.available()) {
    if ( r_state == 0 ) {
      r_cmd = Serial.read();
      if ( r_cmd & 0x80 ) {
        r_state = 0;
        cmd_handle( r_cmd, r_params, param );
      } else 
        r_state++;
    } else {
      ;
      if ( (param[r_state++ - 1] = Serial.read()) & 0x80 ) {
        r_params = r_state;
        r_state = 0;
        cmd_handle( r_cmd, r_params, param );
      }
    }
  }
  
  if ( (ibp > irp) && !inhibit ){
    comm_iwrite(inhbuf[irp++]);
  }
}
