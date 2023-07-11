#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	MainFrame* mainframe = new MainFrame("Auto Clicker");
	mainframe->SetClientSize(800, 200);
	mainframe->Center();
	mainframe->Show();

	return true;
}
