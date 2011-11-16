Attribute VB_Name = "IPSerial"
'********************************************************
'    IPSerial.bas   ver 1.00
'     -- IPSerial module for Visual Basic(5.0 above)
'
'    Description:
'       When you want to develop one VB application with IPSerial,
'       you should add this module to your project.
'
'    History:
'           Date    Author      Comment
'            3/17/03  Shinhay     wrote it.
'
'********************************************************

'Baud Rate Setting

Global Const B50 = 0
Global Const B75 = 1
Global Const B110 = 2
Global Const B134 = 3
Global Const B150 = 4
Global Const B300 = 5
Global Const B600 = 6
Global Const B1200 = 7
Global Const B2400 = 9
Global Const B4800 = 10
Global Const B7200 = 11
Global Const B9600 = 12
Global Const B19200 = 13
Global Const B38400 = 14
Global Const B57600 = 15
Global Const B115200 = 16
Global Const B230400 = 17
Global Const B460800 = 18
Global Const B921600 = 19

' Mode setting
Global Const BIT_5 = 0                              ' Data bits define
Global Const BIT_6 = 1
Global Const BIT_7 = 2
Global Const BIT_8 = 3

Global Const STOP_1 = 0                    ' Stop bits define
Global Const STOP_2 = 4

Global Const P_IP_SERIAL_NONE = 0
Global Const P_IP_SERIAL_EVEN = 8                   ' Parity define
Global Const P_IP_SERIAL_ODD = 16
Global Const P_IP_SERIAL_MARK = 24
Global Const P_IP_SERIAL_SPACE = 32

Global Const F_NONE = &H0                                               ' Flow Control : None
Global Const F_CTS = &H1                                                ' Flow Control : CTS
Global Const F_RTS = &H2                                                ' Flow Control : RTS
Global Const F_XON = &H4                                                ' Flow Control : XON
Global Const F_XOFF = &H8                                               ' Flow Control : XOFF
Global Const F_RTS_CTS = F_RTS Or F_CTS
Global Const F_XON_XOFF = F_XON Or F_XOFF
Global Const F_BOTH = F_RTS Or F_CTS Or F_XON Or F_XOFF


        'linectrl command
Global Const LCTRL_DTR = 1                      ' set DTR on
Global Const LCTRL_RTS = 2                      ' set RTS on

        ' FLUSH command
Global Const FLUSH_RXBUFFER = 0                 ' flush Rx buffer
Global Const FLUSH_TXBUFFER = 1                 ' flush Tx buffer
Global Const FLUSH_ALLBUFFER = 2                ' flush Rx & Tx buffer

        ' LSTATUS command }
Global Const S_CTS = 1                          ' line status : CTS on
Global Const S_DSR = 2                          ' line status : DSR on
Global Const S_DCD = 8                          ' line status : DCD on


' Error code
Global Const NSIO_OK = 0

Global Const NSIO_BADPORT = -1                  ' no such port or port not opened
Global Const NSIO_BADPARM = -2                  ' bad parameter
Global Const NSIO_THREAD_ERR = -3
Global Const NSIO_MEMALLOCERR = -4              ' memory allocate error

Global Const NSIO_INVALID_PASSWORD = -100       'Invalid console password
Global Const NSIO_RESET_TIMEOUT = -101          ' Reset port timeout(fail).
Global Const NSIO_NOT_ALIVE = -102              ' This com port is not alive.

Global Const NSIO_CONNECT_FAIL = -200           ' Connect to Server fail.
Global Const NSIO_SOCK_INIT_FAIL = -201         ' socket initial error
Global Const NSIO_SOCK_ERR = -202               ' socket error
Global Const NSIO_TIMEOUT = -203

 

' Server Control
Declare Function nsio_init Lib "IPSerial.dll" () As Long
Declare Function nsio_end Lib "IPSerial.dll" () As Long
Declare Function nsio_resetserver Lib "IPSerial.dll" (ByVal server_ip As String, ByVal password As String) As Long
Declare Function nsio_checkalive Lib "IPSerial.dll" (ByVal server_ip As String, ByVal timeout As Long) As Long

' Port Control
Declare Function nsio_open Lib "IPSerial.dll" (ByVal server_ip As String, ByVal port_index As Long, ByVal timeouts As Long) As Long
Declare Function nsio_close Lib "IPSerial.dll" (ByVal port_id As Long) As Long

Declare Function nsio_ioctl Lib "IPSerial.dll" (ByVal port_id As Long, ByVal baud As Long, ByVal mode As Long) As Long
Declare Function nsio_flowctrl Lib "IPSerial.dll" (ByVal port_id As Long, ByVal mode As Long) As Long
Declare Function nsio_flush Lib "IPSerial.dll" (ByVal port_id As Long, ByVal func As Long) As Long
Declare Function nsio_xonxoff_chars Lib "IPSerial.dll" (ByVal port_id As Long, ByVal xon_char As Byte, ByVal xoff_char As Byte) As Long
Declare Function nsio_DTR Lib "IPSerial.dll" (ByVal port_id As Long, ByVal mode As Long) As Long
Declare Function nsio_RTS Lib "IPSerial.dll" (ByVal port_id As Long, ByVal mode As Long) As Long
Declare Function nsio_lctrl Lib "IPSerial.dll" (ByVal port_id As Long, ByVal mode As Long) As Long
Declare Function nsio_set_xoff Lib "IPSerial.dll" (ByVal port_id As Long) As Long
Declare Function nsio_set_xon Lib "IPSerial.dll" (ByVal port_id As Long) As Long
Declare Function nsio_baud Lib "IPSerial.dll" (ByVal port_id As Long, ByVal speed As Long) As Long
Declare Function nsio_resetport Lib "IPSerial.dll" (ByVal port_id As Long, ByVal password As String) As Long

' Input/Output data
Declare Function nsio_read Lib "IPSerial.dll" (ByVal port_id As Long, ByRef buf As Byte, ByVal buf_len As Long) As Long
Declare Function nsio_SetReadTimeouts Lib "IPSerial.dll" (ByVal port_id As Long, ByVal timeouts As Long) As Long
Declare Function nsio_write Lib "IPSerial.dll" (ByVal port_id As Long, ByRef buf As Byte, ByVal buf_len As Long) As Long
Declare Function nsio_SetWriteTimeouts Lib "IPSerial.dll" (ByVal port_id As Long, ByVal timeouts As Long) As Long

' Port Status Inquiry
Declare Function nsio_lstatus Lib "IPSerial.dll" (ByVal port_id As Long) As Long
Declare Function nsio_iqueue Lib "IPSerial.dll" (ByVal port_id As Long) As Long
Declare Function nsio_oqueue Lib "IPSerial.dll" (ByVal port_id As Long) As Long
Declare Function nsio_data_status Lib "IPSerial.dll" (ByVal port_id As Long) As Long


' Miscellaneous
Declare Function nsio_break Lib "IPSerial.dll" (ByVal port_id As Long, ByVal time As Long) As Long
Declare Function nsio_break_on Lib "IPSerial.dll" (ByVal port_id As Long) As Long
Declare Function nsio_break_off Lib "IPSerial.dll" (ByVal port_id As Long) As Long
Declare Function nsio_breakcount Lib "IPSerial.dll" (ByVal port_id As Long) As Long


