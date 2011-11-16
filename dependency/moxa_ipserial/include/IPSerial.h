#ifndef _IPSERIAL_H
#define	_IPSERIAL_H

#ifndef WINAPI
#define WINAPI	  
#endif

#ifndef DWORD
typedef unsigned long DWORD;
#endif

#ifndef B50

/*----------------------------------------------------------------------*/
/*	Parameters for nsio_ioctl function 									*/	
/*----------------------------------------------------------------------*/
#define B50					0	/* IOCTL : baud rate = 50			*/
#define B75					1	/* IOCTL : baud rate = 75			*/
#define B110				2	/* IOCTL : baud rate = 110			*/
#define B134				3	/* IOCTL : baud rate = 134.5		*/
#define B150				4	/* IOCTL : baud rate = 150			*/
#define B300				5	/* IOCTL : baud rate = 300 bps		*/
#define B600				6	/* IOCTL : baud rate = 600 bps		*/
#define B1200				7	/* IOCTL : baud rate = 1200 bps		*/
#define B2400				9	/* IOCTL : baud rate = 2400 bps		*/
#define B4800				10	/* IOCTL : baud rate = 4800 bps		*/
#define B7200				11	/* IOCTL : baud rate = 7200 bps		*/
#define B9600				12	/* IOCTL : baud rate = 9600 bps		*/
#define B19200				13	/* IOCTL : baud rate = 19200 bps	*/
#define B38400				14	/* IOCTL : baud rate = 38400 bps	*/
#define B57600				15	/* IOCTL : baud rate = 57600 bps	*/
#define B115200 			16	/* IOCTL : baud rate = 115200 bps	*/
#define B230400 			17	/* IOCTL : baud rate = 230400 bps	*/
#define B460800				18	/* IOCTL : baud rate = 460800 bps	*/
#define B921600				19	/* IOCTL : baud rate = 921600 bps	*/

#define BIT_8				3	/* IOCTL : 8 data bits */
#define BIT_7				2	/* IOCTL : 7 data bits */
#define BIT_6				1	/* IOCTL : 6 data bits */
#define BIT_5				0	/* IOCTL : 5 data bits */

#define STOP_1				0	/* IOCTL : 1 stop bit  */
#define STOP_2				4	/* IOCTL : 2/1.5 stop bits */

/*----------------------------------------------------------------------*/
/*	return value for lstatus function									*/	
/*----------------------------------------------------------------------*/
#define S_CTS				0x01	/* line status : CTS on 	*/
#define S_DSR				0x02	/* line status : DSR on 	*/
#define S_DCD				0x08	/* line status : DCD on 	*/
#endif

/* new define. Prevent define conflict with pcomm */
#define P_IP_SERIAL_NONE				0	/* IOCTL : none parity:0x0	*/
#define P_IP_SERIAL_EVEN				8	/* IOCTL : even parity:0x08	*/
#define P_IP_SERIAL_ODD				        16	/* IOCTL : odd parity: 0x10	*/
#define P_IP_SERIAL_MARK				24	/* IOCTL : mark parity: 0x18	*/
#define P_IP_SERIAL_SPACE				32	/* IOCTL : space parity: 0x20 */

#ifndef P_NONE

/* If PComm.h is not include, this is for old compatible */
#define P_NONE				P_IP_SERIAL_NONE	/* IOCTL : none parity	*/
#define P_EVEN				P_IP_SERIAL_EVEN	/* IOCTL : even parity	*/
#define P_ODD				P_IP_SERIAL_ODD	        /* IOCTL : odd parity	*/
#define P_MARK				P_IP_SERIAL_MARK	/* IOCTL : mark parity	*/
#define P_SPACE				P_IP_SERIAL_SPACE	/* IOCTL : space parity */

#else

#define P_PCOMM_NONE	0x00
#define P_PCOMM_EVEN	0x18
#define P_PCOMM_ODD	0x08
#define P_PCOMM_MARK	0x28
#define P_PCOMM_SPACE	0x38

#endif


