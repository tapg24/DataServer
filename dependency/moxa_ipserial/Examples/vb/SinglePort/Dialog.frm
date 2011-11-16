VERSION 5.00
Begin VB.Form Dialog 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Server Information"
   ClientHeight    =   2805
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   4065
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2805
   ScaleWidth      =   4065
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Height          =   375
      Left            =   1440
      TabIndex        =   0
      Top             =   2280
      Width           =   1215
   End
   Begin VB.Label Label10 
      Caption         =   "Server Model:"
      Height          =   255
      Left            =   120
      TabIndex        =   10
      Top             =   1680
      Width           =   1335
   End
   Begin VB.Label Label9 
      Caption         =   "Label9"
      Height          =   255
      Left            =   1680
      TabIndex        =   9
      Top             =   1680
      Width           =   2175
   End
   Begin VB.Label Label8 
      Caption         =   "Label8"
      Height          =   255
      Left            =   1680
      TabIndex        =   8
      Top             =   1320
      Width           =   2175
   End
   Begin VB.Label Label7 
      Caption         =   "Label7"
      Height          =   255
      Left            =   1680
      TabIndex        =   7
      Top             =   960
      Width           =   2175
   End
   Begin VB.Label Label6 
      Caption         =   "Label6"
      Height          =   255
      Left            =   1680
      TabIndex        =   6
      Top             =   600
      Width           =   2175
   End
   Begin VB.Label Label5 
      Caption         =   "Label5"
      Height          =   255
      Left            =   1680
      TabIndex        =   5
      Top             =   240
      Width           =   2175
   End
   Begin VB.Label Label4 
      Caption         =   "Serial Number:"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   1320
      Width           =   1335
   End
   Begin VB.Label Label3 
      Caption         =   "MAC address:"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   960
      Width           =   1335
   End
   Begin VB.Label Label2 
      Caption         =   "Server IP:"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   600
      Width           =   1335
   End
   Begin VB.Label Label1 
      Caption         =   "Server Name:"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   240
      Width           =   1335
   End
End
Attribute VB_Name = "Dialog"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit

Private Sub OKButton_Click()
    Unload Dialog
End Sub
