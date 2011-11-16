(*********************************************************************
    IPSerial.pas
     -- IPSerial Lib unit for Delphi (32 bit version).


    History:   Date       Author         Comment
               3/17/02    Shinhay        Wrote it.

**********************************************************************)

unit IPSerial;

interface

const
  	{ baud rate setting }
  	B50 	= 0;
  	B75 	= 1;
  	B110 	= 2;
  	B134	= 3;
  	B150 	= 4;
  	B300 	= 5;
  	B600 	= 6;
  	B1200 	= 7;
	B2400 	= 9;
  	B4800 	= 10;
  	B7200 	= 11;
  	B9600 	= 12;
  	B19200 	= 13;
  	B38400 	= 14;
  	B57600 	= 15;
  	B115200 = 16;
  	B230400 = 17;
  	B460800 = 18;
  	B921600 = 19;  


  	{ data bit }
  	BIT_5 	= 0;
  	BIT_6 	= 1;
  	BIT_7 	= 2;
  	BIT_8 	= 3;

  	{ stop bit }
  	STOP_1 	= 0;
  	STOP_2 	= 4;

  	{ parity }
  	P_IP_SERIAL_NONE 	= 0;
  	P_IP_SERIAL_EVEN 	= 8;
  	P_IP_SERIAL_ODD  	= 16;
  	P_IP_SERIAL_MARK 	= 24;
  	P_IP_SERIAL_SPACE	= 32;

	F_NONE = $00;	{ Flow Control : None 	}
	F_CTS = $01;	{ Flow Control : CTS 	}
	F_RTS = $02;	{ Flow Control : RTS 	}
	F_XON = $04;	{ Flow Control : XON  	}
	F_XOFF = $08;	{ Flow Control : XOFF 	}
	F_RTS_CTS = F_RTS or F_CTS;
	F_XON_XOFF = F_XON or F_XOFF;
	F_BOTH = F_RTS or F_CTS or F_XON or F_XOFF;

	{linectrl command }
	LCTRL_DTR	   = 	1;			{ set DTR on		}
	LCTRL_RTS	   =	2;			{ set RTS on		}

	{ FLUSH command	}
	FLUSH_RXBUFFER	   =	0;			{ flush Rx buffer	}
	FLUSH_TXBUFFER	   =	1;			{ flush Tx buffer	}
	FLUSH_ALLBUFFER	   =	2;			{ flush Rx & Tx buffer }

	{ LSTATUS command }
	S_CTS	   =	1;			{ line status : CTS on 	}
	S_DSR	   =	2;			{ line status : DSR on 	}
	S_DCD	   =	8;			{ line status : DCD on 	}


  	{ error code }
	NSIO_OK				=	0;
	
	NSIO_BADPORT			=	-1;		{ no such port or port not opened }
	NSIO_BADPARM			=	-2;		{ bad parameter }
	NSIO_THREAD_ERR			=	-3;
	NSIO_MEMALLOCERR		=	-4;		{ memory allocate error }
	
	NSIO_INVALID_PASSWORD	=	-100;	{ Invalid console password }
	NSIO_RESET_TIMEOUT		=	-101; 	{ Reset port timeout(fail). }
	NSIO_NOT_ALIVE			=	-102; 	{ This com port is not alive. }

	NSIO_CONNECT_FAIL		=	-200; 	{ Connect to Server fail. }
	NSIO_SOCK_INIT_FAIL		=	-201;	{ socket initial error }
	NSIO_SOCK_ERR			=	-202;	{ socket error }
	NSIO_TIMEOUT			=	-203;
  	


{Import routine from IPSerial.dll}
{ Server Control }
function nsio_init(): Longint; stdcall;
function nsio_end(): Longint; stdcall;
function nsio_resetserver(server_ip: PChar; password: PChar): Longint; stdcall;
function nsio_checkalive(server_ip: PChar; timeout: Longint): Longint; stdcall;

