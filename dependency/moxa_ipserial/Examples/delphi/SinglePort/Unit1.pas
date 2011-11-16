unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls;

type
  TForm1 = class(TForm)
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Edit1: TEdit;
    Label2: TLabel;
    Edit2: TEdit;
    GroupBox2: TGroupBox;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    Button7: TButton;
    Button8: TButton;
    GroupBox3: TGroupBox;
    GroupBox4: TGroupBox;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Edit4: TEdit;
    Edit5: TEdit;
    Edit6: TEdit;
    Edit7: TEdit;
    Label7: TLabel;
    Label8: TLabel;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    ComboBox1: TComboBox;
    ComboBox2: TComboBox;
    ComboBox3: TComboBox;
    ComboBox4: TComboBox;
    ComboBox5: TComboBox;
    Label12: TLabel;
    Label13: TLabel;
    GroupBox5: TGroupBox;
    GroupBox6: TGroupBox;
    RadioButton1: TRadioButton;
    RadioButton2: TRadioButton;
    RadioButton3: TRadioButton;
    RadioButton4: TRadioButton;
    Timer1: TTimer;
    Memo1: TMemo;
    procedure Button8Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button7Click(Sender: TObject);
    procedure RadioButton1Click(Sender: TObject);
    procedure RadioButton2Click(Sender: TObject);
    procedure RadioButton3Click(Sender: TObject);
    procedure RadioButton4Click(Sender: TObject);
    procedure ComboBox1Change(Sender: TObject);
    procedure ComboBox4Change(Sender: TObject);
    procedure ComboBox2Change(Sender: TObject);
    procedure ComboBox5Change(Sender: TObject);
    procedure ComboBox3Change(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure Memo1KeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure Memo1KeyPress(Sender: TObject; var Key: Char);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

uses IPSerial, Unit2;
{$R *.DFM}
var
Pid: integer;
Br, Db, Sb, Pt: integer;
Bcnt: integer;
RBUF: string;

procedure TForm1.FormCreate(Sender: TObject);
begin
        Edit1.Text := '192.168.127.254';
        Edit2.Text := '1';
        ComboBox1.Items.Add('50');
        ComboBox1.Items.Add('75');
        ComboBox1.Items.Add('110');
        ComboBox1.Items.Add('134');
        ComboBox1.Items.Add('150');
        ComboBox1.Items.Add('300');
        ComboBox1.Items.Add('600');
        ComboBox1.Items.Add('1200');
        ComboBox1.Items.Add('1800');
        ComboBox1.Items.Add('2400');
        ComboBox1.Items.Add('4800');
        ComboBox1.Items.Add('7200');
        ComboBox1.Items.Add('9600');
        ComboBox1.Items.Add('19200');
        ComboBox1.Items.Add('38400');
        ComboBox1.Items.Add('57600');
        ComboBox1.Items.Add('115200');
        ComboBox1.Items.Add('230400');
        ComboBox1.Items.Add('460800');
        ComboBox1.Items.Add('921600');

        ComboBox2.Items.Add('1');
        ComboBox2.Items.Add('2');

        ComboBox3.Items.Add('NONE');
        ComboBox3.Items.Add('RTS/CTS');
        ComboBox3.Items.Add('XON/XOFF');
        ComboBox3.Items.Add('BOTH');

        ComboBox4.Items.Add('5');
        ComboBox4.Items.Add('6');
        ComboBox4.Items.Add('7');
        ComboBox4.Items.Add('8');

        ComboBox5.Items.Add('NONE');
        ComboBox5.Items.Add('EVEN');
        ComboBox5.Items.Add('ODD');
        ComboBox5.Items.Add('MARK');
        ComboBox5.Items.Add('SPACE');

        Edit4.Text := 'OFF';
        Edit5.Text := 'OFF';
        Edit6.Text := 'OFF';
        Edit7.Text := '0';

        ComboBox1.ItemIndex := 14;
        ComboBox2.ItemIndex := 0;
        ComboBox3.ItemIndex := 0;
        ComboBox4.ItemIndex := 3;
        ComboBox5.ItemIndex := 0;
        Br := B38400;
        Db := 0;
        Sb := 0;
        Pt := 0;
        nsio_init();
end;

procedure TForm1.Button5Click(Sender: TObject);
var
ret: integer;
ip: string;
begin
        ip := Edit1.Text;
        ret := nsio_checkalive(PChar(ip), 3000);
        if ret <> 0 then
                ShowMessage('Check server alive timeout!')
        else
                ShowMessage('Server is alived');
end;

procedure TForm1.Button4Click(Sender: TObject);
var
ret: integer;
ip, pswd: string;
begin
        ip := Edit1.Text;
        pswd := '';
        ret := nsio_resetserver(PChar(ip), PChar(pswd));
        if ret <> 0 then
                ShowMessage('Reset server failed!')
        else
                ShowMessage('Reset server OK!');
end;

procedure TForm1.Button8Click(Sender: TObject);
begin
        Close;
end;

procedure TForm1.Button1Click(Sender: TObject);
var
ret, code: integer;
ip: string;
pid: integer;
begin
        ip := Edit1.Text;
        Val(Edit2.Text, pid, code);
        ret := nsio_open(PChar(ip), pid, 3000);
        if ret < 0 then
                ShowMessage('Open port failed!')
        else
                begin
                Pid := ret;
                ShowMessage('Open port OK!');
                button1.Enabled := FALSE;
                button2.Enabled := TRUE;
                button3.Enabled := TRUE;
                button4.Enabled := FALSE;
                button5.Enabled := TRUE;
                button7.Enabled := TRUE;
                Edit1.Enabled := FALSE;
                Edit2.Enabled := FALSE;
                Memo1.Enabled := TRUE;
                ComboBox1.Enabled := TRUE;
                ComboBox2.Enabled := TRUE;
                ComboBox3.Enabled := TRUE;
                ComboBox4.Enabled := TRUE;
                ComboBox5.Enabled := TRUE;

                Br := Br;

                case ComboBox2.ItemIndex of
                0:      Sb := STOP_1;
                1:      Sb := STOP_2;
                else    Sb := STOP_1;
                end;

                Db := ComboBox4.ItemIndex;

                case ComboBox5.ItemIndex of
                0:      Pt := P_IP_SERIAL_NONE;
                1:      Pt := P_IP_SERIAL_EVEN;
                2:      Pt := P_IP_SERIAL_ODD;
                3:      Pt := P_IP_SERIAL_MARK;
                4:      Pt := P_IP_SERIAL_SPACE;
                else    Pt := P_IP_SERIAL_NONE;
                end;

                ret := nsio_ioctl(Pid, Br, Db or Sb or Pt);
                if ret <> 0 then
                        ShowMessage('Set IO control failed!');

                case ComboBox3.ItemIndex of
                0:      ret := nsio_flowctrl(Pid, 0);
                1:      ret := nsio_flowctrl(Pid, 3);
                2:      ret := nsio_flowctrl(Pid, 12);
                3:      ret := nsio_flowctrl(Pid, 15);
                end;
                if ret <> 0 then
                        ShowMessage('Set flow control failed!');

                Bcnt := 0;
                Timer1.Enabled := TRUE;
                
                end;
end;

procedure TForm1.Button2Click(Sender: TObject);
var
ret: integer;
begin
        ret := nsio_close(Pid);
        if ret <> 0 then
                ShowMessage('Close port failed!')
        else
                begin
                ShowMessage('Close port OK!');
                button1.Enabled := TRUE;
                button2.Enabled := FALSE;
                button3.Enabled := FALSE;
                button4.Enabled := TRUE;
                button5.Enabled := TRUE;
                button7.Enabled := FALSE;
                Edit1.Enabled := TRUE;
                Edit2.Enabled := TRUE;
                Memo1.Enabled := FALSE;
                ComboBox1.Enabled := FALSE;
                ComboBox2.Enabled := FALSE;
                ComboBox3.Enabled := FALSE;
                ComboBox4.Enabled := FALSE;
                ComboBox5.Enabled := FALSE;
                Timer1.Enabled := FALSE
                end;
end;

procedure TForm1.Button3Click(Sender: TObject);
var
ret: integer;
pswd: string;
begin
        pswd := '';
        ret := nsio_resetport(Pid, PChar(pswd));
        if ret <> 0 then
                ShowMessage('Reset port failed!')
        else
                ShowMessage('Reset port OK!')
end;

procedure TForm1.Button7Click(Sender: TObject);
var
ret: integer;
begin
        ret := nsio_break(Pid, 1);
        if ret <> 0 then
                ShowMessage('Send out BREAK signal failed!')
        else
                ShowMessage('Send out BREAK signal OK!');
end;

procedure TForm1.RadioButton1Click(Sender: TObject);
var
ret: integer;
begin
        ret := nsio_DTR(Pid, 1);
        if ret <> 0 then
                ShowMessage('Set DTR ON failed!')
        else
                ShowMessage('Set DTR ON OK!');
end;

procedure TForm1.RadioButton2Click(Sender: TObject);
var
ret: integer;
begin
        ret := nsio_DTR(Pid, 0);
        if ret <> 0 then
                ShowMessage('Set DTR OFF failed!')
        else
                ShowMessage('Set DTR OFF OK!');
end;

procedure TForm1.RadioButton3Click(Sender: TObject);
var
ret: integer;
begin
        ret := nsio_RTS(Pid, 1);
        if ret <> 0 then
                ShowMessage('Set RTS ON failed!')
        else
                ShowMessage('Set RTS ON OK!');
end;

procedure TForm1.RadioButton4Click(Sender: TObject);
var
ret: integer;
begin
        ret := nsio_RTS(Pid, 0);
        if ret <> 0 then
                ShowMessage('Set RTS OFF failed!')
        else
                ShowMessage('Set RTS OFF OK!');
end;

procedure TForm1.ComboBox1Change(Sender: TObject);
var
ret: integer;
begin
        case ComboBox1.ItemIndex of
        0:        begin ret := nsio_baud(Pid, 50);
                  Br := B50; end;
        1:        begin ret := nsio_baud(Pid, 75);
                  Br := B75; end;
        2:        begin ret := nsio_baud(Pid, 110);
                  Br := B110; end;
        3:        begin ret := nsio_baud(Pid, 134);
                  Br := 134; end;
        4:        begin ret := nsio_baud(Pid, 150);
                  Br := 150; end;
        5:        begin ret := nsio_baud(Pid, 300);
                  Br := B300; end;
        6:        begin ret := nsio_baud(Pid, 600);
                  Br := B600; end;
        7:        begin ret := nsio_baud(Pid, 1200);
                  Br := B1200; end;
        8:        begin ret := nsio_baud(Pid, 2400);
                  Br := B2400; end;
        9:        begin ret := nsio_baud(Pid, 4800);
                  Br := B4800; end;
        10:       begin ret := nsio_baud(Pid, 7200);
                  Br := B7200; end;
        11:       begin ret := nsio_baud(Pid, 9600);
                  Br := B9600; end;
        12:       begin ret := nsio_baud(Pid, 19200);
                  Br := B19200; end;
        13:       begin ret := nsio_baud(Pid, 38400);
                  Br := B38400; end;
        14:       begin ret := nsio_baud(Pid, 57600);
                  Br := B57600; end;
        15:       begin ret := nsio_baud(Pid, 115200);
                  Br := B115200; end;
        16:       begin ret := nsio_baud(Pid, 230400);
                  Br := B230400; end;
        17:       begin ret := nsio_baud(Pid, 460800);
                  Br := B460800; end;
        18:       begin ret := nsio_baud(Pid, 921600);
                  Br := B921600; end;
        else      ret := nsio_baud(Pid, 38400);
        end;
        if ret <> 0 then
                ShowMessage('Set Baud Rate failed!')
        else
                ShowMessage('Set Baud Rate OK!');
end;

procedure TForm1.ComboBox4Change(Sender: TObject);
var
ret: integer;
begin
        Db := ComboBox4.ItemIndex;
        ret := nsio_ioctl(Pid, Br, Db or Sb or Pt);
        if ret <> 0 then
                ShowMessage('Set Data Bits failed!')
        else
                ShowMessage('Set Data Bits OK!');
end;

procedure TForm1.ComboBox2Change(Sender: TObject);
var
ret: integer;
begin
        case ComboBox2.ItemIndex of
        0:      Sb := STOP_1;
        1:      Sb := STOP_2;
        else    Sb := STOP_1;
        end;

        ret := nsio_ioctl(Pid, Br, Db or Sb or Pt);
        if ret <> 0 then
                ShowMessage('Set Stop Bits failed!')
        else
                ShowMessage('Set Stop Bits OK!');
end;

procedure TForm1.ComboBox5Change(Sender: TObject);
var
ret: integer;
begin
        case ComboBox5.ItemIndex of
        0:      Pt := P_IP_SERIAL_NONE;
        1:      Pt := P_IP_SERIAL_EVEN;
        2:      Pt := P_IP_SERIAL_ODD;
        3:      Pt := P_IP_SERIAL_MARK;
        4:      Pt := P_IP_SERIAL_SPACE;
        else    Pt := P_IP_SERIAL_NONE;
        end;

        ret := nsio_ioctl(Pid, Br, Db or Sb or Pt);
        if ret <> 0 then
                ShowMessage('Set Parity failed!')
        else
                ShowMessage('Set Parity OK!');
end;

procedure TForm1.ComboBox3Change(Sender: TObject);
var
ret: integer;
begin
        case ComboBox3.ItemIndex of
        0:      ret := nsio_flowctrl(Pid, 0);
        1:      ret := nsio_flowctrl(Pid, 3);
        2:      ret := nsio_flowctrl(Pid, 12);
        3:      ret := nsio_flowctrl(Pid, 15);
        else    ret := nsio_flowctrl(Pid, 0);
        end;

        if ret <> 0 then
                ShowMessage('Set flow control failed!')
        else
                ShowMessage('Set flow control OK!');
end;

procedure TForm1.Timer1Timer(Sender: TObject);
var
ret: integer;
cnt, ls: integer;
s: string;
buf: array[0..16] of Char;
begin
        ret := nsio_read(Pid, @buf, 16);
        if ret > 0 then
        begin
                RBUF := RBUF + buf;
                Memo1.Text := RBUF;
        end;

        ls := nsio_lstatus(Pid);
        if (ls >= 0) and (ls <=15) then
        begin
        if (ls and 1) = 1 then
                Edit4.Text := 'ON'
        else
                Edit4.Text := 'OFF';

        if (ls and 2) = 2 then
                Edit5.Text := 'ON'
        else
                Edit5.Text := 'OFF';

        if (ls and 8) = 8 then
                Edit6.Text := 'ON'
        else
                Edit6.Text := 'OFF';
        end;

        cnt := nsio_breakcount(Pid);
        if cnt > 0 then
        begin
                Bcnt := Bcnt + cnt;
                Str(Bcnt, s);
                Edit7.Text := s;
        end;

end;

procedure TForm1.Memo1KeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
        RBUF := Memo1.Text;
end;

procedure TForm1.Memo1KeyPress(Sender: TObject; var Key: Char);
begin
        nsio_write(Pid, @Key, 1);
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
        nsio_end();
end;

end.
