int map_funcdiv( uint8_t s )
{
	switch ( s )
	{
		case 0x92:
		case 0x94:
		case 0x95:
		case 0x96:
		case 0x97:
		case 0x98:
		case 0x99:
		case 0x9A:
		case 0x9B:
		case 0x9C:
		case 0x9D:
		case 0x9E:
		case 0x9F:
		case 0xA1:
		case 0xA2:
		case 0xA3:
		case 0xA4:
			return 2;
		case 0xBC:
			return 3;
		case 0xBD:
		case 0xBE:
			return 4;
		case 0xB0:
		case 0xB1:
			return 5;
		case 0xAF:
		case 0xAE:
			return 6;
		case 0xA7:
		case 0xA8:
			return 7;
		case 0xA9:
		case 0xAA:
			return 8;
		case 0x8A:
		case 0x8B:
		case 0x8C:
		case 0x8D:
		case 0x8E:
		case 0x8F:
			return 9;
		case 0x56:
		case 0x57:
		case 0x58:
		case 0x59:
		case 0x5A:
			return 10;
		case 0x64:
		case 0x65:
		case 0x66:
		case 0x67:
		case 0x68:
			return 11;
		case 0x71:
		case 0x72:
		case 0x73:
		case 0x74:
			return 12;
		case 0x7C:
		case 0x7D:
			return 13;
		case 0x80:
		case 0x81:
		case 0x82:
		case 0x83:
			return 14;
		default:
			return 1;
	}
}
