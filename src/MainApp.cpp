#include "../dependency/DataServerEngine/include/utils/types.h"
#include "../dependency/DataServerEngine/include/core/project_mgr.h"
#include "../dependency/FRL/include/frl_exception.h"

#include <boost/log/utility/init/to_file.hpp>
#include <boost/log/trivial.hpp>

#include "MainFrame.h"

#include <wx/wx.h>
#include <wx/snglinst.h>

class MyApp : public wxApp
{
	MainFrame* frame_;
	wxSingleInstanceChecker instanceChecker_;

public:

	MyApp() : instanceChecker_(wxT("SibIng DataServer")) {}
	~MyApp() {}

	bool OnInit()
	{
		FRL_EXCEPT_GUARD();

		// setting logger
		/*boost::filesystem::path log_path = Util::GetModulePath() / "log_%N.log";
		boost::log::init_log_to_file(
			boost::log::keywords::file_name = log_path.string(),
			boost::log::keywords::rotation_size = 2 * 1024 * 1024,
			boost::log::keywords::format = "[%LineID%][%TimeStamp%][%ThreadID%][%Severity%]: %_%",
			boost::log::keywords::open_mode = std::ios_base::app
			);*/

		BOOST_LOG_TRIVIAL(trace) << "Запуск сервера сбора данных.";

		setlocale(LC_ALL, "");
		std::locale::global(std::locale("rus_rus.1251"));

		if ( instanceChecker_.IsAnotherRunning() )
		{
			BOOST_LOG_TRIVIAL(info) << "Один экземпляр приложения уже запущен.";
			return false;
		}

		frame_ = new MainFrame();
		frame_->LoadDefaultProject();
		frame_->Show();
		return true;
	}

	int OnExit()
	{
		BOOST_LOG_TRIVIAL(trace) << "Остановка сервера сбора данных.";
		return 0;
	}

	void OnUnhandledException()
	{
		BOOST_LOG_TRIVIAL(error) << "Необработанное исключение. Приложение аварийно закрывается";
	}
};

IMPLEMENT_APP(MyApp)