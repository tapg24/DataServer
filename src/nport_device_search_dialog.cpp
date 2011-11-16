#include "nport_device_search_dialog.h"
#include "../dependency/DataServerEngine/include/channels/channel_modbus/channel_modbus_rtu.h"
#include "../dependency/DataServerEngine/include/channels/channel_modbus/modbus_except.hpp"
#include "../dependency/DataServerEngine/include/utils/bitwise.hpp"
#include "../dependency/DataServerEngine/include/utils/device_encode.h"

IMPLEMENT_DYNAMIC_CLASS(DeviceSearch_NPort, wxDialog)

DeviceSearch_NPort::DeviceSearch_NPort()
{

}

DeviceSearch_NPort::DeviceSearch_NPort( wxWindow* parent, wxWindowID id, const wxString & title, const wxPoint & pos /*= wxDefaultPosition*/, const wxSize & size /*= wxDefaultSize*/, long style /*= wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION*/ )
{
	Create(parent, id, title, pos, size, style);
}

DeviceSearch_NPort::~DeviceSearch_NPort()
{
	searchThread.join();
}

bool DeviceSearch_NPort::Create( wxWindow* parent, wxWindowID id, const wxString & title, const wxPoint & pos /*= wxDefaultPosition*/, const wxSize & size /*= wxDefaultSize*/, long style /*= wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION*/, const wxString & name /*= _T("Обзор серверов")*/ )
{
	if ( !wxDialog::Create(parent, id, title, pos, size, style, name) )
	{
		return false;
	}

	wxPanel* panel = new wxPanel(this);

	wxBoxSizer* main_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* list_sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* text_sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* btn_sizer = new wxBoxSizer(wxVERTICAL);

	main_sizer->Add(list_sizer, 1, wxEXPAND | wxALL, 10);
	main_sizer->Add(text_sizer, 0, wxEXPAND | wxALL, 10);
	main_sizer->Add(btn_sizer, 0, wxEXPAND | wxALL, 10);

	m_deviceView = new wxListView(panel, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	wxListItem col0, col1, col2;
	col0.SetId(0);
	col0.SetText( wxT("№") );
	col0.SetWidth(30);
	m_deviceView->InsertColumn(0, col0);
	col1.SetId(1);
	col1.SetText( wxT("Тип устройства") );
	col1.SetWidth(195);
	m_deviceView->InsertColumn(1, col1);
	col2.SetId(2);
	col2.SetText( wxT("Серийный номер") );
	col2.SetWidth(100);
	m_deviceView->InsertColumn(2, col2);
	list_sizer->Add(m_deviceView, 1, wxEXPAND | wxALL, 0);

	m_ipAdress = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 20), wxSUNKEN_BORDER);
	text_sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("IP адресс сервера:") ), 0, wxTOP | wxBOTTOM, 2);
	text_sizer->Add(m_ipAdress, 0, wxEXPAND | wxALL, 0);
	m_portId = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(40, 20), wxSUNKEN_BORDER);
	text_sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Номер порта:") ), 0, wxTOP | wxBOTTOM, 2);
	text_sizer->Add(m_portId, 0, wxEXPAND | wxALL, 0);
	m_baudRate = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(40, 20), wxSUNKEN_BORDER);
	text_sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Скорость подключения:") ), 0, wxTOP | wxBOTTOM, 2);
	text_sizer->Add(m_baudRate, 0, wxEXPAND | wxALL, 0);
	m_addrMin = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(40, 20), wxSUNKEN_BORDER);
	m_addrMin->SetValue("1");
	m_addrMax = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(40, 20), wxSUNKEN_BORDER);
	m_addrMax->SetValue("247");
	wxBoxSizer* addrSizer = new wxBoxSizer(wxHORIZONTAL);
	addrSizer->Add(m_addrMin, 0, wxALL, 0);
	addrSizer->Add(m_addrMax, 0, wxALL, 0);
	text_sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Диапазон адресов:") ), 0, wxTOP | wxBOTTOM, 2);
	text_sizer->Add(addrSizer, 0, wxALL, 0);
	m_pause = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(40, 20), wxSUNKEN_BORDER);
	m_pause->SetValue("0");
	text_sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Пауза:") ), 0, wxTOP | wxBOTTOM, 2);
	text_sizer->Add(m_pause, 0, wxALL, 0);

	m_startBtn = new wxButton( panel, ID_UpdateBtn, wxT("Начать поиск") );
	Connect(ID_UpdateBtn, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DeviceSearch_NPort::OnStartClicked));
	text_sizer->Add(m_startBtn, 0, wxEXPAND | wxALL, 10);

	m_btn_add = new wxButton( panel, ID_AddBtn, wxT("Добавить сервер") );
	Connect(ID_AddBtn, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DeviceSearch_NPort::OnAddClicked));
	text_sizer->Add(m_btn_add, 0, wxEXPAND | wxALL, 10);

	panel->SetSizer(main_sizer);

	SetSize(630, 300);

	return true;
}

