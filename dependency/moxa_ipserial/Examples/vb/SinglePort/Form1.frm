VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "SinglePort"
   ClientHeight    =   7530
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   8880
   LinkTopic       =   "Form1"
   ScaleHeight     =   7530
   ScaleWidth      =   8880
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   7680
      Top             =   5160
   End
   Begin VB.Frame Frame4 
      Caption         =   "Parameter"
      Height          =   2295
      Left            =   120
      TabIndex        =   18
      Top             =   960
      Width           =   7000
      Begin VB.Frame Frame6 
         Height          =   495
         Left            =   4200
         TabIndex        =   39
         Top             =   1680
         Width           =   2175
         Begin VB.OptionButton Option4 
            Caption         =   "OFF"
            Height          =   255
            Left            =   1080
            TabIndex        =   41
            Top             =   180
            Width           =   855
         End
         Begin VB.OptionButton Option3 
            Caption         =   "ON"
            Height          =   255
            Left            =   120
            TabIndex        =   40
            Top             =   180
            Value           =   -1  'True
            Width           =   975
         End
      End
      Begin VB.Frame Frame5 
         Height          =   495
         Left            =   960
         TabIndex        =   36
         Top             =   1680
         Width           =   2175
         Begin VB.OptionButton Option2 
            Caption         =   "OFF"
            Height          =   255
            Left            =   1080
            TabIndex        =   38
            Top             =   180
            Width           =   975
         End
         Begin VB.OptionButton Option1 
            Caption         =   "ON"
            Height          =   255
            Left            =   120
            TabIndex        =   37
            Top             =   180
            Value           =   -1  'True
            Width           =   735
         End
      End
      Begin VB.ComboBox Combo3 
         Enabled         =   0   'False
         Height          =   315
         Left            =   1560
         TabIndex        =   28
         Text            =   "NONE"
         Top             =   1360
         Width           =   1600
      End
      Begin VB.ComboBox Combo5 
         Enabled         =   0   'False
         Height          =   315
         Left            =   4800
         TabIndex        =   26
         Text            =   "NONE"
         Top             =   860
         Width           =   1600
      End
      Begin VB.ComboBox Combo2 
         Enabled         =   0   'False
         Height          =   315
         Left            =   1560
         TabIndex        =   24
         Text            =   "1"
         Top             =   860
         Width           =   1600
      End
      Begin VB.ComboBox Combo4 
         Enabled         =   0   'False
         Height          =   315
         Left            =   4800
         TabIndex        =   22
         Text            =   "8"
         Top             =   360
         Width           =   1600
      End
      Begin VB.ComboBox Combo1 
         Enabled         =   0   'False
         Height          =   315
         ItemData        =   "Form1.frx":0000
         Left            =   1560
         List            =   "Form1.frx":0002
         TabIndex        =   20
         Text            =   "38400"
         Top             =   360
         Width           =   1600
      End
      Begin VB.Label Label7 
         Caption         =   "RTS:"
         Height          =   255
         Left            =   3500
         TabIndex        =   30
         Top             =   1860
         Width           =   735
      End
      Begin VB.Label Label6 
         Caption         =   "DTR:"
         Height          =   255
         Left            =   240
         TabIndex        =   29
         Top             =   1860
         Width           =   615
      End
      Begin VB.Label Label5 
         Caption         =   "Flow Control:"
         Height          =   255
         Left            =   240
         TabIndex        =   27
         Top             =   1360
         Width           =   1575
      End
      Begin VB.Label Label9 
         Caption         =   "Parity:"
         Height          =   255
         Left            =   3500
         TabIndex        =   25
         Top             =   860
         Width           =   855
      End
      Begin VB.Label Label4 
         Caption         =   "Stop Bits:"
         Height          =   255
         Left            =   240
         TabIndex        =   23
         Top             =   860
         Width           =   1215
      End
      Begin VB.Label Label8 
         Caption         =   "Data bits:"
         Height          =   255
         Left            =   3500
         TabIndex        =   21
         Top             =   360
         Width           =   975
      End
      Begin VB.Label Label3 
         Caption         =   "Baud Rate:"
         Height          =   255
         Left            =   240
         TabIndex        =   19
         Top             =   360
         Width           =   1215
      End
   End
   Begin VB.CommandButton Command8 
      Caption         =   "Exit"
      Height          =   375
      Left            =   7500
      TabIndex        =   9
      Top             =   6840
      Width           =   1215
   End
   Begin VB.CommandButton Command7 
      Caption         =   "Send Break"
      Enabled         =   0   'False
      Height          =   375
      Left            =   7500
      TabIndex        =   8
      Top             =   3360
      Width           =   1215
   End
   Begin VB.CommandButton Command5 
      Caption         =   "Check Alive"
      Height          =   375
      Left            =   7500
      TabIndex        =   7
      Top             =   2760
      Width           =   1215
   End
   Begin VB.CommandButton Command4 
      Caption         =   "Reset Server"
      Height          =   375
      Left            =   7500
      TabIndex        =   6
      Top             =   2160
      Width           =   1215
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Reset Port"
      Enabled         =   0   'False
      Height          =   375
      Left            =   7500
      TabIndex        =   5
      Top             =   1560
      Width           =   1215
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Close"
      Enabled         =   0   'False
      Height          =   375
      Left            =   7500
      TabIndex        =   4
      Top             =   960
      Width           =   1215
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Open"
      Height          =   375
      Left            =   7500
      TabIndex        =   3
      Top             =   360
      Width           =   1215
   End
   Begin VB.Frame Frame3 
      Caption         =   "Status"
      Height          =   735
      Left            =   120
      TabIndex        =   2
      Top             =   6720
      Width           =   7000
      Begin VB.TextBox Text7 
         Enabled         =   0   'False
         Height          =   285
         Left            =   5280
         TabIndex        =   35
         Text            =   "Text7"
         Top             =   280
         Width           =   600
      End
      Begin VB.TextBox Text6 
         Enabled         =   0   'False
         Height          =   285
         Left            =   3720
         TabIndex        =   34
         Text            =   "Text6"
         Top             =   280
         Width           =   600
      End
      Begin VB.TextBox Text5 
         Enabled         =   0   'False
         Height          =   285
         Left            =   2160
         TabIndex        =   33
         Text            =   "Text5"
         Top             =   280
         Width           =   600
      End
      Begin VB.TextBox Text4 
         Enabled         =   0   'False
         Height          =   285
         Left            =   720
         TabIndex        =   32
         Text            =   "Text4"
         Top             =   280
         Width           =   600
      End
      Begin VB.Label Label13 
         Caption         =   "Break:"
         Height          =   255
         Left            =   4680
         TabIndex        =   31
         Top             =   280
         Width           =   495
      End
      Begin VB.Label Label12 
         Caption         =   "DCD:"
         Height          =   255
         Left            =   3120
         TabIndex        =   17
         Top             =   280
         Width           =   405
      End
      Begin VB.Label Label11 
         Caption         =   "DSR:"
         Height          =   255
         Left            =   1640
         TabIndex        =   16
         Top             =   280
         Width           =   400
      End
      Begin VB.Label Label10 
         Caption         =   "CTS:"
         Height          =   255
         Left            =   240
         TabIndex        =   15
         Top             =   280
         Width           =   615
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "Transmit/Receive"
      Height          =   3255
      Left            =   120
      TabIndex        =   1
      Top             =   3360
      Width           =   7000
      Begin VB.TextBox Text3 
         Height          =   2895
         Left            =   120
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   14
         Top             =   240
         Width           =   6735
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Options"
      Height          =   735
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   7000
      Begin VB.TextBox PortID 
         Height          =   285
         Left            =   4800
         TabIndex        =   13
         Top             =   300
         Width           =   735
      End
      Begin VB.TextBox ServerIP 
         Height          =   300
         Left            =   1200
         TabIndex        =   12
         Top             =   300
         Width           =   2000
      End
      Begin VB.Label Label2 
         Caption         =   "Port:"
         Height          =   255
         Left            =   3500
         TabIndex        =   11
         Top             =   300
         Width           =   1095
      End
      Begin VB.Label Label1 
         Caption         =   "Server IP:"
         Height          =   255
         Left            =   240
         TabIndex        =   10
         Top             =   300
         Width           =   1575
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public GPid As Integer
Public Br, Db, Sb, Pt, Fc As Integer
Public RBUF As String
Public Bcnt As Integer



