#define SCAN_ALL_UPS     (0xB3)
#define SCAN_METRONOME   (0xB4)
#define SCAN_OUTPUT_ERR  (0xB5)
#define SCAN_INPUT_ERR   (0xB6)
#define SCAN_KBDLOCK_ACK (0xB7)
#define SCAN_TEST_MODE   (0xB8)
#define SCAN_PREFIX      (0xB9)
#define SCAN_CHMODE_ACK  (0xBA)
#define SCAN_RESERVED    (0x7F)

#define SCAN_KBD_FWID     (0x01)
#define SCAN_KBD_HWID     (0x00)
#define SCAN_KBD_KDPO_ERR (0x3D)
#define SCAN_KBD_POST_ERR (0x3E)

#define FD_MAIN		(1)
#define	FD_NUMPAD	(2)
#define	FD_DELETE	(3)
#define	FD_RETURN	(4)
#define	FD_LOCK		(5)
#define	FD_SHIFT	(6)
#define	FD_HCURSOR	(7)
#define	FD_VCURSOR	(8)
#define	FD_EDITING	(9)
#define	FD_FA		(10)
#define	FD_FB		(11)
#define	FD_FC		(12)
#define	FD_FD		(13)
#define	FD_FE		(14)

#define FDM_DOWN	(0)
#define FDM_AUTO	(1)
#define FDM_DOWNUP	(3)

typedef struct {
	int mode;
	int buffer;
} funcdiv_t;

typedef struct {
	int timeout;
	int interval;
	int tdown;
} autobuf_t;

extern int inhibit;

void funcdiv_defaults( void );
void funcdiv_downonly( void );
void autobuf_set( int buf, int to, int iv );
void funcdiv_set( int fd, int m, int b );

void periph_init( void );
void periph_click( void );
void periph_beep( void );
void periph_ledon( uint8_t l );
void periph_ledoff( uint8_t l );
void periph_setclickvol( int v );
void periph_setbeepvol( int v );

void comm_write( uint8_t c );
void comm_iwrite( uint8_t c );
void comm_post( void );
void comm_init( void );
void comm_loop( void );
void comm_kbdid( void );

void ps2_init();

bool vt220_scan( uint8_t c );

void cmd_handle( int r_cmd, int npar, uint8_t *params );

int map_funcdiv( uint8_t s );
