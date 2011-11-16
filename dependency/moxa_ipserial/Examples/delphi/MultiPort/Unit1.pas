unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls;

type
  TForm1 = class(TForm)
    GroupBox1: TGroupBox;
    GroupBox2: TGroupBox;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Edit1: TEdit;
    Edit2: TEdit;
    Edit3: TEdit;
    Edit4: TEdit;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    Label12: TLabel;
    Memo1: TMemo;
    Memo2: TMemo;
    Memo3: TMemo;
    Memo4: TMemo;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Timer1: TTimer;
    procedure Button3Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Form1Create(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure Memo1KeyPress(Sender: TObject; var Key: Char);
    procedure Memo2KeyPress(Sender: TObject; var Key: Char);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

uses IPSerial;

{$R *.DFM}
var
GPid1, GPid2: integer;
RBUF1, RBUF2: string;
GSTATUS: string;
Br, Db, Sb, Pt, Fc: integer;


procedure TForm1.Form1Create(Sender: TObject);
begin
        Br := B38400;
        Db := BIT_8;
        Sb := STOP_1;
        Pt := P_IP_SERIAL_NONE;
        Fc := 3;        {H/W flow control}
        GSTATUS := '';
        nsio_init();
end;

procedure TForm1.Button3Click(Sender: TObject);
begin
        Close;
end;

procedure TForm1.Button1Click(Sender: TObject);
var
ret, code: integer;
ip1, ip2: string;
pid1, pid2: integer;
begin
        ip1 := Edit1.Text;
        ip2 := Edit3.Text;
        Val(Edit2.Text, pid1, code);
        Val(Edit4.Text, pid2, code);

        ret := nsio_open(PChar(ip1), pid1, 3000);
        if ret < 0 then
        begin
                ShowMessage('Open port one failed!');
                Exit
        end
        else
                GPid1 := ret;

        ret := nsio_open(PChar(ip2), pid2, 3000);
        if ret < 0 then
        begin
                ShowMessage('Open port two failed!');
                Exit
        end
        else
                GPid2 := ret;

        ShowMessage('Open port OK!');

        button1.Enabled := FALSE;
        button2.Enabled := TRUE;
        Edit1.Enabled := FALSE;
        Edit2.Enabled := FALSE;
        Edit3.Enabled := FALSE;
        Edit4.Enabled := FALSE;
        Memo1.Enabled := TRUE;
        Memo2.Enabled := TRUE;

        ret := nsio_ioctl(GPid1, Br, Db or Sb or Pt);
        if ret <> 0 then
                ShowMessage('Set port one IO control failed!');
        ret := nsio_ioctl(GPid2, Br, Db or Sb or Pt);
        if ret <> 0 then
                ShowMessage('Set port two IO control failed!');

        ret := nsio_flowctrl(GPid1, 3);
        if ret <> 0 then
                ShowMessage('Set port one flow control failed!');
        ret := nsio_flowctrl(GPid2, 3);
        if ret <> 0 then
                ShowMessage('Set port two flow control failed!');

        case Br of
        B50:      GSTATUS := GSTATUS + '50';
        B75:      GSTATUS := GSTATUS + '75';
        B110:     GSTATUS := GSTATUS + '110';
        B134:     GSTATUS := GSTATUS + '134';
        B150:     GSTATUS := GSTATUS + '150';
        B300:     GSTATUS := GSTATUS + '300';
        B600:     GSTATUS := GSTATUS + '600';
        B1200:    GSTATUS := GSTATUS + '1200';
        B2400:    GSTATUS := GSTATUS + '2400';
        B4800:    GSTATUS := GSTATUS + '4800';
        B7200:    GSTATUS := GSTATUS + '7200';
        B9600:    GSTATUS := GSTATUS + '9600';
        B19200:   GSTATUS := GSTATUS + '19200';
        B38400:   GSTATUS := GSTATUS + '38400';
        B57600:   GSTATUS := GSTATUS + '57600';
        B115200:  GSTATUS := GSTATUS + '115200';
        B230400:  GSTATUS := GSTATUS + '230400';
        B460800:  GSTATUS := GSTATUS + '460800';
        B921600:  GSTATUS := GSTATUS + '921600';
        else      GSTATUS := GSTATUS + '38400';
        end;

        case Pt of
        P_IP_SERIAL_NONE:   GSTATUS := GSTATUS + ',N';
        P_IP_SERIAL_EVEN:   GSTATUS := GSTATUS + ',E';
        P_IP_SERIAL_ODD:    GSTATUS := GSTATUS + ',O';
        P_IP_SERIAL_MARK:   GSTATUS := GSTATUS + ',M';
        P_IP_SERIAL_SPACE:  GSTATUS := GSTATUS + ',S';
        else      GSTATUS := GSTATUS + ',N';
        end;

        case Db of
        BIT_5:    GSTATUS := GSTATUS + '5';
        BIT_6:    GSTATUS := GSTATUS + '6';
        BIT_7:    GSTATUS := GSTATUS + '7';
        BIT_8:    GSTATUS := GSTATUS + '8';
        else      GSTATUS := GSTATUS + '8';
        end;

        case Sb of
        STOP_1:   GSTATUS := GSTATUS + '1';
        STOP_2:   GSTATUS := GSTATUS + '2';
        else      GSTATUS := GSTATUS + '1';
        end;

        case Fc of
        0:        GSTATUS := GSTATUS + ',no flow control';
        3:        GSTATUS := GSTATUS + ',H/W flow control';
        12:       GSTATUS := GSTATUS + ',S/W flow control';
        15:       GSTATUS := GSTATUS + ',H/W,S/W flow control';
        else      GSTATUS := GSTATUS + ',no flow control';
        end;

        Memo1.Clear();
        Memo2.Clear();
        Memo3.Clear();
        Memo4.Clear();
        Label7.Caption := GSTATUS;
        Label8.Caption := GSTATUS;
        RBUF1 := '';
        RBUF2 := '';

        Timer1.Enabled := TRUE;
end;


procedure TForm1.Button2Click(Sender: TObject);
var
ret: integer;
begin
        ret := nsio_close(GPid1);
        if ret <> 0 then
        begin
                ShowMessage('Close port one failed!');
                Exit
        end;

        ret := nsio_close(GPid2);
        if ret <> 0 then
        begin
                ShowMessage('Close port two failed!');
                Exit
        end;

        ShowMessage('Close port OK!');

        button1.Enabled := TRUE;
        button2.Enabled := FALSE;
        Edit1.Enabled := TRUE;
        Edit2.Enabled := TRUE;
        Edit3.Enabled := TRUE;
        Edit4.Enabled := TRUE;
        Memo1.Enabled := FALSE;
        Memo2.Enabled := FALSE;

        Timer1.Enabled := FALSE;
        
        GSTATUS := '';
        Label7.Caption := GSTATUS;
        Label8.Caption := GSTATUS;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
var
ret: integer;
buf: array[0..16] of Char;
begin
        ret := nsio_read(GPid1, @buf, 16);
        if ret > 0 then
        begin
                RBUF1 := RBUF1 + buf;
                Memo3.Text := RBUF1;
        end;

        ret := nsio_read(GPid2, @buf, 16);
        if ret > 0 then
        begin
                RBUF2 := RBUF2 + buf;
                Memo4.Text := RBUF2;
        end;
end;

procedure TForm1.Memo1KeyPress(Sender: TObject; var Key: Char);
begin
        nsio_write(GPid1, @Key, 1);
end;



procedure TForm1.Memo2KeyPress(Sender: TObject; var Key: Char);
begin
        nsio_write(GPid2, @Key, 1);
end;



procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
        nsio_end();
end;

end.