Private Sub Form_Load()
    Combo1.AddItem "50"
    Combo1.AddItem "75"
    Combo1.AddItem "110"
    Combo1.AddItem "134.5"
    Combo1.AddItem "150"
    Combo1.AddItem "300"
    Combo1.AddItem "600"
    Combo1.AddItem "1200"
    Combo1.AddItem "1800"
    Combo1.AddItem "2400"
    Combo1.AddItem "4800"
    Combo1.AddItem "7200"
    Combo1.AddItem "9600"
    Combo1.AddItem "19200"
    Combo1.AddItem "38400"
    Combo1.AddItem "57600"
    Combo1.AddItem "115200"
    Combo1.AddItem "230400"
    Combo1.AddItem "460800"
    Combo1.AddItem "921600"
    
    Combo2.AddItem "1"
    Combo2.AddItem "2"
    
    Combo3.AddItem "NONE"
    Combo3.AddItem "RTS/CTS"
    Combo3.AddItem "XON/XOFF"
    Combo3.AddItem "BOTH"
    
    Combo4.AddItem "5"
    Combo4.AddItem "6"
    Combo4.AddItem "7"
    Combo4.AddItem "8"
    
    Combo5.AddItem "NONE"
    Combo5.AddItem "EVEN"
    Combo5.AddItem "ODD"
    Combo5.AddItem "MARK"
    Combo5.AddItem "SPACE"
    
    Option1.Value = True
    Option2.Value = False
    Option3.Value = True
    Option4.Value = False
    
    ServerIP.Text = "192.168.127.254"
    PortID.Text = "1"
    Text3.Text = ""
    Text4.Text = "OFF"
    Text5.Text = "OFF"
    Text6.Text = "OFF"
    Text7.Text = "0"
    
    Call nsio_init
       
