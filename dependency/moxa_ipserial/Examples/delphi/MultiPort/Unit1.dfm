object Form1: TForm1
  Left = 192
  Top = 113
  Width = 498
  Height = 480
  Caption = 'Multiport'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = Form1Create
  PixelsPerInch = 96
  TextHeight = 13
  object Label5: TLabel
    Left = 8
    Top = 88
    Width = 31
    Height = 13
    Caption = 'status:'
  end
  object Label6: TLabel
    Left = 248
    Top = 88
    Width = 31
    Height = 13
    Caption = 'status:'
  end
  object Label7: TLabel
    Left = 48
    Top = 88
    Width = 3
    Height = 13
  end
  object Label8: TLabel
    Left = 288
    Top = 88
    Width = 3
    Height = 13
  end
  object Label9: TLabel
    Left = 96
    Top = 112
    Width = 52
    Height = 13
    Caption = 'Transmitter'
  end
  object Label10: TLabel
    Left = 336
    Top = 112
    Width = 52
    Height = 13
    Caption = 'Transmitter'
  end
  object Label11: TLabel
    Left = 96
    Top = 264
    Width = 43
    Height = 13
    Caption = 'Receiver'
  end
  object Label12: TLabel
    Left = 344
    Top = 264
    Width = 43
    Height = 13
    Caption = 'Receiver'
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 233
    Height = 73
    Caption = 'One'
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 24
      Width = 47
      Height = 13
      Caption = 'Server IP:'
    end
    object Label2: TLabel
      Left = 16
      Top = 40
      Width = 62
      Height = 13
      Caption = 'Port Number:'
    end
    object Edit1: TEdit
      Left = 96
      Top = 16
      Width = 121
      Height = 21
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 0
      Text = '192.168.127.254'
    end
    object Edit2: TEdit
      Left = 96
      Top = 40
      Width = 65
      Height = 21
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 1
      Text = '1'
    end
  end
  object GroupBox2: TGroupBox
    Left = 248
    Top = 8
    Width = 233
    Height = 73
    Caption = 'Two'
    TabOrder = 1
    object Label3: TLabel
      Left = 16
      Top = 24
      Width = 47
      Height = 13
      Caption = 'Server IP:'
    end
    object Label4: TLabel
      Left = 16
      Top = 40
      Width = 62
      Height = 13
      Caption = 'Port Number:'
    end
    object Edit3: TEdit
      Left = 96
      Top = 16
      Width = 121
      Height = 21
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 0
      Text = '192.168.127.254'
    end
    object Edit4: TEdit
      Left = 96
      Top = 40
      Width = 65
      Height = 21
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 1
      Text = '2'
    end
  end
  object Memo1: TMemo
    Left = 8
    Top = 128
    Width = 233
    Height = 129
    Enabled = False
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    ScrollBars = ssVertical
    TabOrder = 2
    OnKeyPress = Memo1KeyPress
  end
  object Memo2: TMemo
    Left = 248
    Top = 128
    Width = 233
    Height = 129
    Enabled = False
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    ScrollBars = ssVertical
    TabOrder = 3
    OnKeyPress = Memo2KeyPress
  end
  object Memo3: TMemo
    Left = 8
    Top = 280
    Width = 233
    Height = 129
    Color = clMenu
    Enabled = False
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    ScrollBars = ssVertical
    TabOrder = 4
  end
  object Memo4: TMemo
    Left = 248
    Top = 280
    Width = 233
    Height = 129
    Color = clMenu
    Enabled = False
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    ScrollBars = ssVertical
    TabOrder = 5
  end
  object Button1: TButton
    Left = 72
    Top = 416
    Width = 75
    Height = 25
    Caption = 'Open'
    TabOrder = 6
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 208
    Top = 416
    Width = 75
    Height = 25
    Caption = 'Close'
    Enabled = False
    TabOrder = 7
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 344
    Top = 416
    Width = 75
    Height = 25
    Caption = 'Exit'
    TabOrder = 8
    OnClick = Button3Click
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer1Timer
    Left = 176
    Top = 88
  end
end
