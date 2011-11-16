VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Multi Ports"
   ClientHeight    =   7485
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9645
   LinkTopic       =   "Form1"
   ScaleHeight     =   7485
   ScaleWidth      =   9645
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   500
      Left            =   4560
      Top             =   1680
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Exit"
      Height          =   375
      Left            =   7200
      TabIndex        =   20
      Top             =   6960
      Width           =   1575
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Close"
      Enabled         =   0   'False
      Height          =   375
      Left            =   3960
      TabIndex        =   19
      Top             =   6960
      Width           =   1575
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Open"
      Height          =   375
      Left            =   840
      TabIndex        =   18
      Top             =   6960
      Width           =   1575
   End
   Begin VB.TextBox Text8 
      BackColor       =   &H80000004&
      Enabled         =   0   'False
      Height          =   1935
      Left            =   4920
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   17
      Top             =   4800
      Width           =   4455
   End
   Begin VB.TextBox Text7 
      BackColor       =   &H80000004&
      Enabled         =   0   'False
      Height          =   1935
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   16
      Top             =   4800
      Width           =   4455
   End
   Begin VB.TextBox Text6 
      Enabled         =   0   'False
      Height          =   2055
      Left            =   4920
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   15
      Top             =   2160
      Width           =   4455
   End
   Begin VB.TextBox Text5 
      Enabled         =   0   'False
      Height          =   2055
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   14
      Top             =   2160
      Width           =   4455
   End
   Begin VB.Frame Frame2 
      Caption         =   "Two"
      Height          =   1215
      Left            =   4920
      TabIndex        =   1
      Top             =   120
      Width           =   4455
      Begin VB.TextBox Text4 
         Height          =   285
         Left            =   1680
         TabIndex        =   9
         Text            =   "2"
         Top             =   720
         Width           =   1095
      End
      Begin VB.TextBox Text3 
         Height          =   285
         Left            =   1680
         TabIndex        =   8
         Text            =   "192.168.127.254"
         Top             =   360
         Width           =   2415
      End
      Begin VB.Label Label4 
         Caption         =   "Port Number:"
         Height          =   255
         Left            =   360
         TabIndex        =   5
         Top             =   720
         Width           =   1215
      End
      Begin VB.Label Label3 
         Caption         =   "Server IP:"
         Height          =   255
         Left            =   360
         TabIndex        =   4
         Top             =   360
         Width           =   1215
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "One"
      Height          =   1215
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   4455
      Begin VB.TextBox Text2 
         Height          =   285
         Left            =   1680
         TabIndex        =   7
         Text            =   "1"
         Top             =   720
         Width           =   1095
      End
      Begin VB.TextBox Text1 
         Height          =   285
         Left            =   1680
         TabIndex        =   6
         Text            =   "192.168.127.254"
         Top             =   360
         Width           =   2415
      End
      Begin VB.Label Label2 
         Caption         =   "Port Number:"
         Height          =   255
         Left            =   360
         TabIndex        =   3
         Top             =   720
         Width           =   1335
      End
      Begin VB.Label Label1 
         Caption         =   "Server IP:"
         Height          =   255
         Left            =   360
         TabIndex        =   2
         Top             =   360
         Width           =   1215
      End
   End
   Begin VB.Label Label13 
      Caption         =   "status:"
      Height          =   255
      Left            =   4920
      TabIndex        =   24
      Top             =   1440
      Width           =   615
   End
   Begin VB.Label Label11 
      Caption         =   "status:"
      Height          =   255
      Left            =   120
      TabIndex        =   23
      Top             =   1440
      Width           =   495
   End
   Begin VB.Label Label10 
      Height          =   255
      Left            =   5760
      TabIndex        =   22
      Top             =   1440
      Width           =   3495
   End
   Begin VB.Label Label9 
      Height          =   255
      Left            =   840
      TabIndex        =   21
      Top             =   1440
      Width           =   3615
   End
   Begin VB.Label Label8 
      Caption         =   "Receiver"
      Height          =   255
      Left            =   6360
      TabIndex        =   13
      Top             =   4440
      Width           =   1695
   End
   Begin VB.Label Label7 
      Caption         =   "Receiver"
      Height          =   255
      Left            =   1440
      TabIndex        =   12
      Top             =   4440
      Width           =   1935
   End
   Begin VB.Label Label6 
      Caption         =   "Transmitter"
      Height          =   255
      Left            =   6360
      TabIndex        =   11
      Top             =   1800
      Width           =   1695
   End
   Begin VB.Label Label5 
      Caption         =   "Transmitter"
      Height          =   255
      Left            =   1440
      TabIndex        =   10
      Top             =   1800
      Width           =   1935
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public GPid1, GPid2 As Long
Public RBUF1, RBUF2 As String
Const GFLOW = 3 ' H/W flow control
Const GBAUD = B38400
Const GDATABITS = BIT_8
Const GSTOPBITS = STOP_1
Const GPARITY = P_IP_SERIAL_NONE
Public GSTATUS As String

