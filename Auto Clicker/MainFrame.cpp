#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/thread.h>
#include <Windows.h>
#include <time.h>
#include <wx/thread.h>
#include <fstream>

//All the code that goes in ClickingThread

ClickingThread::ClickingThread() : wxThread(wxTHREAD_DETACHED)
{

}

wxThread::ExitCode ClickingThread::Entry()
{
	double delay = FindDelay();

	Sleep(2000);
	while (!TestDestroy()) {              //The thread should not terminate
		if (IsMainFrameMinimized())       //If minimized start clicking
			ClickMouse(delay);
		else                              //If opened exit
			break;
	}

	return 0;
}

bool ClickingThread::IsMainFrameMinimized()
{
	wxFrame* mainFrame = wxStaticCast(wxTheApp->GetTopWindow(), wxFrame);
	return mainFrame && mainFrame->IsIconized();        //If mainFrame available and is minimized
}

double ClickingThread::FindDelay()
{
	std::ifstream file("delay.txt");

	if (!file.is_open())               //Returns -1 when file cannot be found
		return -1;

	int delay;
	file >> delay;

	file.close();
	return delay;
}

void ClickingThread::ClickMouse(int delay)
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	Sleep(delay);
}

//GUI functions


MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	ConstructComponents();
	BindFunction();
}

MainFrame::~MainFrame()
{
	if (m_clickingThread && m_clickingThread->IsAlive()) {    //If thread is running delete it
		m_clickingThread->Delete();
		m_clickingThread = nullptr;
	}
}

void MainFrame::ConstructComponents()
{
	wxFont font(wxFontInfo(wxSize(0, 24)).Bold());

	panel = new wxPanel(this);
	panel->SetFont(font);

	const wxString text = "Start Clicking";

	button  = new wxButton(panel, START_ID, text, wxDefaultPosition, wxSize(800, 100));
	label   = new wxStaticText(panel, wxID_ANY, "CPS: ", wxPoint(10, 120), wxDefaultSize);
	textbox = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(80, 120), wxSize(500, 30));
}

void MainFrame::BindFunction()
{
	Bind(wxEVT_BUTTON, &MainFrame::OnStartButtonClicked, this, START_ID);
}

void MainFrame::StartClicker()              
{
	//Add the delay to delay.txt
	std::fstream file("delay.txt", std::ios::out);

	wxString value = textbox->GetValue();

	int cps;
	value.ToInt(&cps);

	file << (1000 / cps); //milisecond equation is delay = 1000 / cps
	file.close();

	m_clickingThread = new ClickingThread();         //Create new thread
	m_clickingThread->Run();                         //Run new thread
}

void MainFrame::OnStartButtonClicked(wxCommandEvent& evt)
{
	StartClicker();
}