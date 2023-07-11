#pragma once
#include <wx/wx.h>
#include <fstream>

class ClickingThread : public wxThread
{
public:
	ClickingThread();              //Create new thread

private:
	virtual ExitCode Entry() override;

	bool IsMainFrameMinimized();   //Check if the app is open or minimized
	double FindDelay();            //Find the delay between each click
	void ClickMouse(int delay);

};

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
	virtual ~MainFrame();

private:
	void ConstructComponents();                       //Construct widgets
	void BindFunction();                              //Bind stop / start functions to button
	void StartClicker();                              //Function for stopping and starting
	void OnStartButtonClicked(wxCommandEvent& evt);   //Button to start clicker

	const short unsigned int START_ID = 0001;         //ID of the button
	ClickingThread* m_clickingThread;                 //Create instance of ClickingThread function

	wxPanel*      panel;
	wxButton*     button;
	wxTextCtrl*   textbox;
	wxStaticText* label;
};