Public Sub GetStatus()
    GSTATUS = ""
    
    Select Case GBAUD
    Case 0
        GSTATUS = GSTATUS + "300"
    Case 1
        GSTATUS = GSTATUS + "600"
    Case 2
        GSTATUS = GSTATUS + "1200"
    Case 3
        GSTATUS = GSTATUS + "2400"
    Case 4
        GSTATUS = GSTATUS + "4800"
    Case 5
        GSTATUS = GSTATUS + "7200"
    Case 6
        GSTATUS = GSTATUS + "9600"
    Case 7
        GSTATUS = GSTATUS + "19200"
    Case 8
        GSTATUS = GSTATUS + "38400"
    Case 9
        GSTATUS = GSTATUS + "57600"
    Case 10
        GSTATUS = GSTATUS + "115200"
    Case 11
        GSTATUS = GSTATUS + "230400"
    Case 12
        GSTATUS = GSTATUS + "460800"
    Case 13
        GSTATUS = GSTATUS + "921600"
    Case 14
        GSTATUS = GSTATUS + "150"
    Case 15
        GSTATUS = GSTATUS + "134"
    Case 16
        GSTATUS = GSTATUS + "110"
    Case 17
        GSTATUS = GSTATUS + "75"
    Case 18
        GSTATUS = GSTATUS + "50"
    End Select
    
    GSTATUS = GSTATUS + ","
    
    Select Case GPARITY
    Case 0
        GSTATUS = GSTATUS + "N"
    Case 8
        GSTATUS = GSTATUS + "E"
    Case 16
        GSTATUS = GSTATUS + "O"
    Case 24
        GSTATUS = GSTATUS + "M"
    Case 32
        GSTATUS = GSTATUS + "S"
    End Select
    
    Select Case GDATABITS
    Case 0
        GSTATUS = GSTATUS + "5"
    Case 1
        GSTATUS = GSTATUS + "6"
    Case 2
        GSTATUS = GSTATUS + "7"
    Case 3
        GSTATUS = GSTATUS + "8"
    End Select
    
    Select Case GSTOPBITS
    Case 0
        GSTATUS = GSTATUS + "1"
    Case 4
        GSTATUS = GSTATUS + "2"
    End Select
    
    GSTATUS = GSTATUS + ","
    
    Select Case GFLOW
    Case 0
        GSTATUS = GSTATUS + "No flow control"
    Case 3
        GSTATUS = GSTATUS + "H/W flow control"
    Case 12
        GSTATUS = GSTATUS + "S/W flow control"
    Case 15
        GSTATUS = GSTATUS + "H/W,S/W flow control"
    End Select
    
End Sub

