object Form1: TForm1
  Left = 194
  Top = 117
  Width = 488
  Height = 480
  Caption = 'Single Port'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 16
    Width = 353
    Height = 57
    Caption = 'Options'
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 24
      Width = 47
      Height = 13
      Caption = 'Server IP:'
    end
    object Label2: TLabel
      Left = 216
      Top = 24
      Width = 22
      Height = 13
      Caption = 'Port:'
    end
    object Edit1: TEdit
      Left = 72
      Top = 24
      Width = 121
      Height = 21
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 0
      Text = '192.168.127.254'
    end
    object Edit2: TEdit
      Left = 256
      Top = 24
      Width = 73
      Height = 21
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 1
      Text = '1'
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 80
    Width = 353
    Height = 145
    Caption = 'Parameter'
    TabOrder = 1
    object Label7: TLabel
      Left = 8
      Top = 24
      Width = 54
      Height = 13
      Caption = 'Baud Rate:'
    end
    object Label8: TLabel
      Left = 184
      Top = 24
      Width = 45
      Height = 13
      Caption = 'Data bits:'
    end
    object Label9: TLabel
      Left = 8
      Top = 48
      Width = 44
      Height = 13
      Caption = 'Stop bits:'
    end
    object Label10: TLabel
      Left = 184
      Top = 48
      Width = 29
      Height = 13
      Caption = 'Parity:'
    end
    object Label11: TLabel
      Left = 8
      Top = 72
      Width = 61
      Height = 13
      Caption = 'Flow Control:'
    end
    object Label12: TLabel
      Left = 8
      Top = 112
      Width = 26
      Height = 13
      Caption = 'DTR:'
    end
    object Label13: TLabel
      Left = 184
      Top = 112
      Width = 25
      Height = 13
      Caption = 'RTS:'
    end
    object ComboBox1: TComboBox
      Left = 80
      Top = 24
      Width = 89
      Height = 21
      Enabled = False
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 0
      Text = 'ComboBox1'
      OnChange = ComboBox1Change
    end
    object ComboBox2: TComboBox
      Left = 80
      Top = 48
      Width = 89
      Height = 21
      Enabled = False
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 1
      Text = 'ComboBox2'
      OnChange = ComboBox2Change
    end
    object ComboBox3: TComboBox
      Left = 80
      Top = 72
      Width = 89
      Height = 21
      Enabled = False
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 2
      Text = 'ComboBox3'
      OnChange = ComboBox3Change
    end
    object ComboBox4: TComboBox
      Left = 248
      Top = 24
      Width = 89
      Height = 21
      Enabled = False
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 3
      Text = 'ComboBox4'
      OnChange = ComboBox4Change
    end
    object ComboBox5: TComboBox
      Left = 248
      Top = 48
      Width = 89
      Height = 21
      Enabled = False
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      ItemHeight = 13
      TabOrder = 4
      Text = 'ComboBox5'
      OnChange = ComboBox5Change
    end
    object GroupBox5: TGroupBox
      Left = 40
      Top = 96
      Width = 129
      Height = 41
      TabOrder = 5
      object RadioButton1: TRadioButton
        Left = 8
        Top = 16
        Width = 41
        Height = 17
        Caption = 'ON'
        Checked = True
        TabOrder = 0
        TabStop = True
        OnClick = RadioButton1Click
      end
      object RadioButton2: TRadioButton
        Left = 64
        Top = 16
        Width = 49
        Height = 17
        Caption = 'OFF'
        TabOrder = 1
        OnClick = RadioButton2Click
      end
    end
    object GroupBox6: TGroupBox
      Left = 216
      Top = 96
      Width = 129
      Height = 41
      TabOrder = 6
      object RadioButton3: TRadioButton
        Left = 8
        Top = 16
        Width = 41
        Height = 17
        Caption = 'ON'
        Checked = True
        TabOrder = 0
        TabStop = True
        OnClick = RadioButton3Click
      end
      object RadioButton4: TRadioButton
        Left = 64
        Top = 16
        Width = 49
        Height = 17
        Caption = 'OFF'
        TabOrder = 1
        OnClick = RadioButton4Click
      end
    end
  end
  object Button1: TButton
    Left = 384
    Top = 24
    Width = 75
    Height = 25
    Caption = 'Open'
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 384
    Top = 64
    Width = 75
    Height = 25
    Caption = 'Close'
    Enabled = False
    TabOrder = 3
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 384
    Top = 104
    Width = 75
    Height = 25
    Caption = 'Reset Port'
    Enabled = False
    TabOrder = 4
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 384
    Top = 144
    Width = 75
    Height = 25
    Caption = 'Reset Server'
    TabOrder = 5
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 384
    Top = 184
    Width = 75
    Height = 25
    Caption = 'Check Alive'
    TabOrder = 6
    OnClick = Button5Click
  end
  object Button7: TButton
    Left = 384
    Top = 232
    Width = 75
    Height = 25
    Caption = 'Send Break'
    Enabled = False
    TabOrder = 7
    OnClick = Button7Click
  end
  object Button8: TButton
    Left = 384
    Top = 408
    Width = 75
    Height = 25
    Caption = 'Exit'
    TabOrder = 8
    OnClick = Button8Click
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 392
    Width = 353
    Height = 57
    Caption = 'Status'
    TabOrder = 9
    object Label3: TLabel
      Left = 8
      Top = 24
      Width = 24
      Height = 13
      Caption = 'CTS:'
    end
    object Label4: TLabel
      Left = 88
      Top = 24
      Width = 26
      Height = 13
      Caption = 'DSR:'
    end
    object Label5: TLabel
      Left = 176
      Top = 24
      Width = 26
      Height = 13
      Caption = 'DCD:'
    end
    object Label6: TLabel
      Left = 256
      Top = 24
      Width = 31
      Height = 13
      Caption = 'Break:'
    end
    object Edit4: TEdit
      Left = 40
      Top = 24
      Width = 41
      Height = 21
      Enabled = False
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 0
      Text = 'Edit4'
    end
    object Edit5: TEdit
      Left = 120
      Top = 24
      Width = 41
      Height = 21
      Enabled = False
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 1
      Text = 'Edit5'
    end
    object Edit6: TEdit
      Left = 208
      Top = 24
      Width = 41
      Height = 21
      Enabled = False
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 2
      Text = 'Edit6'
    end
    object Edit7: TEdit
      Left = 296
      Top = 24
      Width = 41
      Height = 21
      Enabled = False
      ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
      TabOrder = 3
      Text = 'Edit7'
    end
  end
  object GroupBox4: TGroupBox
    Left = 8
    Top = 232
    Width = 353
    Height = 153
    Caption = 'Transmit/Receive'
    TabOrder = 10
  end
  object Memo1: TMemo
    Left = 16
    Top = 248
    Width = 337
    Height = 129
    Enabled = False
    ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
    ScrollBars = ssVertical
    TabOrder = 11
    OnKeyPress = Memo1keyPress
    OnKeyUp = Memo1KeyUp
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 400
    Top = 320
  end
end