/*----------------------------------------------------------------------*/
/*	Parameters for nsio_flowctrl function								*/	
/*----------------------------------------------------------------------*/
#define F_NONE				0x00	/* Flow Control : None 	*/
#define F_CTS				0x01	/* Flow Control : CTS 	*/
#define F_RTS				0x02	/* Flow Control : RTS 	*/
#define F_XON				0x04	/* Flow Control : XON  	*/
#define F_XOFF				0x08	/* Flow Control : XOFF 	*/
#define F_RTS_CTS			F_RTS | F_CTS
#define F_XON_XOFF			F_XON | F_XOFF
#define F_BOTH				F_RTS | F_CTS | F_XON | F_XOFF

/*----------------------------------------------------------------------*/
/*	Parameters for nsio_lctrl function									*/	
/*----------------------------------------------------------------------*/
#define LCTRL_DTR			0x01	/* set DTR on		*/
#define LCTRL_RTS			0x02	/* set RTS on		*/

/*----------------------------------------------------------------------*/
/*	Parameters for D_COMMAND_FLUSH command								*/	
/*----------------------------------------------------------------------*/
#define FLUSH_RXBUFFER		0x00	/* flush Rx buffer		*/
#define FLUSH_TXBUFFER		0x01	/* flush Tx buffer		*/
#define FLUSH_ALLBUFFER		0x02	/* flush Rx & Tx buffer */




/*----------------------------------------------------------------------*/
/*	error code	 														*/	
/*----------------------------------------------------------------------*/
#define NSIO_OK					0

#define NSIO_BADPORT			-1	/* no such port or port not opened */
#define NSIO_BADPARM			-2	/* bad parameter */
#define NSIO_THREAD_ERR			-3
#define NSIO_MEMALLOCERR		-4	/* memory allocate error */

#define NSIO_INVALID_PASSWORD	-100	/* Invalid console password */
#define NSIO_RESET_TIMEOUT		-101	/* Reset port timeout(fail). */
#define NSIO_NOT_ALIVE			-102  /* This com port is not alive. */

#define NSIO_CONNECT_FAIL		-200  /* Connect to Server fail. */
#define NSIO_SOCK_INIT_FAIL		-201	/* socket initial error */
#define NSIO_SOCK_ERR			-202	/* socket error */
#define NSIO_TIMEOUT			-203	/* Timeout */




#ifdef __cplusplus
extern "C" {
#endif

// Server Control
int WINAPI nsio_init();
int WINAPI nsio_end();
int WINAPI nsio_resetserver(char *server_ip, char *password);
int WINAPI nsio_checkalive(char *server_ip, DWORD timeout);

// Port Control
int WINAPI nsio_open(char *server_ip, int port_index, DWORD timeouts);
int WINAPI nsio_close(int port_id);

int WINAPI nsio_ioctl(int port_id, int baud, int mode);
int WINAPI nsio_flowctrl(int port_id, int mode);
int WINAPI nsio_flush(int port_id, int func);
int WINAPI nsio_xonxoff_chars(int port_id, unsigned char xon_char, unsigned char xoff_char);
int WINAPI nsio_DTR(int port_id, int mode);
int WINAPI nsio_RTS(int port_id, int mode);
int WINAPI nsio_lctrl(int port_id, int mode);
int WINAPI nsio_set_xoff(int port_id);
int WINAPI nsio_set_xon(int port_id);
int WINAPI nsio_baud(int port_id, long speed);
int WINAPI nsio_resetport(int port_id, char *password);

// Input/Output data
int WINAPI nsio_read(int port_id, char *buf, int len);
int WINAPI nsio_SetReadTimeouts(int port_id, DWORD timeouts);
int WINAPI nsio_write(int port_id, char *buf, int len);
int WINAPI nsio_SetWriteTimeouts(int port_id, DWORD timeouts);

// Port Status Inquiry
int WINAPI nsio_lstatus(int port_id);
long WINAPI nsio_iqueue(int port_id);
long WINAPI nsio_oqueue(int port_id);
int WINAPI nsio_data_status(int port_id);


// Miscellaneous
int WINAPI nsio_break(int port_id, int time);
int WINAPI nsio_break_on(int port_id);
int WINAPI nsio_break_off(int port_id);
int WINAPI nsio_breakcount(int port_id);

#ifdef __cplusplus
}
#endif



#endif