End Sub

Private Sub Combo1_Click()
' baud rate changed
Static ret As Integer

    ret = nsio_baud(GPid, Val(Combo1.Text))
    If ret <> NSIO_OK Then
        MsgBox "Baud rate changed fail!", vbOKOnly
    Else
        MsgBox "Baud rate changed OK!", vbOKOnly
    End If

End Sub

Private Sub Combo2_Click()
' stop bits changed
Static ret As Integer

    Sb = Combo2.ListIndex
    Select Case Sb
    Case 0
        Sb = 0
    Case 1
        Sb = 4
    End Select
    
    ret = nsio_ioctl(GPid, 14, Db Or Sb Or Pt)
    ret = nsio_baud(GPid, Val(Combo1.Text))
    If ret <> NSIO_OK Then
        MsgBox "Stop bits changed fail!", vbOKOnly
    Else
        MsgBox "Stop bits changed OK!", vbOKOnly
    End If
    
End Sub

Private Sub Combo3_Click()
' flow control changed
Static ret As Integer

    Fc = Combo3.ListIndex
    Select Case Fc
    Case 0
        Fc = 0
    Case 1
        Fc = 3
    Case 2
        Fc = 12
    Case 3
        Fc = 15
    End Select
    
    ret = nsio_flowctrl(GPid, Fc)
    If ret <> NSIO_OK Then
        MsgBox "Flow control changed fail!", vbOKOnly
    Else
        MsgBox "Flow control changed OK!", vbOKOnly
    End If
    

End Sub


Private Sub Combo4_Click()
' data bits changed
Static ret As Integer

    Db = Combo4.ListIndex
    ret = nsio_ioctl(GPid, 14, Db Or Sb Or Pt)
    ret = nsio_baud(GPid, Val(Combo1.Text))
    If ret <> NSIO_OK Then
        MsgBox "Data bits changed fail!", vbOKOnly
    Else
        MsgBox "Data bits changed OK!", vbOKOnly
    End If
    