Private Sub Command1_Click()
' Open port
    Static pid1, pid2 As Integer
    Static ret As Integer
    Static port_id As Long
    
    pid1 = Val(Text2.Text)
    pid2 = Val(Text4.Text)
    
    port_id = nsio_open(Text1.Text, pid1, 3000)
    If port_id < NSIO_OK Then
        MsgBox "Open port 1 fail!", vbOKOnly, "Open Port"
        Exit Sub
    Else
        GPid1 = port_id
    End If
    
    port_id = nsio_open(Text3.Text, pid2, 3000)
    If port_id < NSIO_OK Then
        MsgBox "Open port 2 fail!", vbOKOnly, "Open Port"
        nsio_close (GPid1)
        Exit Sub
    Else
        GPid2 = port_id
    End If
    
    MsgBox "Open port OK!", vbOKOnly, "Open Port"
    
    Command1.Enabled = False
    Command2.Enabled = True
        
    Text1.Enabled = False
    Text2.Enabled = False
    Text3.Enabled = False
    Text4.Enabled = False
        
    Text5.Enabled = True
    Text6.Enabled = True
    
    ret = nsio_ioctl(GPid1, GBAUD, GDATABITS Or GSTOPBITS Or GPARITY)
    If ret <> NSIO_OK Then
        MsgBox "IO control fail!", vbOKOnly
        Exit Sub
    End If
                
    ret = nsio_flowctrl(GPid1, GFLOW)
    
    ret = nsio_ioctl(GPid2, GBAUD, GDATABITS Or GSTOPBITS Or GPARITY)
    If ret <> NSIO_OK Then
        MsgBox "IO control fail!", vbOKOnly
        Exit Sub
    End If
                
    ret = nsio_flowctrl(GPid2, GFLOW)
    
    Form1.GetStatus
    
    Label9.Caption = GSTATUS
    Label10.Caption = GSTATUS
    Timer1.Enabled = True

End Sub

Private Sub Command2_Click()
' Close Port
    Static ret As Integer
    
    ret = nsio_close(GPid1)
    If ret <> NSIO_OK Then
        MsgBox "Close port fail!", vbOKOnly, "Close Port"
        Exit Sub
    End If

    ret = nsio_close(GPid2)
    If ret <> NSIO_OK Then
        MsgBox "Close port fail!", vbOKOnly, "Close Port"
        Exit Sub
    End If
    
    MsgBox "Close port OK!", vbOKOnly, "Close Port"
    
    Command1.Enabled = True
    Command2.Enabled = False
        
    Text1.Enabled = True
    Text2.Enabled = True
    Text3.Enabled = True
    Text4.Enabled = True
    
    Text5.Enabled = False
    Text6.Enabled = False
    
    Label9.Caption = ""
    Label10.Caption = ""
    
    Timer1.Enabled = False
    
End Sub

Private Sub Command3_Click()
    Unload Form1
End Sub


Private Sub Form_Load()
    Call nsio_init
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Call nsio_end
End Sub

Private Sub Text5_KeyPress(KeyAscii As Integer)
    Static ret As Integer
    Static key As Byte
    
    key = KeyAscii
    ret = nsio_write(GPid1, key, 1)
    RBUF = Text3.Text

End Sub


Private Sub Text6_KeyPress(KeyAscii As Integer)
    Static ret As Integer
    Static key As Byte
    
    key = KeyAscii
    ret = nsio_write(GPid2, key, 1)
    RBUF = Text3.Text

End Sub

Private Sub Timer1_Timer()
    Static ret, i As Integer
    Dim buf(0 To 15) As Byte
    
    ret = nsio_read(GPid1, buf(0), 16)
    If ret > 0 Then
        For i = 0 To (ret - 1)
            RBUF1 = RBUF1 + Chr(buf(i))
        Next i
        Text7.Text = RBUF1
    End If

    ret = nsio_read(GPid2, buf(0), 16)
    If ret > 0 Then
        For i = 0 To (ret - 1)
            RBUF2 = RBUF2 + Chr(buf(i))
        Next i
        Text8.Text = RBUF2
    End If
    
End Sub