{ Port Control }
function nsio_open(server_ip: PChar; port_index: Longint; timeouts: Longint): Longint; stdcall;
function nsio_close(port_id: Longint): Longint; stdcall;

function nsio_ioctl(port_id: Longint; baud: Longint; mode: Longint): Longint; stdcall;
function nsio_flowctrl(port_id: Longint; mode: Longint): Longint; stdcall;
function nsio_flush(port_id: Longint; func: Longint): Longint; stdcall;
function nsio_xonxoff_chars(port_id: Longint; xon_char: Byte; xoff_char: Byte): Longint; stdcall;
function nsio_DTR(port_id: Longint; mode: Longint): Longint; stdcall;
function nsio_RTS(port_id: Longint; mode: Longint): Longint; stdcall;
function nsio_lctrl(port_id: Longint; mode: Longint): Longint; stdcall;
function nsio_set_xoff(port_id: Longint): Longint; stdcall;
function nsio_set_xon(port_id: Longint): Longint; stdcall;
function nsio_baud(port_id: Longint; speed: Longint): Longint; stdcall;
function nsio_resetport(port_id: Longint; password: PChar): Longint; stdcall;

{ Input/Output data }
function nsio_read(port_id: Longint; buf: PChar; len: Longint): Longint; stdcall;
function nsio_SetReadTimeouts(port_id: Longint; timeouts: Longint): Longint; stdcall;
function nsio_write(port_id: Longint; buf: PChar; len: Longint): Longint; stdcall;
function nsio_SetWriteTimeouts(port_id: Longint; timeouts: Longint): Longint; stdcall;

{ Port Status Inquiry }
function nsio_lstatus(port_id: Longint): Longint; stdcall;
function nsio_iqueue(port_id: Longint): Longint; stdcall;
function nsio_oqueue(port_id: Longint): Longint; stdcall;
function nsio_data_status(port_id: Longint): Longint; stdcall;


{ Miscellaneous }
function nsio_break(port_id, time: Longint): Longint; stdcall;
function nsio_break_on(port_id: Longint): Longint; stdcall;
function nsio_break_off(port_id: Longint): Longint; stdcall;
function nsio_breakcount(port_id: Longint): Longint; stdcall;




implementation
{ Server Control }
function nsio_init; external 'IPSerial.dll';
function nsio_end; external 'IPSerial.dll';
function nsio_resetserver; external 'IPSerial.dll';
function nsio_checkalive; external 'IPSerial.dll';

{ Port Control }
function nsio_open; external 'IPSerial.dll';
function nsio_close; external 'IPSerial.dll';

function nsio_ioctl; external 'IPSerial.dll';
function nsio_flowctrl; external 'IPSerial.dll';
function nsio_flush; external 'IPSerial.dll';
function nsio_xonxoff_chars; external 'IPSerial.dll';
function nsio_DTR; external 'IPSerial.dll';
function nsio_RTS; external 'IPSerial.dll';
function nsio_lctrl; external 'IPSerial.dll';
function nsio_set_xoff; external 'IPSerial.dll';
function nsio_set_xon; external 'IPSerial.dll';
function nsio_baud; external 'IPSerial.dll';
function nsio_resetport; external 'IPSerial.dll';

{ Input/Output data }
function nsio_read; external 'IPSerial.dll';
function nsio_SetReadTimeouts; external 'IPSerial.dll';
function nsio_write; external 'IPSerial.dll';
function nsio_SetWriteTimeouts; external 'IPSerial.dll';

{ Port Status Inquiry }
function nsio_lstatus; external 'IPSerial.dll';
function nsio_iqueue; external 'IPSerial.dll';
function nsio_oqueue; external 'IPSerial.dll';
function nsio_data_status; external 'IPSerial.dll';

{ Miscellaneous }
function nsio_break; external 'IPSerial.dll';
function nsio_break_on; external 'IPSerial.dll';
function nsio_break_off; external 'IPSerial.dll';
function nsio_breakcount; external 'IPSerial.dll';

end.