End Sub


Private Sub Combo5_Click()
' parity changed
Static ret As Integer

    Pt = Combo5.ListIndex
    Select Case Pt
    Case 0
        Pt = 0
    Case 1
        Pt = 8
    Case 2
        Pt = 16
    Case 3
        Pt = 24
    Case 4
        Pt = 32
    End Select
    
    ret = nsio_ioctl(GPid, 14, Db Or Sb Or Pt)
    ret = nsio_baud(GPid, Val(Combo1.Text))
    If ret <> NSIO_OK Then
        MsgBox "Parity changed fail!", vbOKOnly
    Else
        MsgBox "Parity changed OK!", vbOKOnly
    End If
    
End Sub


Private Sub Command1_Click()
' Open port
    Static pid As Integer
    Static ret As Integer
    Static port_id As Long
    
    pid = Val(PortID.Text)
    
    port_id = nsio_open(ServerIP.Text, pid, 3000)
    If port_id < NSIO_OK Then
        MsgBox "Open port fail!", vbOKOnly, "Open Port"
    Else
        MsgBox "Open port OK!", vbOKOnly, "Open Port"
        GPid = ret
        Command1.Enabled = False
        Command2.Enabled = True
        Command3.Enabled = True
        Command4.Enabled = False
        Command7.Enabled = True
        Combo1.Enabled = True
        Combo2.Enabled = True
        Combo3.Enabled = True
        Combo4.Enabled = True
        Combo5.Enabled = True
        
        ServerIP.Enabled = False
        PortID.Enabled = False
        
        Br = Combo1.ListIndex
        Sb = Combo2.ListIndex
        Fc = Combo3.ListIndex
        Db = Combo4.ListIndex
        Pt = Combo5.ListIndex
        
        If Db = -1 Then
            Db = 3
        End If
        
        Select Case Pt
        Case -1
            Pt = 0
        Case 0
            Pt = 0
        Case 1
            Pt = 8
        Case 2
            Pt = 16
        Case 3
            Pt = 24
        Case 4
            Pt = 32
        End Select
        
        Select Case Fc
        Case -1
            Fc = 0
        Case 0
            Fc = 0
        Case 1
            Fc = 3
        Case 2
            Fc = 12
        Case 3
            Fc = 15
        End Select
        
        Select Case Sb
        Case -1
            Sb = 0
        Case 0
            Sb = 0
        Case 1
            Sb = 4
        End Select
        
        ret = nsio_ioctl(GPid, 14, Db Or Sb Or Pt)
        If ret <> NSIO_OK Then
            MsgBox "IO control fail!", vbOKOnly
        End If
        
        ret = nsio_baud(GPid, Val(Combo1.Text))
        
        ret = nsio_flowctrl(GPid, Fc)
        
        Timer1.Enabled = True
        
        Bcnt = 0
    End If
    
End Sub

Private Sub Command2_Click()
' Close Port
    Static ret As Integer
    
    ret = nsio_close(GPid)
    If ret = NSIO_OK Then
        MsgBox "Close port OK!", vbOKOnly, "Close Port"
        Command1.Enabled = True
        Command2.Enabled = False
        Command3.Enabled = False
        Command4.Enabled = True
        Command7.Enabled = False
        Combo1.Enabled = False
        Combo2.Enabled = False
        Combo3.Enabled = False
        Combo4.Enabled = False
        Combo5.Enabled = False
        
        ServerIP.Enabled = True
        PortID.Enabled = True
        
        Timer1.Enabled = False
    Else
        MsgBox "Close port fail!", vbOKOnly, "Close Port"
    End If
End Sub


Private Sub Command3_Click()
' Reset port
    Dim pswd As String
    Static ret As Integer

    pswd = ""
    ret = nsio_resetport(GPid, pswd)
    If ret <> NSIO_OK Then
        MsgBox "Reset port fail!", vbOKOnly, "Reset Port"
    Else
        MsgBox "Reset port Ok!", vbOKOnly, "Reset Port"
    End If
    

End Sub


