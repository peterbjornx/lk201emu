
int ctrlclick = 0;
int autoinhibit = 0;

void cmd_mode( int r_cmd, int npar, uint8_t *param )
{
	int div = (r_cmd & 0x78) >> 3;
	int m = (r_cmd & 0x6) >> 1;
	int b = 4;
	if ( npar == 1 )
		b = param[0] & 0x3;
	if ( div == 0xF ) {
		//autobuf_set( m, param[0] & 0x7F, param[1] & 0x7F );
		return;
	} else if ( div != 0 ) {
		//funcdiv_set( div, m&0x3, b );
		return;
	}
}

void cmd_handle( int r_cmd, int npar, uint8_t *param )
{
  //Serial.print("Command: ");
  //Serial.println(r_cmd,HEX);
  //Serial.println(param[0],HEX);
  if ( !(r_cmd & 1) ) {
  	cmd_mode( r_cmd, npar, param );
  	return;
  }
    switch( r_cmd ) {
      case 0x11://TURN OFF LED
        periph_ledoff( param[0] );
        break;
      case 0x13://TURN ON LED
        periph_ledon( param[0] );
        break;
      case 0x8B://RESUME TX
        inhibit = 0;
        periph_ledoff(0x2);
        break;
      case 0x89://INHIBIT TX
        inhibit = 1;
        periph_ledon(0x2);
        comm_iwrite(SCAN_KBDLOCK_ACK);
        break;
      case 0x99:
        periph_setclickvol(0x7);
        break;
      case 0x1B:
        periph_setclickvol(param[0] & 0x7F);
        break;
      case 0xB9:
        ctrlclick = 0;
        break;
      case 0xBB:
        ctrlclick = 1;
        break;
      case 0x9F:
        periph_click();
        break;
      case 0xA1:
        periph_setbeepvol(0x7);
        break;
      case 0x23:
        periph_setbeepvol(param[0] & 0x7F);
        break;
      case 0xA7:
        periph_beep();
        break;
      case 0xC1:
        autoinhibit = 1;//TEMPORARY
        break;
      case 0xE3:
        autoinhibit = 0;
        break;
      case 0xE1:
        autoinhibit = 2;//PERMANENT
        break;
      case 0xD9:
        funcdiv_downonly();
        break;
      case 0xAB://KBID
      	comm_kbdid();
        break;
      case 0xFD:
        comm_post();//TODO: Jump to powerup
        break;
      case 0xCB:
       // Serial.println("TODO: Testmode");
        break;
      case 0xD3:
       // Serial.println("Reinstate defaults");
        break;
      default:
       // Serial.print("Unknown ");
       // Serial.println(r_cmd,HEX);
        break;
    }
}