void DeviceSearch_NPort::OnStartClicked( wxCommandEvent& event )
{
	searchThread = boost::thread(&DeviceSearch_NPort::SearchDeviceThread, this);
}

void DeviceSearch_NPort::OnAddClicked( wxCommandEvent& event )
{
	wxDialog::EndModal(wxID_OK);
}

void DeviceSearch_NPort::SearchDeviceThread()
{
	m_deviceView->DeleteAllItems();

	if ( m_ipAdress->GetLineText(0).IsEmpty() || m_portId->GetLineText(0).IsEmpty() || m_baudRate->GetLineText(0).IsEmpty())
	{
		wxMessageBox("Заполните все поля.");
		return;
	}

	std::string ip = m_ipAdress->GetLineText(0).mb_str();
	long port;
	m_portId->GetLineText(0).ToLong(&port);
	long speed;
	m_baudRate->GetLineText(0).ToLong(&speed);
	long timeout = 200;

	try
	{
		modbusStream_.reset( new ModbusRTU_NPort(ip, port, speed, timeout) );
		modbusStream_->Open();
	}
	catch(std::runtime_error&)
	{
		wxMessageBox("Не удалось открыть порт.");
		return;
	}
	catch(modbus_error_notalive&)
	{
		wxMessageBox("Сервер NPORT недоступен.");
		return;
	}

	m_startBtn->Disable();

	long rowId = 0;
	int32_t minIdx, maxIdx, pauseDuration;
	m_addrMin->GetLineText(0).ToLong(&minIdx);
	m_addrMax->GetLineText(0).ToLong(&maxIdx);
	m_pause->GetLineText(0).ToLong(&pauseDuration);
	for (int32_t idx = minIdx; idx < maxIdx; ++idx, ++rowId)
	{
		SetTitle( wxString() << "Адрес - " << idx << ". Количество устройств - " << m_deviceView->GetItemCount() );

		WORD deviceType = 0;
		WORD deviceId = 0;

		try
		{
			Sleep(pauseDuration);

			modbusStream_->ReadRegister(idx, 4, 4);
			modbusStream_->GetBuffer((BYTE*)&deviceType, 0, 2);
			deviceType = swap_endian<WORD>(deviceType);
			modbusStream_->GetBuffer((BYTE*)&deviceId, 2, 2);
			deviceId = swap_endian<WORD>(deviceId);
		}
		catch(modbus_error_notalive&)
		{
			modbusStream_->Close();
			wxMessageBox("Сервер NPORT недоступен.");
			return;
		}
		catch(modbus_error&)
		{
			continue;
		}

		rowId = m_deviceView->InsertItem(rowId, wxString() << idx);
		m_deviceView->SetItem(rowId, 1, wxString() << Sirius::GetName(deviceType));
		m_deviceView->SetItem(rowId, 2, wxString() << deviceId);
	}

	modbusStream_->Close();
	SetTitle( wxString() << "Поиск окончен. " << "Количество устройств - " << m_deviceView->GetItemCount() );
	m_startBtn->Enable();
}