Private Sub Command4_Click()
' Reset Server
    Dim pswd As String
    Static ret As Integer

    pswd = ""
    ret = nsio_resetserver(ServerIP.Text, pswd)
    If ret <> NSIO_OK Then
        MsgBox "Reset Server fail!", vbOKOnly, "Reset Server"
    Else
        MsgBox "Reset Server Ok!", vbOKOnly, "Reset Server"
    End If
    

End Sub

Private Sub Command5_Click()
' Check alive
    Static ret As Integer
    Dim i As Integer
    
    ret = nsio_checkalive(ServerIP.Text, 3000)
    If ret = NSIO_OK Then
        MsgBox "Server is alived!", vbOKOnly, "Check Alive"
    Else
        MsgBox "Check alive timeout!", vbOKOnly, "Check Alive"
    End If
        
End Sub



Private Sub Command7_Click()
' Send break
    Static ret As Integer
        
    ret = nsio_break(GPid, 1)
    If ret <> NSIO_OK Then
        MsgBox "Send break fail!", vbOKOnly, "Send break"
    Else
        MsgBox "Send break OK!", vbOKOnly, "Send break"
    End If
    
End Sub

Private Sub Command8_Click()
    Unload Form1
End Sub



Private Sub Form_Unload(Cancel As Integer)
    Call nsio_end
End Sub

Private Sub Option1_Click()
' set DTR ON
    Static ret As Integer
    
    ret = nsio_DTR(GPid, 1)
    If ret = NSIO_OK Then
        MsgBox "Set DTR ON OK!", vbOKOnly, "Set DTR"
    Else
        MsgBox "Set DTR ON fail!", vbOKOnly, "Set DTR"
    End If
    
End Sub

Private Sub Option2_Click()
' set DTR OFF
    Static ret As Integer
    
    ret = nsio_DTR(GPid, 0)
    If ret = NSIO_OK Then
        MsgBox "Set DTR OFF OK!", vbOKOnly, "Set DTR"
    Else
        MsgBox "Set DTR OFF fail!", vbOKOnly, "Set DTR"
    End If
    
End Sub

Private Sub Option3_Click()
' set RTS ON
    Static ret As Integer
    
    ret = nsio_RTS(GPid, 1)
    If ret = NSIO_OK Then
        MsgBox "Set RTS ON OK!", vbOKOnly, "Set RTS"
    Else
        MsgBox "Set RTS ON fail!", vbOKOnly, "Set RTS"
    End If

End Sub

Private Sub Option4_Click()
' set RTS OFF
    Static ret As Integer
    
    ret = nsio_RTS(GPid, 0)
    If ret = NSIO_OK Then
        MsgBox "Set RTS OFF OK!", vbOKOnly, "Set RTS"
    Else
        MsgBox "Set RTS OFF fail!", vbOKOnly, "Set RTS"
    End If

End Sub

Private Sub Text3_KeyPress(KeyAscii As Integer)
    Static ret As Integer
    Static key As Byte
    
    key = KeyAscii
    ret = nsio_write(GPid, key, 1)
    RBUF = Text3.Text
    
End Sub

Private Sub Timer1_Timer()
    Static ret, i As Integer
    Dim buf(0 To 15) As Byte
    
    ret = nsio_read(GPid, buf(0), 16)
    If ret > 0 Then
        For i = 0 To (ret - 1)
            RBUF = RBUF + Chr(buf(i))
        Next i
        Text3.Text = RBUF
    End If
    
    ret = nsio_breakcount(GPid)
    If (ret > 0) Then
        Bcnt = Bcnt + ret
    End If
    Text7.Text = Str(Bcnt)
    
    ret = nsio_lstatus(GPid)
    If (ret And 1) = 1 Then
        Text4.Text = "ON"
    Else
        Text4.Text = "OFF"
    End If
    
    If (ret And 2) = 2 Then
        Text5.Text = "ON"
    Else
        Text5.Text = "OFF"
    End If
    
    If (ret And 8) = 8 Then
        Text6.Text = "ON"
    Else
        Text6.Text = "OFF"
    End If
    
End Sub
